#ifndef __VGA_H_INCLUDED__
#define __VGA_H_INCLUDED__

// Includes
#include "../common/ports.h"
#include "../common/arguments.h"

// Definitions
#define VIDMEM  0xB8000
#define COLS    80
#define ROWS    25

// Functions
void PrintChar(unsigned char c);
void NewLine();
void PrintString(unsigned char* str);
void PrintHex(int decimal);

void ScrollUp();

void MoveCursor(unsigned char x, unsigned char y);
void ClearScreen();
void MoveToXY(char x, char y);

void SetColor(unsigned char newColor);

int Printf(const char *str, ...);

#endif