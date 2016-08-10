#include "idt_handlers.h"

#include "hal.h"

// Initialization
void setup_idt_handlers() {
    set_irq_handler(0, divide_by_zero_handler);
    set_irq_handler(1, single_step_handler);
    set_irq_handler(2, non_maskable_interrupt_handler);
    set_irq_handler(3, breakpoint_handler);
    set_irq_handler(4, overflow_handler);
    set_irq_handler(5, bounds_check_handler);
    set_irq_handler(6, undefined_opcode_handler);
    set_irq_handler(7, no_coprocessor_handler);
    set_irq_handler(8, double_fault_handler);
    set_irq_handler(9, coprocessor_segment_overrun_handler);
    set_irq_handler(10, invalid_tss_handler);
    set_irq_handler(11, segment_not_present_handler);
    set_irq_handler(12, stack_segment_overrun_handler);
    set_irq_handler(13, general_protection_fault_handler);
    set_irq_handler(14, page_fault_handler);
    set_irq_handler(15, unassigned_handler);
    set_irq_handler(16, coprocessor_handler);
    set_irq_handler(17, alignment_check_handler);
    set_irq_handler(18, machine_check_handler);
    
    set_irq_handler(32, timer_handler);
    set_irq_handler(33, keyboard_handler);
    set_irq_handler(34, cascade_8259a_handler);
    set_irq_handler(35, serial_port_2_handler);
    set_irq_handler(36, serial_port_1_handler);
    set_irq_handler(37, parallel_port_2_handler);
    set_irq_handler(38, diskette_handler);
    set_irq_handler(39, parallel_port_1_handler);
    set_irq_handler(40, real_time_clock_handler);
    set_irq_handler(41, cga_vertical_retrace_handler);
    set_irq_handler(42, reserved_1_handler);
    set_irq_handler(43, reserved_2_handler);
    set_irq_handler(44, auxiliary_device_handler);
    set_irq_handler(45, fpu_handler);
    set_irq_handler(46, hard_disk_controller_handler);
    set_irq_handler(47, reserved_3_handler);
}

// Kernel Panic (until we add more)
static void kernel_panic() {
    //asm("cli");
    //asm("hlt");
    for (;;);   // Loop forever
}

// INT 0
static void divide_by_zero_handler() {
    asm("addl $12, %esp");
    asm("pushal");
    
    PrintString("[ERROR] Exception thrown: Illegal Division by 0\n");
    
    asm("popal");
    asm("iretl");
}

// INT 1
static void single_step_handler() {
    asm("addl $12, %esp");
    asm("pushal");
    
    PrintString("[ERROR] Exception thrown: Debugger Single Step\n");
    
    asm("popal");
    asm("iret");
}

// INT 2
static void non_maskable_interrupt_handler() {
    asm("addl $12, %esp");
    asm("pushal");
    
    PrintString("[ERROR] Exception thrown: Non Maskable Interrupt\n");
    
    asm("popal");
    asm("iret");
}

// INT 3
static void breakpoint_handler() {
    asm("addl $12, %esp");
    asm("pushal");
    
    PrintString("[ERROR] Exception thrown: Debugger Breakpoint\n");
    
    asm("popal");
    asm("iret");
}

static void overflow_handler() {
    asm("addl $12, %esp");
    asm("pushal");
    
    PrintString("[ERROR] Exception thrown: Overflow\n");
    
    asm("popal");
    asm("iret");
}

static void bounds_check_handler() {
    asm("addl $12, %esp");
    asm("pushal");
    
    PrintString("[ERROR] Exception thrown: Bounds Check\n");
    
    asm("popal");
    asm("iret");
}

static void undefined_opcode_handler() {
    asm("addl $12, %esp");
    asm("pushal");
    
    PrintString("[ERROR] Exception thrown: Undefined Opcode\n");
    
    asm("popal");
    asm("iret");
}

static void no_coprocessor_handler() {
    asm("addl $12, %esp");
    asm("pushal");
    
    PrintString("[ERROR] Exception thrown: No Coprocessor\n");
    
    asm("popal");
    asm("iret");
}

static void double_fault_handler() {
    asm("addl $16, %esp");
    asm("pushal");
    
    PrintString("[ERROR] Exception thrown: Double Fault\n");
    kernel_panic();
}

static void coprocessor_segment_overrun_handler() {
    asm("addl $12, %esp");
    asm("pushal");
    
    PrintString("[ERROR] Exception thrown: Coprocessor Segment Overrun\n");
    kernel_panic();
}

static void invalid_tss_handler() {
    asm("addl $16, %esp");
    asm("pushal");
    
    PrintString("[ERROR] Exception thrown: Invalid TSS\n");
    
    asm("popal");
    asm("iret");
}

static void segment_not_present_handler() {
    asm("addl $16, %esp");
    asm("pushal");
    
    PrintString("[ERROR] Exception thrown: Segment Not Present\n");
    
    asm("popal");
    asm("iret");
}

static void stack_segment_overrun_handler() {
    asm("addl $16, %esp");
    asm("pushal");
    
    PrintString("[ERROR] Exception thrown: Stack Segment Overrun\n");
    
    asm("popal");
    asm("iret");
}

static void general_protection_fault_handler() {
    asm("addl $0x10, %esp");
    //asm("pushal");
    
    PrintString("[ERROR] Exception thrown: General Protection Fault\n");
        
    int errorCode;
    int eip;
    int cs;
    int eflags;
    asm("movl 4(%%esp), %0" : "=r"(errorCode));
    Printf("GPF Error Code: %i\n", errorCode);
    
    asm("movl 8(%%esp), %0" : "=r"(eip));
    Printf("GPF EIP = %i\n", eip);
    
    asm("movl 12(%%esp), %0" : "=r"(cs));
    Printf("GPF CS = %i\n", (int)cs);
    
    asm("movl 16(%%esp), %0" : "=r"(eflags));
    Printf("GPF EFLAGS = %i\n", eflags);
    
    asm("hlt");
    //for (;;);
    //kernel_panic();
}

