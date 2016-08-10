%ifndef __GDT_S_INCLUDE__
%define __GDT_S_INCLUDE__

%define NULL_DESC 0x00
%define CODE_DESC 0x08
%define DATA_DESC 0x10

; LoadGDT()
LoadGDT:
    cli
    pusha
    lgdt    [gdt]
    sti
    popa
    ret

; Start of our Global Descriptor Table
gdt_start:

gdt_null:
    dd      0x00000000
    dd      0x00000000

gdt_code:
    dw      0xFFFF      ; limit low
	dw      0           ; base low
	db      0           ; base middle
	db      10011010b   ; access
	db      11001111b   ; granularity
	db      0           ; base high

gdt_data:
    dw      0xFFFF      ; limit low (Same as code)10:56 AM 7/8/2007
	dw      0           ; base low
	db      0           ; base middle
	db      10010010b   ; access
	db      11001111b   ; granularity
	db      0           ; base high

gdt_end:

gdt:
    dw      gdt_end - gdt_start - 1
    dd      gdt_start

%endif