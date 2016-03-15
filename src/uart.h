#ifndef __UART__
#define __UART__

#include "char_dev.h"
#include "ring_buffer.h"
#include "types.h"

struct char_dev * uart_open(const char *file);

#endif /* __UART__ */
