#include "gdt.h"

#pragma pack (push, 1)
// GDTR Structure
typedef struct GDTR {
    
    unsigned short limit;   // Size of GDT
    unsigned int base;      // Base Address of GDT
    
} GDTR;
#pragma pack (pop)

static struct GDT_Descriptor _gdt[MAX_DESCRIPTORS]; // Our GDT Descriptors
static struct GDTR _gdtr;                           // Our Global Descriptor Table

// Declaration of "install_gdt()"
static void install_gdt();

// Implementation of "install_gdt()"
static void install_gdt() {
    __asm__("lgdt (%%eax)" : :"a" (&_gdtr));
    
    asm("movw $0x10, %ax");
    asm("movw %ax, %ds");
    asm("movw %ax, %es");
    asm("movw %ax, %fs");
    asm("movw %ax, %gs");
}

// Setup Descriptor in Global Descriptor Table
void set_gdt_descriptor(unsigned int i, unsigned long long base, unsigned long long limit, unsigned char access, unsigned char grand) {
    if (i > MAX_DESCRIPTORS)
        return;
    
    // Null out the descriptor
    memset((void*)&_gdt[i], 0, sizeof(GDT_Descriptor));
    
    // Set limit and base address
    _gdt[i].baseLow = (unsigned short)(base & 0xFFFF);
    _gdt[i].baseMid = (unsigned char)((base >> 16) & 0xFF);
    _gdt[i].baseHigh = (unsigned char)((base >> 24) & 0xFF);
    _gdt[i].limit = (unsigned short)(limit & 0xFFFF);
    
    // Set flags and grandularity bytes
    _gdt[i].flags = access;
    _gdt[i].grand = (unsigned char)((limit >> 16) & 0x0F);
    _gdt[i].grand |= grand & 0xF0;
}

// Returns Descriptor in GDT
GDT_Descriptor *get_gdt_descriptor(int i) {
    if (i > MAX_DESCRIPTORS)
        return 0;
    
    return &_gdt[i];
}

// Initialize GDT
int initialize_gdt() {
    // Set up GDTR
    _gdtr.limit = (sizeof(struct GDT_Descriptor) * MAX_DESCRIPTORS) - 1;
    _gdtr.base = (unsigned int)&_gdt[0];
    
    // Set null descriptor
    set_gdt_descriptor(0, 0, 0, 0, 0);
    
    // Set default code descriptor
    set_gdt_descriptor(1, 0, 0xFFFFFFFF,
                       GDT_DESC_READWRITE | GDT_DESC_EXEC_CODE | GDT_DESC_CODEDATA | GDT_DESC_MEMORY,
                       GDT_GRAND_4K | GDT_GRAND_32BIT | GDT_GRAND_LIMIT_HI_MASK);
    
    // FLAGS
    // 00000000 -> 10011010
    
    // Set default data descriptor
    set_gdt_descriptor(2, 0, 0xFFFFFFFF,
                       GDT_DESC_READWRITE | GDT_DESC_CODEDATA | GDT_DESC_MEMORY,
                       GDT_GRAND_4K | GDT_GRAND_32BIT | GDT_GRAND_LIMIT_HI_MASK);
    
    // 10010010
    
    // Install GDTR
    install_gdt();
    
    return 0;
}