static void page_fault_handler() {
    asm("addl $16, %esp");
    asm("pushal");
    
    PrintString("[ERROR] Exception thrown: Page Fault\n");
    
    asm("popal");
    asm("iret");
}

static void unassigned_handler() {
    asm("addl $12, %esp");
    asm("pushal");
    
    PrintString("[ERROR] Exception thrown: Unassigned\n");
    
    asm("popal");
    asm("iret");
}

static void coprocessor_handler() {
    asm("addl $12, %esp");
    asm("pushal");
    
    PrintString("[ERROR] Exception thrown: Coprocessor Error\n");
    
    asm("popal");
    asm("iret");
}

static void alignment_check_handler() {
    asm("addl $16, %esp");
    asm("pushal");
    
    PrintString("[ERROR] Exception thrown: Alignment Check\n");
    
    asm("popal");
    asm("iret");
}

static void machine_check_handler() {
    asm("addl $12, %esp");
    asm("pushal");
    
    PrintString("[ERROR] Exception thrown: Machine Check\n");
    kernel_panic();
}

// TODO: Add remaining Software Interrupts

// TODO: Add Hardware Interrupts
static void timer_handler() {
    asm("addl $12, %esp");
    //asm("pushal");
    
    PrintString("[OS] Timer Interrupt\n");
    
    int eip;
    int cs;
    int eflags;
    
    asm("movl 4(%%esp), %0" : "=r"(eip));
    asm("movl 8(%%esp), %0" : "=r"(cs));
    asm("movl 12(%%esp), %0" : "=r"(eflags));
    Printf("Timer EIP = %i\n", eip);
    Printf("Timer CS = %i\n", cs);
    Printf("Timer EFLAGS = %i\n", eflags);
    
    interrupt_done(0);
    
    //asm("popal");
    asm("iretl");
}

static void keyboard_handler() {
    asm("addl $12, %esp");
    asm("pushal");
    
    PrintString("[OS] Keyboard Interrupt\n");
    
    interrupt_done(0);
    asm("popal");
    asm("iret");
}

static void cascade_8259a_handler() {
    asm("addl $12, %esp");
    asm("pushal");
    
    PrintString("[OS] Cascade for 8259A Slave Interrupt\n");
    
    interrupt_done(0);
    asm("popal");
    asm("iret");
}

static void serial_port_2_handler() {
    asm("addl $12, %esp");
    asm("pushal");
    
    PrintString("[OS] Serial Port 2 Interrupt\n");
    
    interrupt_done(0);
    asm("popal");
    asm("iret");
}

static void serial_port_1_handler() {
    asm("addl $12, %esp");
    asm("pushal");
    
    PrintString("[OS] Serial Port 1 Interrupt\n");
    
    interrupt_done(0);
    asm("popal");
    asm("iret");
}

static void parallel_port_2_handler() {
    asm("addl $12, %esp");
    asm("pushal");
    
    PrintString("[OS] Parallel Port 2 Interrupt\n");
    
    interrupt_done(0);
    asm("popal");
    asm("iret");
}

static void diskette_handler() {
    asm("addl $12, %esp");
    asm("pushal");
    
    PrintString("[OS] Diskette Drive Interrupt\n");
    
    interrupt_done(0);
    asm("popal");
    asm("iret");
}

static void parallel_port_1_handler() {
    asm("addl $12, %esp");
    asm("pushal");
    
    PrintString("[OS] Parallel Port 1 Interrupt\n");
    
    interrupt_done(0);
    asm("popal");
    asm("iret");
}

static void real_time_clock_handler() {
    asm("addl $12, %esp");
    asm("pushal");
    
    PrintString("[OS] Real Time Clock Interrupt\n");
    
    interrupt_done(1);
    asm("popal");
    asm("iret");
}

static void cga_vertical_retrace_handler() {
    asm("addl $12, %esp");
    asm("pushal");
    
    PrintString("[OS] CGA Vertical Retrace Interrupt\n");
    
    interrupt_done(1);
    asm("popal");
    asm("iret");
}

static void reserved_1_handler() {
    asm("addl $12, %esp");
    asm("pushal");
    
    PrintString("[OS] Reserved Interrupt (IRQ10 / IRQ2)");
    
    interrupt_done(1);
    asm("popal");
    asm("iret");
}

static void reserved_2_handler() {
    asm("addl $12, %esp");
    asm("pushal");
    
    PrintString("[OS] Reserved Interrupt (IRQ11 / IRQ3)");
    
    interrupt_done(1);
    asm("popal");
    asm("iret");
}

static void auxiliary_device_handler() {
    asm("addl $12, %esp");
    asm("pushal");
    
    PrintString("[OS] Auxiliary Device Interrupt\n");
    
    interrupt_done(1);
    asm("popal");
    asm("iret");
}

static void fpu_handler() {
    asm("addl $12, %esp");
    asm("pushal");
    
    PrintString("[OS] FPU Interrupt\n");
    
    interrupt_done(1);
    asm("popal");
    asm("iret");
}

static void hard_disk_controller_handler() {
    asm("addl $12, %esp");
    asm("pushal");
    
    PrintString("[OS] Hard Disk Controller Handler");
    
    interrupt_done(1);
    asm("popal");
    asm("iret");
}

static void reserved_3_handler() {
    asm("addl $12, %esp");
    asm("pushal");
    
    PrintString("[OS] Reserved Interrupt (IRQ15 / IRQ7)");
    
    interrupt_done(1);
    asm("popal");
    asm("iret");
}