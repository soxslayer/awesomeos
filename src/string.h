#pragma once

#include "types.h"

int strncmp(const char *str1, const char *str2, size_t nbytes);
size_t strnlen(const char *str, size_t nbytes);
void memset(void *r, size_t nbytes, uint8_t value);
