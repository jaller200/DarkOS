#include "cpu.h"

#include "../screen/vga.h"

#include "idt_handlers.h"
#include "registers.h"
#include "idt.h"
#include "gdt.h"

// Initializes CPU resources
int initialize_cpu() {
    initialize_gdt();
    PrintString("[OS] Initialized GDT\n");
    
    initialize_idt(0x8);
    PrintString("[OS] Initialized IDT...\n");
    
    setup_idt_handlers();
    PrintString("[OS] Finished Mapping IDT Handlers\n");
    
    //generate_interrupt(0x00);
    
    return 0;
}

// Shuts down CPU resources
void shutdown_cpu() {
    
}

// Get the CPU Vendor Name
char * cpu_get_vendor() {
    static int vendor[4] = {0};
    asm volatile("cpuid" : "=a"(vendor[3]), "=b"(vendor[0]), "=c"(vendor[2]), "=d"(vendor[1]) : "a"(0) : "cc");
    
    return (char*)vendor;
}