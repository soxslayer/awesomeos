#include "char_dev.h"
#include "string.h"
#include "uart.h"

static struct char_dev * _fds[MAX_FDS];

void char_dev_init()
{
  memset(_fds, sizeof(_fds), 0);

  _fds[FD_KOUT] = uart_open(":uart0");
}

int char_dev_open(const char *file)
{
  int i;
  int fd = -1;

  for (i = 0; i < MAX_FDS; ++i) {
    if (!_fds[i]) {
      fd = i;
      break;
    }
  }

  if (fd == -1)
    return IO_ERROR;

  /* Only support device files right now */
  if (file[0] != ':')
    return IO_ERROR;

  /* TODO Create some sort of registration system for this */
  if (strncmp(file, ":uart", 5) == 0)
    _fds[fd] = uart_open(file);

  _fds[fd]->flags = 0;

  return fd;
}

void char_dev_close(int fd)
{
  if (fd < 0 || fd >= MAX_FDS || _fds[fd] == 0)
    return;

  _fds[fd]->close(_fds[fd]);
  _fds[fd] = 0;
}

ssize_t char_dev_read(int fd, void *buff, size_t nbytes)
{
  return IO_ERROR;
}

ssize_t char_dev_write(int fd, const void *buff, size_t nbytes)
{
  if (fd < 0 || fd >= MAX_FDS || _fds[fd] == 0)
    return IO_ERROR;

  ssize_t n_wrote = 0;

  while (n_wrote < nbytes) {
    ssize_t r = _fds[fd]->write(_fds[fd],
                                ((const char *)buff) + n_wrote,
                                nbytes - n_wrote);

    if (r == 0)
      break;
    else if (r == -1)
      return IO_ERROR;
    else if (r > 0)
      n_wrote += r;

    if (_fds[fd]->flags & IOCTL_ASYNC)
      break;
  }

  return n_wrote;
}

int char_dev_ioctl(int fd, int op, void *data)
{
  if (fd < 0 || fd >= MAX_FDS || _fds[fd] == 0)
    return IO_ERROR;

  switch (op) {
    case IOCTL_SET_FLAGS:
      _fds[fd]->flags = (uint32_t)data;
      break;

    default:
      return _fds[fd]->ioctl(_fds[fd], op, data);
  }

  return IO_ERROR;
}
