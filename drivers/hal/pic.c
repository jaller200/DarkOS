#include "pic.h"

/*** Initialization Control Word Masks ***/

#define PIC_1_REG_COMMAND   0x20    // PIC 1 Command Register Port (write-only)
#define PIC_1_REG_STATUS    0x20    // PIC 1 Status Register Port (read-only)
#define PIC_1_REG_DATA      0x21    // PIC 1 Data Register Port
#define PIC_1_REG_IMR       0x21    // PIC 1 IMR Register Port

#define PIC_2_REG_COMMAND   0xA0    // PIC 2 Command Register Port (write-only)
#define PIC_2_REG_STATUS    0xA0    // PIC 2 Status Register Port (read-only)
#define PIC_2_REG_DATA      0xA1    // PIC 2 Data Register Port
#define PIC_2_REG_IMR       0xA1    // PIC 2 IMR Register Port

// Masks

// ICW 1
#define PIC_ICW1_MASK_IC4   0x01    // 00000001 (Set to send ICW 4)
#define PIC_ICW1_MASK_SNGL  0x02    // 00000010 (Set if there is only 1 PIC)
#define PIC_ICW1_MASK_ADI   0x04    // 00000100 (Set if call address interval is 4, else 8)
#define PIC_ICW1_MASK_LTIM  0x08    // 00001000 (Set if operating in Level Triggered Mode)
#define PIC_ICW1_MASK_INIT  0x10    // 00010000 (Set to initialize PIC)

// ICW 4
#define PIC_ICW4_MASK_UPM   0x01    // 00000001 (Set if in 80x86 mode)
#define PIC_ICW4_MASK_AEOI  0x02    // 00000010 (Set if auto EOI)
#define PIC_ICW4_MASK_MS    0x04    // 00000100 (Set if BUF is set)
#define PIC_ICW4_MASK_BUF   0x08    // 00001000 (Set if buffer mode)
#define PIC_ICW4_MASK_SFNM  0x10    // 00010000 (Set if fully nested mode)

// ICW Bits

// ICW 1
#define PIC_ICW1_IC4_EXPECT 0x01    // 00000001
#define PIC_ICW1_IC4_NO     0x00    // 00000000
#define PIC_ICW1_SNGL_ONE   0x02    // 00000010
#define PIC_ICW1_SGNL_TWO   0x00    // 00000000
#define PIC_ICW1_ADI_4      0x04    // 00000100
#define PIC_ICW1_ADI_8      0x00    // 00000000
#define PIC_ICW1_LTIM_LVL   0x08    // 00001000
#define PIC_ICW1_LTIM_EDG   0x00    // 00000000
#define PIC_ICW1_INIT_YES   0x10    // 00010000
#define PIC_ICW1_INIT_NO    0x00    // 00000000

// ICW 4
#define PIC_ICW4_UPM_8086   0x01    // 00000001
#define PIC_ICW4_UPM_MCS    0x00    // 00000000
#define PIC_ICW4_AEOI_ON    0x02    // 00000010
#define PIC_ICW4_AEOT_OFF   0x00    // 00000000
#define PIC_ICW4_MS_SET     0x04    // 00000100
#define PIC_ICW4_MS_NOSET   0x00    // 00000000
#define PIC_ICW4_BUF_SET    0x08    // 00001000
#define PIC_ICW4_BUF_NOSET  0x00    // 00000000
#define PIC_ICW4_SFNM_ON    0x10    // 00010000
#define PIC_ICW4_SFNM_OFF   0x00    // 00000000


/*** End ***/

/*** Functions ***/

void initialize_pic(unsigned char base0, unsigned char base1) {
    // Send ICW 1 to both PICs
    unsigned char icw = 0;
    
    disable();
    
    icw = (icw & ~PIC_ICW1_MASK_INIT) | PIC_ICW1_INIT_YES;  // Send initialization bit
    icw = (icw & ~PIC_ICW1_MASK_IC4) | PIC_ICW1_IC4_EXPECT; // Expect ICW 4
    
    pic_send_command(icw, 0);
    pic_send_command(icw, 1);
    
    // Send ICW 2 to both PICs
    pic_send_data(base0, 0);
    pic_send_data(base1, 1);
    
    // Send ICW 3 to both PICs
    pic_send_data(0x04, 0);
    pic_send_data(0x02, 1);
    
    // Send ICW 4 to both PICs
    // icw = 0;
    icw = (icw & ~PIC_ICW4_MASK_UPM) | PIC_ICW4_UPM_8086;
    
    pic_send_data(icw, 0);
    pic_send_data(icw, 1);
}

// Send data to PIC
void pic_send_data(unsigned char data, unsigned char picNum) {
    // 80x86 family has 2 PICs
    if (picNum > 1)
        return;
    
    unsigned char reg = (picNum == 1) ? PIC_2_REG_DATA : PIC_1_REG_DATA;    // PIC Data Register
    port_byte_out(reg, data);       // Write the byte to the data register
}

// Read data from PIC
unsigned char pic_read_data(unsigned char picNum) {
    if (picNum > 1)
        return;
    
    unsigned char reg = (picNum == 1) ? PIC_2_REG_DATA : PIC_1_REG_DATA;
    return port_byte_in(reg);   // Read the byte from the data register
}

// Send a command to the PIC
void pic_send_command(unsigned char command, unsigned char picNum) {
    if (picNum > 1)
        return;
    
    unsigned char reg = (picNum == 1) ? PIC_2_REG_COMMAND : PIC_1_REG_COMMAND;
    port_byte_out(reg, command);    // Port out the command to the control register
}

// Read the status register of the PIC
unsigned char pic_read_status(unsigned char picNum) {
    if (picNum > 1)
        return;
    
    unsigned char reg = (picNum == 1) ? PIC_2_REG_STATUS : PIC_1_REG_STATUS;
    return port_byte_in(reg);   // Read the byte from the status register
}