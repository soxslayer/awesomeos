#include "stdio.h"
#include "args.h"
#include "io.h"

static int format_decimal(char *buff, size_t buff_size, int d)
{
  int i;
  int j;
  int neg = 0;

  if (!buff_size) return 0;

  if (!d) {
    buff[0] = '0';
    return 1;
  }

  for (i = 0; i < buff_size; ++i)
    buff[i] = ' ';

  if (d < 0) {
    neg = 1;
    d *= -1;
  }

  /* this will format the number placing the least significant digit at the
   * highest index in buff */
  for (i = 0; i < buff_size && d > 0; ++i) {
    buff[buff_size - i - 1] = (char)(d % 10) + '0';
    d /= 10;
  }

  if (neg) buff[buff_size - i++ - 1] = '-';

  if (i < buff_size) {
    /* slide everything over to start at index 0 of buff */
    for (j = 0; j < i; ++j)
      buff[j] = buff[buff_size - i + j];
  }

  return i;
}

static int format_hex(char *buff, size_t buff_size, int d)
{
  int mask = 28;
  int i;
  int start = 0;

  if (!buff_size) return 0;

  if (!d) {
    buff[0] = '0';
    return 1;
  }

  for (i = 0; mask >= 0 && i < buff_size; mask -= 4) {
    int nibble = (d >> mask) & 0xf;

    if (nibble) start = 1;

    if (start) {
      if (nibble >= 10) buff[i] = nibble + 'W';
      else buff[i] = nibble + '0';

      ++i;
    }
  }

  return i;
}

void fprintf(int fd, const char *fmt, ...)
{
  va_list l;
  const char *p = fmt;
  char scratch[16];
  int scratch_ptr = 0;

  va_start(l, fmt);

#define flush_scratch() \
  if (scratch_ptr) write(fd, scratch, scratch_ptr), scratch_ptr = 0

#define scratch_add(c) \
  do { \
    scratch[scratch_ptr++] = c; \
    if (scratch_ptr == sizeof(scratch)) flush_scratch(); \
  } while (0)

  for (; *p; ++p) {
    if (*p == '%') {
      ++p;

      if (*p == '%') {
        scratch_add('%');
      }
      else {
        if (!p) break; /* What!!! */

        flush_scratch();

        switch (*p) {
          case 'd': {
            int d = va_arg(l, int);

            scratch_ptr = format_decimal (scratch, sizeof(scratch), d);

            break;
          }

          case 'X':
          case 'x': {
            int d = va_arg(l, int);

            scratch_ptr = format_hex(scratch, sizeof(scratch), d);

            break;
          }
        }

        flush_scratch();
      }
    }
    else if (*p == '\n') {
      scratch_add('\r');
      scratch_add('\n');
    }
    else {
      scratch_add(*p);
    }
  }

  flush_scratch();

  va_end(l);

#undef scratch_add
#undef flush_scratch
}
