#ifndef __RING_BUFER__
#define __RING_BUFER__

#include "types.h"

#define RING_BUFFER_SIZE_N_BITS 10
#define RING_BUFFER_SIZE (1 << RING_BUFFER_SIZE_N_BITS)

struct ring_buffer_t
{
  int read_ptr;
  int write_ptr;
  char buff[RING_BUFFER_SIZE];
  size_t cnt;
};

void ring_buffer_init(struct ring_buffer_t *rb);
size_t ring_buffer_size(struct ring_buffer_t *rb);
ssize_t ring_buffer_write(struct ring_buffer_t *rb, const void *buff,
                          size_t size);
ssize_t ring_buffer_read(struct ring_buffer_t *rb, void *buff, size_t size);

#endif /* __RING_BUFER__ */
