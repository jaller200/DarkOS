#ifndef __PIC_H_INCLUDED__
#define __PIC_H_INCLUDED__

#include "../common/common.h"
#include "../common/ports.h"

/*** Programmable Interrupt Controller Definitions ***/
#define IRQ_0       0x20    // IRQs 0-7 mapped to Interrupts 0x20-0x27 (32-39)
#define IRQ_8       0x28    // IRQs 8-15 mapped to Interrupts 0x28-0x2F (40-47)

// The following devices use PIC 1 to generate interrupts
#define PIC_IRQ_TIMER       0   // IRQ0
#define PIC_IRQ_KEYBOARD    1   // IRQ1
#define PIC_IRQ_CASCADE     2   // IRQ2
#define PIC_IRQ_SERIAL_1    3   // IRQ3
#define PIC_IRQ_SERIAL_2    4   // IRQ4
#define PIC_IRQ_PARALLEL_2  5   // IRQ5
#define PIC_IRQ_DISKETTE    6   // IRQ6
#define PIC_IRQ_PARALLEL_1  7   // IRQ7

// The following devices use PIC 2 to generate interrupts
#define PIC_IRQ_CMOS        0   // IRQ8
#define PIC_IRQ_CGA         1   // IRQ9
#define PIC_IRQ_AUXILIARY   4   // IRQ12
#define PIC_IRQ_FPU         5   // IRQ13
#define PIC_IRQ_HARD_DISK   6   // IRQ14

// ICW 4 Masks
#define PIC_OCW2_MASK_L1    0x01    // 00000001
#define PIC_OCW2_MASK_L2    0x02    // 00000010
#define PIC_OCW2_MASK_L3    0x04    // 00000100
#define PIC_OCW2_MASK_EOI   0x20    // 00100000
#define PIC_OCW2_MASK_SL    0x40    // 01000000
#define PIC_OCW2_MASK_R     0x80    // 10000000

/*** Functions ***/
void initialize_pic(unsigned char base0, unsigned char base1);

unsigned char pic_read_data(unsigned char picNum);

void pic_send_data(unsigned char data, unsigned char picNum);
void pic_send_command(unsigned char command, unsigned char picNum);

unsigned char pic_read_data(unsigned char picNum);
unsigned char pic_read_status(unsigned char picNum);

// void pic_mask_irq(unsigned char irqMask, unsigned char picNum);


#endif