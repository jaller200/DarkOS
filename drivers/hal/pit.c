#include "pit.h"
#include "hal.h"

#include "../common/ports.h"
#include "../common/common.h"

/*** Constants ***/
#define PIT_REG_COUNTER0    0x40    // Software Port for Counter 0
#define PIT_REG_COUNTER1    0x41    // Software Port for Counter 1
#define PIT_REG_COUNTER2    0x42    // Software Port for Counter 2
#define PIT_REG_COMMAND     0x43    // Software Port for Command Register

/*** Global ***/
static volatile pit_ticks = 0;          // This is our global tick count
static bool pit_initialized = false;

/*** IRQs ***/
void pit_irq();

void pit_irq() {
    asm("addl $10, %esp");
    asm("pushal");
    
    pit_ticks++;
    PrintString("\n");
    
    interrupt_done(0);
    
    asm("popal");
    asm("iret");
}

static void bobby();

static void bobby() {
    PrintString("BOBBY!");
}

/*** Functions ***/

void initialize_pit() {
    //set_irq_handler(32, pit_irq);    // Set the PIT IRQ to entry 32 of the IVT
    pit_initialized = true;
    enable();
}

bool is_pit_initialized() {
    return pit_initialized;
}

// Send data to PIT counter
void pit_send_data(unsigned short data, unsigned char counter) {
    unsigned char port = (counter == PIT_OCW_COUNTER_0) ? PIT_REG_COUNTER0 : ((counter == PIT_OCW_COUNTER_1) ? PIT_REG_COUNTER1 : PIT_REG_COUNTER2);
    port_byte_out(port, (unsigned char)data);
}

// Send a command to the PIT
void pit_send_command(unsigned char command) {
    port_byte_out(PIT_REG_COMMAND, command);
}

// Read data from PIT
unsigned char pit_read_data(unsigned short counter) {
    unsigned char port = (counter == PIT_OCW_COUNTER_0) ? PIT_REG_COUNTER0 : ((counter == PIT_OCW_COUNTER_1) ? PIT_REG_COUNTER1 : PIT_REG_COUNTER2);
    return port_byte_in(port);
}

// Get the tick count
unsigned int pit_get_tick_count() {
    return pit_ticks;
}

// Set the tick count and return the previous count
unsigned int pit_set_tick_count(unsigned int tick) {
    unsigned int prev = pit_ticks;
    pit_ticks = tick;
    return prev;
}

// Start PIT Counter
void pit_start_counter(unsigned int freq, unsigned char counter, unsigned char mode) {
    // Can't have no frequency
    if (freq == 0)
        return;
    
    unsigned short divisor = (unsigned short)(1193181 / (unsigned short)freq);  // This is our COUNT
    
    // Send operation command
    unsigned char ocw = 0;
    ocw = (ocw & ~PIT_OCW_MASK_MODE) | mode;
    ocw = (ocw & ~PIT_OCW_MASK_RL) | PIT_OCW_RL_LSB_MSB;
    ocw = (ocw & ~PIT_OCW_MASK_COUNTER) | counter;
    
    // Send the Control Word to the PIT
    pit_send_command(ocw);
    
    // Now send the frequency and start the timer
    pit_send_data(divisor & 0xFF, 0);
    pit_send_data((divisor >> 8) & 0xFF, 0);
    
    // Set the ticks
    pit_ticks = 0;
}