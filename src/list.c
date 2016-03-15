#include "list.h"

void list_init (struct list_t *list)
{
  list->next = 0;
  list->prev = 0;
}

void list_add (struct list_t *list, struct list_t *node)
{
  node->prev = list->prev;
  node->next = list;
  if (list->prev) list->prev->next = node;
  list->prev = node;
}

void list_remove (struct list_t *node)
{
  if (node->prev) node->prev->next = node->next;
  if (node->next) node->next->prev = node->prev;
}
