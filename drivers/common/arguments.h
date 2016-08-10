#ifndef __ARGUMENTS_H_INCLUDED__
#define __ARGUMENTS_H_INCLUDED__

#include "va_list.h"

#define STACKITEM int       // Width of stack == width of int

// Round up width of objects push on stack.
// The expression before the "&" insures that we get 0 for objects of size 0
#define VA_SIZE(TYPE) ((sizeof(TYPE) + sizeof(STACKITEM) - 1) & ~(sizeof(STACKITEM) - 1))

// &(LASTARG) points to the LEFTMOST argument in function call
// (before the ...)
#define va_start(AP, LASTARG) (AP=((va_list)&(LASTARG) + VA_SIZE(LASTARG)))

// Nothing for va_end
#define va_end(AP)

#define va_arg(AP, TYPE) (AP += VA_SIZE(TYPE), *((TYPE *)(AP - VA_SIZE(TYPE))))

#endif