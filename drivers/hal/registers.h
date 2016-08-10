#ifndef __REGISTERS_H_INCLUDED__
#define __REGISTERS_H_INCLUDED__

// This class contains register structures and declarations

#include "../common/common.h"

// 32-Bit Registers
struct R32BIT {
    unsigned int eax, ebx, ecx, edx, esi, edi, ebp, esp, eflags;
    unsigned char cflag;
};

// 16-Bit Registers
struct R16BIT {
    unsigned short ax, bx, cx, dx, si, di, bp, sp, es, cs, ss, ds, flags;
    unsigned char cflag;
};

// 16-Bit Registers expressed in 32-Bit Registers
struct R16BIT32 {
    unsigned short ax, axh, bx, bxh, cx, cxh, dx, dxh;
    unsigned short si, di, bp, sp, es, cs, ss, ds, flags;
    unsigned char cflag;
};

// 8-Bit Registers
struct R8BIT {
    unsigned char al, ah, bl, bh, cl, ch, dl, dh;
};
// 8-Bit Registers expressed in 32-Bit Registers
struct R8BIT32 {
    unsigned char al, ah; unsigned short axh;
    unsigned char bl, bh; unsigned short bxh;
    unsigned char cl, ch; unsigned short cxh;
    unsigned char dl, dh; unsigned short dxh;
};

// 8-Bit and 16-Bit Registers union
union INTR16 {
    struct R16BIT x;
    struct R8BIT h;
};

// 32-Bit, 16-Bit, and 8-Bit Registers union
union INTR32 {
    struct R32BIT x;
    struct R16BIT32 l;
    struct R8BIT32 h;
};

#endif