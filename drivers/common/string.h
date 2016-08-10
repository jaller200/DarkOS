#ifndef __STRING_H_INCLUDED__
#define __STRING_H_INCLUDED__

#include "common.h"

char *strcpy(char *str1, const char *str2);
size_t strlen(const char *str);

void * memcpy(void *dest, const void *src, size_t count);
void * memset(void *dest, char val, size_t count);
unsigned short * memsetw(unsigned short *dest, unsigned short val, size_t count);

#endif