#pragma once

#if 0
#include "types.h"

#ifndef NULL
# define NULL (void*)0
#endif

void mem_init();

void * malloc(size_t nbytes);
void free(void *ptr);
#endif

void barrier();
