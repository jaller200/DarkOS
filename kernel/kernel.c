// Include
#include "../drivers/screen/vga.h"
#include "../drivers/hal/hal.h"
#include "../drivers/hal/idt_handlers.h"

//void Divide0();

void main(void) {    
    ClearScreen();
    PrintString("[OS] Welcome to DarkOS\n");
    PrintString("[OS] Loading OS...\n");
    
    initialize_hal();
    enable();
    
    PrintString("[OS] Initialized Hardware Abstraction Layer\n");
    
    const char * cpu = get_cpu_vendor();
    PrintString("[OS] CPU Vendor Identified As: ");
    PrintString(cpu);
    
    //set_irq_handler(0, Divide0);
    
    //asm("int $0");
    // generate_interrupt(0x00);
    
    // int ticks = pit_get_tick_count();
    // Printf("Tick Count: %i\n", ticks);
    // sound(0xA0);
    
    // Test to generate illegal division by 0
    
    for (;;) {
        Printf("Tick Count: %i\n", pit_get_tick_count());
    }
}