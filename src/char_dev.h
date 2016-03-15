#ifndef __CHAR_DEV__
#define __CHAR_DEV__

#include "bits.h"
#include "types.h"

#define MAX_FDS 16

#define FD_KOUT 0

struct char_dev
{
  void (*close)(struct char_dev *dev);
  ssize_t (*read)(struct char_dev *dev, void *buff, size_t nbytes);
  ssize_t (*write)(struct char_dev *dev, const void *buff, size_t nbytes);
  int (*ioctl)(struct char_dev *dev, int op, void *data);
  uint32_t flags;
};

/* I/O return flags */
#define IO_ERROR -1
#define IO_BUSY -2

#define IOCTL_SET_FLAGS 0
# define IOCTL_ASYNC _b(1)

void char_dev_init();

int char_dev_open(const char *file);
void char_dev_close(int fd);
ssize_t char_dev_read(int fd, void *buff, size_t nbytes);
ssize_t char_dev_write(int fd, const void *buff, size_t nbytes);
int char_dev_ioctl(int fd, int op, void *data);

#endif /* __CHAR_DEV__ */
