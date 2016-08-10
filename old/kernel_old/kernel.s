; This is our makeshift kernel until we can program it in C
BITS    32
ORG     0x100000

jmp     main

%include "kernel/include/vga.s"

; Strings
MSG_WELCOME:    db  "[OS] Welcome to DarkOS",10,0
MSG_LOADING:    db  "[OS] Loading OS...",10,0

main:
    mov     ax, 0x10
    mov     ds, ax
    mov     ss, ax
    mov     es, ax
    mov     esp, 0x90000

    call    ClearScreen

    mov     ebx, MSG_WELCOME
    call    PrintString

    mov     ebx, MSG_LOADING
    call    PrintString

    cli
    hlt