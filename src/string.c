#include "string.h"

int strncmp(const char *str1, const char *str2, size_t nbytes)
{
  int i;

  for (i = 0; i < nbytes; ++i) {
    if (str1[i] < str2[i]) return -1;
    else if (str1[i] > str2[i]) return 1;
    else if (str1[i] == 0 && str2[i] != 0) return -1;
    else if (str1[i] != 0 && str2[i] == 0) return 1;
  }

  return 0;
}

size_t strnlen(const char *str, size_t nbytes)
{
  int i;

  for (i = 0; i < nbytes; ++i)
    if (str[i] == 0) break;

  return i;
}

void memset(void *r, size_t nbytes, uint8_t value)
{
  int i;

  for (i = 0; i < nbytes; ++i) ((char*)r)[i] = value;
}
