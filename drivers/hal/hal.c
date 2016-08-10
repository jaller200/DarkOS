#include "hal.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "pit.h"

#include "../screen/vga.h"

// Initialize hardware devices
int initialize_hal() {
    initialize_cpu();
    
    initialize_pic(0x20, 0x28);
    PrintString("[OS] Initialized PIC\n");
    
    initialize_pit();
    PrintString("[OS] Initialized PIT\n");
    
    pit_start_counter(100, PIT_OCW_COUNTER_0, PIT_OCW_MODE_WAVE);
    PrintString("[OS] Started PIT Counter 0\n");
    
    enable();
    
    return 0;
}

// Shutdown hardware devices
int shutdown_hal() {
    shutdown_cpu();
    
    return 0;
}

// Assembly to start interrupt
void intstart() {
    asm("addw $12, %esp");
    asm("pushal");
}

// IRET
void intret() {
    asm("popal");
    asm("iretl");
}

// Halt the system
void halt_system() {
    disable();
    asm("hlt");
}

// Generate interrupt call
void generate_interrupt(unsigned char n) {
    asm(".code32");
    asm volatile("mov %0, %%al" : "=r" (n));
    asm("mov %al, genint+1");
    asm("jmp genint");
    asm("genint:");
    asm("int $0");
}

// Send EOI to PIC
void interrupt_done(unsigned char n) {
    // Check if valid IRQ
    if (n > 16)
        return;
    
    // Test is we need to send EOI to second PIC
    if (n >= 8)
        pic_send_command(PIC_OCW2_MASK_EOI, 1);
    
    pic_send_command(PIC_OCW2_MASK_EOI, 0);
}

bool interrupt_mask(unsigned char n, bool enable) {
    // TODO: Create Function to mask interrupt number
}

// Set IRQ vector
void set_irq_handler(unsigned char n, IRQ_HANDLER irq) {
    install_ir(n, IDT_DESC_PRESENT | IDT_DESC_BIT32, 0x8, irq);
}

// Fetch an IRQ vector
IRQ_HANDLER get_irq_handler(unsigned char n) {
    IDT_Descriptor *desc = get_ir(n);
    if (!desc)
        return 0;       // No IR found
    
    unsigned int addr = desc->baseLow | (desc -> baseHigh << 16);   // Get the address
    
    IRQ_HANDLER irq = (IRQ_HANDLER)addr;
    return irq;
}

// Output Sound to Speaker
void sound(unsigned int frequency) {
    port_byte_out(0x61, 3 | (unsigned char)(frequency << 2));
}

void enable() {
    asm("sti");
}

void disable() {
    asm("cli");
}

const char * get_cpu_vendor() {
    return cpu_get_vendor();
}

int get_tick_count() {
    return pit_get_tick_count();
}