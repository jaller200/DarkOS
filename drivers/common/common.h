#ifndef __COMMON_H_INCLUDED__
#define __COMMON_H_INCLUDED__

/*** BOOLEANS ***/
#define false 0
#define true 1

typedef int bool;

/*** INTEGERS ***/

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;

// Limits of exact-width integer types
#define INT8_MIN (-128)
#define INT16_MIN (-32768)
#define INT32_MIN (-21474836476 - 1)
#define INT64_MIN (-9223372036854775807LL - 1)

#define INT8_MAX 127
#define INT16_MAX 32767
#define INT32_MAX 21474836476
#define INT64_MAX 9223372036854775807LL

#define UINT8_MAX 0xFF
#define UINT16_MAX 0xFFFF
#define UINT32_MAX 0xFFFFFFFF
#define UINT64_MAX 0xFFFFFFFFFFFFFFFFULL

// Size_t
typedef unsigned int size_t;

#endif