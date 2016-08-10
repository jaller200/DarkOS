// This is a class that stores our VGA Driver functions

// Includes
#include "vga.h"

#include "../common/string.h"

// Local Constants
unsigned char _CurX = 0;    // The current X Position
unsigned char _CurY = 0;    // The current Y Position

unsigned char COLOR = 15;   // Default - White

void PrintChar(unsigned char c) {
    unsigned char *vga = (unsigned char *)VIDMEM + (_CurX * 2) + (_CurY * (COLS * 2));
    
    if (_CurY >= ROWS) {
        ScrollUp();
    }
    
    if (c == '\n' || c == '\r') {   // Start new line
        _CurX = 0;
        _CurY++;
        return;
    }
    
    *vga++ = c;
    *vga = COLOR;
    
    _CurX += 1;
    if (_CurX >= COLS) {
        _CurX = 0;
        _CurY++;
    }
    
    MoveCursor(_CurX, _CurY);
}

void NewLine() {
    PrintChar("\n");
}

void ScrollUp() {
    unsigned char *vga = (unsigned char *)VIDMEM;
    
    char i = 0;
    while (i < ROWS) {
        unsigned char *newPos = vga + ((0 * 2) + (i * COLS * 2));
        unsigned char *oldPos = vga + ((0 * 2) + ((i + 1) * (COLS * 2)));
        
        memcpy(newPos, oldPos, COLS * 2);
        i++;
    }
    
    _CurY--;
    MoveCursor(_CurX, _CurY);
}

void PrintString(unsigned char *str) {
    if (!str)
        return;
    
    int i = 0;
    while (str[i] != 0x00) {
        PrintChar(str[i++]);
    }
    
    MoveCursor(_CurX, _CurY);
}

void PrintHex(int decimal) {
    int quotient, remainder;
    int i, j = 0;
    
    char hexadecimalnum[100];
    
    quotient = decimal;
    
    while(quotient != 0) {
        remainder = quotient % 16;
        if (remainder < 10)
            hexadecimalnum[j++] = 48 + remainder;
        else
            hexadecimalnum[j++] = 55 + remainder;
        quotient = quotient / 16;
    }
    
    i = j;
    while (i >= 0) {
        PrintChar(hexadecimalnum[i]);
        
        i--;
    }
}

void MoveCursor(unsigned char x, unsigned char y) {
    unsigned short position = x + (y * COLS);
    
    port_byte_out(0x03D4, 0x0F);
    port_byte_out(0x03D5, (unsigned char)(position));
    port_byte_out(0x03D4, 0x0E);
    port_byte_out(0x03D5, (unsigned char)(position >> 8));
}

void ClearScreen() {
    unsigned char *vga = (unsigned char *)VIDMEM;
    
    int i = 0;
    while (i < (COLS*2) * (ROWS*2)) {
        vga[i] = ' ';
        vga[i+1] = COLOR;
        i += 2;
    }
    
    _CurX = 0; _CurY = 0;
    MoveCursor(0, 0);
}

void GoToXY(char x, char y) {
    _CurX = x;
    _CurY = y;
    MoveCursor(x, y);
}

void SetColor(unsigned char newColor) {
    COLOR = newColor;
}

// itoa / itoa_s functions
// These are used to convert integers to chars / strings

char tbuf[32];
char bchars[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

void itoa(unsigned int i, unsigned int base, unsigned char* buf) {
    int pos = 0;
    int opos = 0;
    int top = 0;
    
    if (i == 0 || base > 16) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    
    while (i != 0) {
        tbuf[pos] = bchars[i % base];
        pos++;
        i /= base;
    }
    
    top = pos--;
    
    opos = 0;
    while (opos < top) {
        buf[opos] = tbuf[pos];
        
        pos--;
        opos++;
    }
    
    buf[opos] = 0;
}

void itoa_s(int i, unsigned int base, unsigned char * buf) {
    if (base > 16) return;
    if (i < 0) {
        *buf++ = '-';
        i *= -1;
    }
    itoa(i, base, buf);
}

// Printf function
int Printf(const char *str, ...) {
    // If string is null, return
    if (!str)
        return;
    
    va_list args;       // Our arguments
    va_start(args, str);
    
    size_t i = 0;
    while (i < strlen(str)) {
        switch (str[i]) {
            case '%':
                switch (str[i+1]) {
                    case 'c': {
                        unsigned char c = va_arg(args, char);
                        PrintChar(c);
                        i++;
                        break;
                    }
                        
                    case 's': {
                        int c = ((int)va_arg(args, char));
                        char str[64];
                        strcpy(str, (const char *)c);
                        PrintString(str);
                        i++;
                        break;
                    }
                        
                    case 'd':
                    case 'i': {
                        int c = va_arg(args, int);
                        unsigned char str[32] = {0};
                        itoa_s(c, 10, str);
                        PrintString(str);
                        i++;
                        break;
                    }
                        
                    case 'X':
                    case 'x': {
                        int c = va_arg(args, int);
                        unsigned char str[32] = {0};
                        itoa_s(c, 16, str);
                        PrintString(str);
                        i++;
                        break;
                    }
                        
                    default:
                        va_end(args);
                        return 1;
                }
                
                break;
                
            default:
                PrintChar(str[i]);
                break;
        }
        i++;
    }
    
    va_end(args);
    return i;
}