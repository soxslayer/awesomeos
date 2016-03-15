#pragma once

#include "types.h"

typedef char * va_list;

#define va_start(l,p) l = (va_list)&(p) + sizeof(uint32_t)
#define va_arg(l,t) (l += sizeof(uint32_t), *(t*)(l - sizeof(uint32_t)))
#define va_end(l) l = 0
