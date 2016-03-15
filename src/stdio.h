#pragma once

#include "io.h"

/* DOES NOT SUPPORT FLOATS */
void fprintf(int fd, const char *fmt, ...);

#define printf(fmt,...) fprintf(stdout,fmt,##__VA_ARGS__)
