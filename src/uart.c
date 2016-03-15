#include "uart.h"

#include "bits.h"
#include "irq.h"
#include "mem.h"
#include "reg.h"
//#include "sem.h"
#include "types.h"
#include "usr_led.h"

#define UART_FIFO_SIZE 64

enum tx_trigger_level_t
{
  TRIGGER_8 = 8,
  TRIGGER_16 = 16,
  TRIGGER_32 = 32,
  TRIGGER_56 = 56
};

enum uart_mode_t
{
  MODE_OPERATIONAL,
  MODE_A,
  MODE_B
};

struct uart_t
{
  struct char_dev cdev;
  addr_t base;
  struct ring_buffer_t tx_buff;
  struct ring_buffer_t rx_buff;
  //struct sem_t lock;
  enum tx_trigger_level_t trigger_lvl;
};

static struct uart_t _uart_dev;

static void uart_write_nibble(struct uart_t *uart, uint8_t nibble)
{
  if (nibble >= 10)
    UART_THR(uart->base) = nibble + 'W';
  else
    UART_THR(uart->base) = nibble + '0';
}

static void uart_write_byte(struct uart_t *uart, uint8_t byte)
{
  uart_write_nibble(uart, byte >> 4);
  uart_write_nibble(uart, byte & 0xf);
}

static void uart_write_int(struct uart_t *uart, uint32_t i)
{
  uart_write_byte(uart, i >> 24);
  uart_write_byte(uart, i >> 16);
  uart_write_byte(uart, i >> 8);
  uart_write_byte(uart, i & 0xff);
}

static void uart_enable_rx_interrupt(struct uart_t *uart)
{
  set_bit(UART_IER(uart->base), UART_IER_RHRIT);
}

static void uart_disable_rx_interrupt(struct uart_t *uart)
{
  clr_bit(UART_IER(uart->base), UART_IER_RHRIT);
}

static void uart_enable_tx_interrupt(struct uart_t *uart)
{
  set_bit(UART_IER(uart->base), UART_IER_THRIT);
}

static void uart_disable_tx_interrupt(struct uart_t *uart)
{
  clr_bit(UART_IER(uart->base), UART_IER_THRIT);
}

static ssize_t uart_write(struct char_dev *cdev, const void *buff,
                           size_t size)
{
  struct uart_t *uart = (struct uart_t *)cdev;
  size_t n_wrote;

  //sem_take(&uart->lock);

  n_wrote = ring_buffer_write(&uart->tx_buff, buff, size);
  uart_enable_tx_interrupt(uart);

  if (n_wrote == 0)
    return IO_BUSY;

  //sem_give(&uart->lock);

  return n_wrote;
}

static ssize_t uart_read(struct char_dev *cdev, void *buff, size_t size)
{
  return 0;
}

static void uart_close(struct char_dev *cdev)
{
}

static void uart_set_mode(struct uart_t *uart, enum uart_mode_t mode)
{
  switch (mode) {
    case MODE_OPERATIONAL:
      clr_bit(UART_LCR(uart->base), UART_LCR_DIV_EN);
      break;

    case MODE_A:
      set_bit(UART_LCR(uart->base), UART_LCR_DIV_EN);
      clr_bit(UART_LCR(uart->base), UART_LCR_BREAK_EN);
      break;

    case MODE_B:
      UART_LCR(uart->base) = 0xbf;
      break;
  }
}

static uint16_t uart_disable_baud_clock(struct uart_t *uart)
{
  uint16_t old_div = 0;

  uart_set_mode(uart, MODE_A);

  old_div = UART_DLH(uart->base) << 8;
  old_div |= UART_DLL(uart->base);

  UART_DLH(uart->base) = 0;
  UART_DLL(uart->base) = 0;

  uart_set_mode(uart, MODE_OPERATIONAL);

  return old_div;
}

static void uart_enable_baud_clock(struct uart_t *uart, uint16_t clk_div)
{
  uart_set_mode(uart, MODE_A);
  UART_DLL(uart->base) = clk_div & 0xff;
  UART_DLH(uart->base) = clk_div >> 8;
  uart_set_mode(uart, MODE_OPERATIONAL);
}

static void uart_enable_fifo(struct uart_t *uart)
{
  uint16_t old_div = uart_disable_baud_clock(uart);

  set_bit(UART_FCR(uart->base), UART_FCR_FIFO_EN);

  uart_enable_baud_clock(uart, old_div);
}

static void uart_disable_fifo(struct uart_t *uart)
{
  uint16_t old_div = uart_disable_baud_clock(uart);

  clr_bit(UART_FCR(uart->base), UART_FCR_FIFO_EN);

  uart_enable_baud_clock(uart, old_div);
}

static void uart_set_tx_trigger_level(struct uart_t *uart,
                                       enum tx_trigger_level_t lvl)
{
  int mask;

  set_bit(UART_EFR(uart->base), UART_EFR_ENHANCEDEN);

  switch (lvl) {
    case TRIGGER_8: mask = 0; break;
    case TRIGGER_16: mask = 1; break;
    case TRIGGER_32: mask = 2; break;
    case TRIGGER_56: mask = 3; break;
  }

  UART_FCR(uart->base) |= mask << UART_FCR_TX_FIFO_TRIG;
  uart->trigger_lvl = lvl;

  clr_bit(UART_EFR(uart->base), UART_EFR_ENHANCEDEN);
}

static void uart_set_rx_trigger_level(struct uart_t *uart,
                                      enum tx_trigger_level_t lvl)
{
  UART_FCR(uart->base) |= lvl << UART_FCR_RX_FIFO_TRIG;
}

static void uart_irq_handler(int int_num)
{
  struct uart_t *uart;

  switch (int_num) {
    case INTC_UART0INT: uart = &_uart_dev; break;
    default: return;
  }

  int src = (UART_IIR(uart->base) & UART_IIR_IT_TYPE_MASK)
            >> UART_IIR_IT_TYPE;
  size_t n_fifo;

  switch (src) {
    case UART_IIR_THR: {
      char b[UART_FIFO_SIZE];
      n_fifo = UART_FIFO_SIZE - UART_TXFIFO_LVL(uart->base);
      ssize_t n_tx = ring_buffer_read(&uart->tx_buff, b, n_fifo);
      int i;

      for (i = 0; i < n_tx; ++i) UART_THR(uart->base) = b[i];

      if (!ring_buffer_size(&uart->tx_buff))
        uart_disable_tx_interrupt(uart);

      break;
    }
  }
}



struct char_dev * uart_open(const char *file)
{
  _uart_dev.cdev.close = uart_close;
  _uart_dev.cdev.read = uart_read;
  _uart_dev.cdev.write = uart_write;
  _uart_dev.base = UART0_BASE;

  ring_buffer_init(&_uart_dev.tx_buff);
  ring_buffer_init(&_uart_dev.rx_buff);

  //sem_init(&_uart_dev.lock, 1);

  uart_enable_fifo(&_uart_dev);
  uart_set_rx_trigger_level(&_uart_dev, TRIGGER_32);

  irq_register_handler(INTC_UART0INT, uart_irq_handler);

  barrier();

  //uart_enable_rx_interrupt(&_uart_dev);

  set_bit(INTC_MIR_CLEAR2, INTC_INT32_BIT(INTC_UART0INT));

  return &_uart_dev.cdev;
}
