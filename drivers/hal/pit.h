#ifndef __PIT_H_INCLUDED__
#define __PIT_H_INCLUDED__

#include "../common/common.h"

/*** Constants ***/
#define PIT_OCW_MASK_BINCOUNT   0x01    // 00000001 (Binary Counter Mask)
#define PIT_OCW_MASK_MODE       0x0E    // 00001110 (Operating Mode)
#define PIT_OCW_MASK_RL         0x30    // 00110000 (Read/Load Mode)
#define PIT_OCW_MASK_COUNTER    0xC0    // 11000000 (Select Counter)

// OCW Bits
#define PIT_OCW_BCP_BINARY      0x00    // 00000000 (Binary)
#define PIT_OCW_BCP_BCD         0x01    // 00000001 (Binary Coded Decimal)
#define PIT_OCW_MODE_INTCOUNT   0x00    // 00000000 (Interrupt / Terminal Count)
#define PIT_OCW_MODE_ONESHOT    0x02    // 00000010 (Programmable One-Shot)
#define PIT_OCW_MODE_RATE       0x04    // 00000100 (Rate Generator)
#define PIT_OCW_MODE_WAVE       0x06    // 00000110 (Square Wave Generator)
#define PIT_OCW_MODE_SOFTWARE   0x08    // 00001000 (Software Triggered Strobe)
#define PIT_OCW_MODE_HARDWARE   0x0A    // 00001010 (Hardware Triggered Strobe)
#define PIT_OCW_RL_LATCH        0x00    // 00000000 (Counter Value Latched Internally)
#define PIT_OCW_RL_LSB          0x10    // 00010000 (Read / Load LSB only)
#define PIT_OCW_RL_MSB          0x20    // 00100000 (Read / Load MSB only)
#define PIT_OCW_RL_LSB_MSB      0x30    // 00110000 (Read / Load LSB then MSB)
#define PIT_OCW_COUNTER_0       0x00    // 00000000 (Counter 0)
#define PIT_OCW_COUNTER_1       0x40    // 01000000 (Counter 1)
#define PIT_OCW_COUNTER_2       0x80    // 10000000 (Counter 2)

/*** Functions ***/

// Initialization / Deinitialization
void initialize_pit();
bool is_pit_initialized();

// Sending Data / Commands
void pit_send_data(unsigned short data, unsigned char counter);
void pit_send_command(unsigned char command);

// Reading Data
unsigned char pit_read_data(unsigned short counter);

// Global Tick Count
unsigned int pit_set_tick_count(unsigned int tick);
unsigned int pit_get_tick_count();

// Counter
void pit_start_counter(unsigned int freq, unsigned char counter, unsigned char mode);

#endif