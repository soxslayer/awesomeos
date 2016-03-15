#include "mem.h"

#include "arm.h"
//#include "sem.h"
//#include "list.h"

void barrier()
{
  __dsb();
  asm volatile("" ::: "memory");
}

#if 0
struct heap_free_node_t
{
  struct list_t list;
  size_t size;
};

struct heap_alloc_node_t
{
  size_t size;
};

__attribute__((section(".heap")))
char __HEAP;
__attribute__((section(".heap.end")))
char __HEAP_END;

struct sem_t _heap_sem;
struct heap_free_node_t *_free_list;

void mem_init()
{
  _free_list = (struct heap_free_node_t *)&__HEAP;

  sem_init(&_heap_sem, 1);

  list_init(&_free_list->list);
  _free_list->size = (size_t)(&__HEAP_END - &__HEAP);
}

void * malloc(size_t nbytes)
{
  struct heap_free_node_t *t;
  void *ret = 0;
  size_t alloc_size = nbytes + sizeof(struct heap_alloc_node_t);

  sem_take(&_heap_sem);

  t = _free_list;

  while (t) {
    if (t->size >= alloc_size) {
      struct heap_alloc_node_t *ptr = (struct heap_alloc_node_t *)t;
      size_t n_free_size = t->size - alloc_size;

      if (list_head(&t->list))
        _free_list = (struct heap_free_node_t *)_free_list->list.next;

      list_remove(&t->list);

      if (n_free_size < sizeof(struct heap_free_node_t)) {
        /* not enough room for a free node in the remaining free space,
         * allocate it all */
        alloc_size = t->size;
      }
      else {
        /* split the free node and add it back to the list */
        t = (struct heap_free_node_t *)((char *)t + alloc_size);
        t->size = n_free_size;
        if (_free_list) list_add(&_free_list->list, &t->list);
        _free_list = t;
      }

      ptr->size = alloc_size;
      ret = (char *)ptr + sizeof(struct heap_alloc_node_t);

      break;
    }

    t = (struct heap_free_node_t *)t->list.next;
  }

  sem_give(&_heap_sem);

  return ret;
}

void free(void *ptr)
{
  struct heap_alloc_node_t *an = (struct heap_alloc_node_t *)
    ((char*)ptr - sizeof(struct heap_alloc_node_t));
  struct heap_free_node_t *fn = (struct heap_free_node_t *)an;

  sem_take(&_heap_sem);

  fn->size = an->size;
  if (_free_list) list_add(&_free_list->list, &fn->list);
  _free_list = fn;

  sem_give(&_heap_sem);
}
#endif
