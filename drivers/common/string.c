#include "string.h"

// Copy str1 into str2
char *strcpy(char *str1, const char *str2) {
    char *str1_p = str1;
    while (*str1++ = *str2++);
    return str1_p;
}

// Return the size of a string
size_t strlen(const char *str) {
    size_t len = 0;
    while (str[len++]);
    return len;
}

// Copy bytes from "src" to "dest"
void *memcpy(void *dest, const void *src, size_t count) {
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for (; count != 0; count--)
        *dp++ = *sp++;
    return dest;
}

// Fill byte in "dest" with value
void *memset(void *dest, char val, size_t count) {
    unsigned char *temp = (unsigned char *)dest;
    while (count != 0) {
        count--;
        temp[count] = val;
    }
    //for (; count != 0; count--, temp[count] = val);
    return dest;
}

// Fill word (short) in "dest" with value
unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count) {
    unsigned short *temp = (unsigned short *)dest;
    for (; count != 0; count--)
        *temp++ = val;
    return dest;
}