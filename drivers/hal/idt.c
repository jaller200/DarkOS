// This is where our Interrupt Descriptor Table (IDT) functions
// are stored

#include "idt.h"

#pragma pack (push, 1)
struct IDTR {
    unsigned short limit;
    unsigned int base;
};
#pragma pack (pop)

// Interrupt Descriptor Table
static struct IDT_Descriptor _idt [MAX_INTERRUPTS];
static struct IDTR           _idtr;

/*** IRQ Handlers ***/
static void default_handler();

// Default Handler
static void default_handler() {
    //ClearScreen();
    PrintString("[FAULT]: default_handler: Unhandled Exception");
    
    //halt_system();
    
    for (;;);
}

// Installs IDTR into processor's IDTR register
static void install_idt();

// Implementation
static void install_idt() {
    __asm__("lidt (%%eax)" : :"a"(&_idtr));
}

// Return an IDT Descriptor from IDT
IDT_Descriptor* get_ir(unsigned int ir) {
    if (ir > MAX_INTERRUPTS)
        return 0;
    
    return &_idt[ir];
}

// Install an IR in the IDT
int install_ir(unsigned int ir, unsigned short flags, unsigned short selector, IRQ_HANDLER irq) {
    if (ir > MAX_INTERRUPTS)
        return 0;
    
    if (!irq)
        return 0;
    
    unsigned long long base = (unsigned long long)&(*irq);
    
    _idt[ir].baseLow = (unsigned short)(base & 0xFFFF);
    _idt[ir].baseHigh = (unsigned short)((base >> 16) & 0xFFFF);
    _idt[ir].reserved = 0;
    _idt[ir].flags = (unsigned char)flags;
    _idt[ir].selector = selector;
    
    return 0;
}

// Initialize the IDT
int initialize_idt(unsigned short codeSelector) {
    _idtr.limit = sizeof(struct IDT_Descriptor) * MAX_INTERRUPTS - 1;
    _idtr.base = (unsigned int)&_idt[0];
    
    // Null out the idt
    memset((void*)&_idt[0], 0, sizeof(IDT_Descriptor) * MAX_INTERRUPTS - 1);
    
    //install_ir(0, IDT_DESC_PRESENT | IDT_DESC_BIT32, codeSelector, (IRQ_HANDLER)handler0);
    //install_ir(1, IDT_DESC_PRESENT | IDT_DESC_BIT32, codeSelector, (IRQ_HANDLER)handler1);
    //install_ir(2, IDT_DESC_PRESENT | IDT_DESC_BIT32, codeSelector, (IRQ_HANDLER)handler2);
    //install_ir(3, IDT_DESC_PRESENT | IDT_DESC_BIT32, codeSelector, (IRQ_HANDLER)handler3);
    //install_ir(4, IDT_DESC_PRESENT | IDT_DESC_BIT32, codeSelector, (IRQ_HANDLER)handler4);
    //install_ir(5, IDT_DESC_PRESENT | IDT_DESC_BIT32, codeSelector, (IRQ_HANDLER)handler5);
    
    int i = 0;
    while (i < MAX_INTERRUPTS) {
        install_ir(i, IDT_DESC_PRESENT | IDT_DESC_BIT32, codeSelector, (IRQ_HANDLER)default_handler);
        i++;
    }
    
    install_idt();
    
    return 0;
}