#ifndef __GDT_H_INCLUDED__
#define __GDT_H_INCLUDED__

#include "../common/common.h"
#include "../common/string.h"

#define MAX_DESCRIPTORS     3       // Null, Code, and Data Descriptors

/*** GDT Descriptor Access Bit Flags ***/
#define GDT_DESC_ACCESS     0x0001  // 00000001 (Set Access Bit)
#define GDT_DESC_READWRITE  0x0002  // 00000010 (Descriptor is readable and writeable. Default: readonly)
#define GDT_EXPANSION       0x0004  // 00000100 (Set Expansion Direction Bit)
#define GDT_DESC_EXEC_CODE  0x0008  // 00001000 (Executable Code Segment. Default: Data Segment)
#define GDT_DESC_CODEDATA   0x0010  // 00010000 (Set Code or Data Descriptor. Default: System Defined Descriptor)
#define GDT_DESC_DPL        0x0060  // 01100000 (Set DPL Bits)
#define GDT_DESC_MEMORY     0x0080  // 10000000 (Set "In Memory" Bit)

/*** GDT Descriptor Grandularity Bit Flags ***/

#define GDT_GRAND_LIMIT_HI_MASK 0x0F    // 00001111 (Masks out limitHigh (High 4 bits of limit)
#define GDT_GRAND_OS            0x10    // 00010000 (Set OS Defined Bit)
#define GDT_GRAND_32BIT         0x40    // 01000000 (Set if 32 Bit. Default: 16 Bit)
#define GDT_GRAND_4K            0x80    // 10000000 (4K Grandularity. Default: None)

/****************************/

#pragma pack (push, 1)
// GDT Descriptor Struct
typedef struct GDT_Descriptor {
    
    unsigned short limit;   // Bits 0-15 of Segment Limit
    
    unsigned short baseLow; // Bits 0-23 of Base Address
    unsigned char baseMid;
    
    unsigned char flags;    // Descriptor Access Flags
    unsigned char grand;    // Grandularity Flags
    
    unsigned char baseHigh; // Bits 24-31 of Base Address
    
} GDT_Descriptor;
#pragma pack (pop)
 
/*** Functions ***/

// Setup a descriptor in the Global Descriptor Table
void set_gdt_descriptor(unsigned int i, unsigned long long base, unsigned long long limit, unsigned char access, unsigned char grand);

// Return Descriptor at index "i" in GDT
GDT_Descriptor *get_gdt_descriptor(int i);

// Initialize the GDT
int initialize_gdt();

#endif