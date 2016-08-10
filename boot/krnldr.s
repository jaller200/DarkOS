BITS    16
ORG     0x500

jmp     main

; Includes
%include "boot/include/gdt.s"
%include "boot/include/a20.s"
%include "boot/include/fat12.s"
%include "boot/include/common.s"

main:
    cli
    mov     ax, 0x0000
    mov     ds, ax
    mov     es, ax
    mov     ax, 0x0000
    mov     ss, ax
    mov     sp, 0xFFFF
    sti

    mov     si, MSG_BOOTED
    call    Print

    ; First, what we want to do is load our Global Descriptor Table
    call    LoadGDT
    mov     si, MSG_GDT_LOADED
    call    Print

    ; Second, set the A20 gate to allow us to use up to 4GB of memory
    call    EnableA20
    mov     si, MSG_A20_ENABLED
    call    Print

    ; Load the FAT and try to load our kernel into memory
    call    LoadRoot

    mov     ebx, 0x00000000     ; BX:BP points to buffer to load to
    mov     bp, IMAGE_RMODE_BASE
    mov     si, KernelName

    call    LoadFile            ; Search for the file

    mov     dword [KernelSize], ecx     ; Store the size of the Kernel for later (when copying)
    cmp     ax, 0                       ; Check for success
    je      EnterKernel

    mov     si, MSG_FAIL
    call    Print
    xor     ax, ax
    int     0x16
    int     0x19

EnterKernel:
    mov     si, MSG_KERNEL_LOADED
    call    Print

    mov     si, MSG_JUMPING
    call    Print

    ; Now we can enter Protected Mode by setting Bit 0 of CR0 register
    mov     eax, cr0
    or      eax, 1
    mov     cr0, eax

    ; Now jump to our 32-Bit code
    jmp     CODE_DESC:KernelStage

    cli
    hlt

; Print()
Print:
    lodsb
    cmp     al, 0x00
    je      .done

    mov     ah, 0x0E
    int     0x10
    jmp     Print

    .done:
        ret

MSG_BOOTED:         db  "[OS] Loading DarkOS...",10,13,0
MSG_GDT_LOADED:     db  "[OS] Loaded GDT",10,13,0
MSG_A20_ENABLED:    db  "[OS] A20 Gate Enabled",10,13,0
MSG_KERNEL_LOADED:  db  "[OS] Kernel Loaded",10,13,0
MSG_JUMPING:        db  "[OS] Entering 32-Bit Kernel...",10,13,0

MSG_FAIL:           db  "[OS] Unable to locate KERNEL.BIN... Press any key to quit",0
MSG_TEST:           db  "Test",10,13,0

; ****************
; 32-Bit Code
; ****************

BITS    32

KernelStage:
    cli
    mov     ax, DATA_DESC
    mov     ds, ax
    mov     ss, ax
    mov     es, ax
    mov     esp, 0x90000

; Copy the kernel to 1MB (0x100000)
CopyImage:
    mov     eax, dword [KernelSize]
    movzx   ebx, word [BytesPerSector]
    mul     ebx         ; EAX = ImageSize * BytesPerSector (total bytes)
    mov     ebx, 4
    div     ebx         ; EAX = (ImageSize * BytesPerSector) / 4

    cld
    mov     esi, IMAGE_RMODE_BASE
    mov     edi, IMAGE_PMODE_BASE
    mov     ecx, eax
    rep     movsd

    call    CODE_DESC:IMAGE_PMODE_BASE

    cli
    hlt