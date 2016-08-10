#ifndef __IDT_EXCEPTIONS_H_INCLUDED__
#define __IDT_EXCEPTIONS_H_INCLUDED__

#include "idt.h"

#include "../screen/vga.h"

/*** Exceptions for IVT Entries ***/

// Initialization
void setup_idt_handlers();

// Panic!
static void kernel_panic();

// Software Interrupts
static void divide_by_zero_handler();                  // INT 0
static void single_step_handler();                     // INT 1
static void non_maskable_interrupt_handler();          // INT 2
static void breakpoint_handler();                      // INT 3
static void overflow_handler();                        // INT 4
static void bounds_check_handler();                    // INT 5
static void undefined_opcode_handler();                // INT 6
static void no_coprocessor_handler();                  // INT 7
static void double_fault_handler();                    // INT 8
static void coprocessor_segment_overrun_handler();     // INT 9
static void invalid_tss_handler();                     // INT 10
static void segment_not_present_handler();             // INT 11
static void stack_segment_overrun_handler();           // INT 12
static void general_protection_fault_handler();        // INT 13
static void page_fault_handler();                      // INT 14
static void unassigned_handler();                      // INT 15
static void coprocessor_handler();                     // INT 16
static void alignment_check_handler();                 // INT 17
static void machine_check_handler();                   // INT 18

// Reserved Interrupts
static void reserved_handler();                        // INT 19 - INT 31

// Hardware Interrupts
static void timer_handler();                           // INT 32
static void keyboard_handler();
static void cascade_8259a_handler();
static void serial_port_2_handler();
static void serial_port_1_handler();
static void parallel_port_2_handler();
static void diskette_handler();
static void parallel_port_1_handler();
static void real_time_clock_handler();
static void cga_vertical_retrace_handler();
static void reserved_1_handler();
static void reserved_2_handler();
static void auxiliary_device_handler();
static void fpu_handler();
static void hard_disk_controller_handler();
static void reserved_3_handler();

#endif