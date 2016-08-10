%ifndef __VGA_S_INCLUDED__
%define __VGA_S_INCLUDED__

BITS    32

; This is our VGA code

; Constants
%define VIDMEM  0xB8000
%define COLS    80
%define ROWS    25
%define CHAR_ATTRIB 15

_CurX   db  0
_CurY   db  0

; PrintChar() - Print a 32-bit character to the screen
; IN:
;   BL => Char to prints
PrintChar:
    pusha
    mov     edi, VIDMEM

    ; Get current position
    xor     eax, eax

    ; Remember: currentPos = x + y * COLS; x and y are in _CurX and _CurY
    ; Becauase there are 2 bytes per character, COLS=number of characters in line,
    ; so we have to multiply COLS * 2 to get number of bytes per line
    mov     ecx, COLS*2         ; ECX = COLS * 2 (160 bytes)
    mov     al, byte [_CurY]    ; Move _CurY into AL
    mul     ecx                 ; EAX = _CurY * (COLS * 2)
    push    eax                 ; Push this to the stack

    mov     al, byte [_CurX]    ; Move _CurX into AL
    mov     cl, 2               ; Move 2 into CL
    mul     cl                  ; EAX = _CurX * 2
    pop     ecx                 ; Pop (_CurY * (COLS * 2)) into ECX
    add     eax, ecx            ; EAX = (_CurX * 2) + (_CurY * (COLS * 2))

    xor     ecx, ecx            ; Clear ECX
    add     edi, eax            ; EDI = 0xB8000 + EAX

    cmp     bl, 0x0A            ; Check if BL if 0x0A (10 in decimal - new line)
    je      .row                ; If so, go to a new line

    mov     dl, bl              ; Move the byte from BL into DL
    mov     dh, CHAR_ATTRIB     ; Move CHAR_ATTRIB into DH
    mov     word [edi], dx      ; Now move these values into the word at DI

    inc     byte [_CurX]        ; Now increment _CurX
    cmp     byte [_CurX], COLS  ; Check if _CurX == COLS
    je      .row                ; If so, go to a new line
    jmp     .done               ; Else, we are done

    .row:
        mov     byte [_CurX], 0 ; Update _CurX == 0
        inc     byte [_CurY]    ; Increment _CurY

    .done:
        mov     bh, byte [_CurY]    ; Now update the hardware cursor to _CurX and _CurY
        mov     bl, byte [_CurX]
        call    MoveCursor

        popa
        ret

; PrintString() - Print a 32-bit string
; IN:
;   EBX => String to print
PrintString:
    pusha
    push    ebx         ; Push EBX to stack (this is the address of our string)
    pop     edi         ; Pop this address into EDI

    .loop:
        mov     bl, byte [edi]      ; Load the first byte from EDI into BL
        cmp     bl, 0x00            ; Check if the byte is 0x00
        je      .done               ; If so, jump to .done

        call    PrintChar           ; Now call "PrintChar()"
        inc     edi                 ; Increment EDI
        jmp     .loop               ; And loop

    .done:
        mov     bh, byte [_CurY]    ; Now update the hardware cursor to _CurX and _CurY
        mov     bl, byte [_CurX]
        call    MoveCursor

        popa
        ret

; MoveCursor() - Mov the hardware cursor
; IN:
;   BH => Y Pos
;   BL => X Pos
MoveCursor:
    pusha

    xor     eax, eax
    mov     ecx, COLS
    mov     al, bh
    mul     ecx
    add     al, bl
    mov     ebx, eax

    mov     al, 0x0F
    mov     dx, 0x03D4
    out     dx, al

    mov     al, bl
    mov     dx, 0x03D5
    out     dx, al

    xor     eax, eax

    mov     al, 0x0E
    mov     dx, 0x03D4
    out     dx, al

    mov     al, bh
    mov     dx, 0x03D5
    out     dx, al

    popa
    ret

; ClearScreen() - Clear the screen
ClearScreen:
    pusha

    cld
    mov     edi, VIDMEM
    mov     cx, 2000
    mov     ah, CHAR_ATTRIB
    mov     al, ' '
    rep     stosw

    mov     byte [_CurX], 0
    mov     byte [_CurY], 0
    popa
    ret

; GoToXY() - Go to XY coordinates
; IN:
;   AH => Y Coordinate
;   AL => X Coordinate
BITS    32

GoToXY:
    pusha
    mov     [_CurX], al
    mov     [_CurY], ah

    popa
    ret

%endif