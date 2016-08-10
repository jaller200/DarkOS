#ifndef __HAL_H_INCLUDED__
#define __HAL_H_INCLUDED__

#include "../common/common.h"

#include "cpu.h"

#define near
#define far

typedef void (*IRQ_HANDLER)(void);

// Initialize Hardware Layer Abstraction
int initialize_hal();

// Shutdown Hardware Layer Abstraction
int shutdown_hal();

// Generates Interrupt
void generate_interrupt(unsigned char n);
void interrupt_done(unsigned char n);
bool interrupt_mask(unsigned char n, bool enable);

void enable();  // Enable interrupts
void disable(); // Disable interrupts

void intstart();
void intret();

// Halt the system
void halt_system();

// Set and retrieve interrupt vectors
void set_irq_handler(unsigned char n, IRQ_HANDLER);
IRQ_HANDLER get_irq_handler(unsigned char n);

// Hardware-related
void sound(unsigned int frequency);
const char * get_cpu_vendor();
int get_tick_count();

#endif