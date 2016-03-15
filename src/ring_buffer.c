#include "ring_buffer.h"

#define RING_BUFFER_MOD(a) ((a) & (RING_BUFFER_SIZE - 1))

void ring_buffer_init(struct ring_buffer_t *rb)
{
  rb->read_ptr = 0;
  rb->write_ptr = 0;
  rb->cnt = 0;
}

size_t ring_buffer_size(struct ring_buffer_t *rb)
{
  return rb->cnt;
}

ssize_t ring_buffer_write(struct ring_buffer_t *rb, const void *buff,
                          size_t size)
{
  int n_write_ptr = rb->write_ptr;
  int n_wrote;

  for (n_wrote = 0; size; ++n_wrote, --size) {
    n_write_ptr = RING_BUFFER_MOD(n_write_ptr + 1);

    /* buffer full */
    if (n_write_ptr == rb->read_ptr)
      break;

    rb->buff[rb->write_ptr] = ((const char *)buff)[n_wrote];
    rb->write_ptr = n_write_ptr;
  }

  rb->cnt += n_wrote;

  return n_wrote;
}

ssize_t ring_buffer_read(struct ring_buffer_t *rb, void *buff, size_t size)
{
  int n_read_ptr = rb->read_ptr;
  int n_read;

  if (rb->read_ptr == rb->write_ptr)
    return 0;

  for (n_read = 0; size && rb->read_ptr != rb->write_ptr; ++n_read, --size) {
    n_read_ptr = RING_BUFFER_MOD(n_read_ptr + 1);

    ((char *)buff)[n_read] = rb->buff[rb->read_ptr];
    rb->read_ptr = n_read_ptr;
  }

  rb->cnt -= n_read;

  return n_read;
}
