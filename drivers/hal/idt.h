#ifndef __IDT_H_INCLUDED__
#define __IDT_H_INCLUDED__

#include "../screen/vga.h"
#include "../common/string.h"

#define MAX_INTERRUPTS      256     // The total number of interrupts in our IDT

/*** IDT Descriptor Bits ***/

// Must be in the format 0D110, where D is descriptor type
#define IDT_DESC_BIT16      0x06    // 00000110
#define IDT_DESC_BIT32      0x0E    // 00001110
#define IDT_DESC_RING1      0x40    // 01000000
#define IDT_DESC_RING2      0x20    // 00100000
#define IDT_DESC_RING3      0x60    // 01100000
#define IDT_DESC_PRESENT    0x80    // 10000000

/*** Constants & Structures ***/

// Interrupt handler w/o error code
typedef void (*IRQ_HANDLER)(void);

#pragma pack (push, 1)
typedef struct IDT_Descriptor {
    
    unsigned short baseLow;     // This is the lower portion of the IR Address (bits 0-15)
    unsigned short selector;    // This is the Code Selector in the GDT (0x0008)
    unsigned char reserved;     // Reserved - should be 0
    unsigned char flags;        // These are the IDT Descriptor Flags (defined above)
    unsigned short baseHigh;    // This is the higher portion of the IR Address (bits 16-31)
    
} IDT_Descriptor;
#pragma pack (pop)

/*** Functions ***/

IDT_Descriptor *get_ir(unsigned int ir);
int install_ir(unsigned int ir, unsigned short flags, unsigned short selector, IRQ_HANDLER);
int initialize_idt(unsigned short codeSelector);

#endif