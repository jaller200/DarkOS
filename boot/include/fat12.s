%ifndef __FAT12_S_INCLUDED__
%define __FAT12_S_INCLUDED__

%include "boot/include/disk.s"

; Constants
%define ROOT_OFFSET 0x2E00  ; This is where our root offset is (0x0500:0x2E00)
%define FAT_SEG 0x2C0       ; This is the absolute path to our FAT in memory
%define ROOT_SEG 0x2E0      ; This is the absolute path to our Root Dir in memory

; LoadRoot()
LoadRoot:
    pusha
    push    es              ; Store ES on the stack

    ; Compute size of root dir
    xor     cx, cx
    xor     dx, dx
    mov     ax, 32
    mul     word [RootDirEntries]
    div     word [BytesPerSector]
    xchg    cx, ax

    ; Now get the start of our Root Dir
    mov     al, byte [NumberOfFATs]
    mul     word [SectorsPerFAT]
    add     ax, word [ReservedSectors]

    mov     word [DataSector], ax
    add     word [DataSector], cx

    ; Now read the sectors into memory
    push    word ROOT_SEG
    pop     es              ; ES == ROOT_SEG
    mov     bx, 0x0000
    call    ReadSectors
    pop     es
    popa
    ret

; LoadFAT()
; IN: 
;   ES:DI => Root Directory Table
LoadFAT:
    pusha
    push    es

    ; Compute size of FAT
    xor     ax, ax
    mov     al, byte [NumberOfFATs]
    mul     word [SectorsPerFAT]
    mov     cx, ax

    ; Now get the start of the FAT
    mov     ax, word [ReservedSectors]

    ; Now read the FAT into memory
    push    word FAT_SEG
    pop     es
    mov     bx, 0x0000
    call    ReadSectors
    pop     es
    popa
    ret

; FindFile() - Search to Root Dir for a file
; IN:
;   DS:SI => File Name
; OUT:
;   AX => File Index Number in Directory Table, -1 If Error
FindFile:
    push    cx
    push    dx
    push    bx
    mov     bx, si

    ; Browse root directory for binary image
    mov     cx, word [RootDirEntries]
    mov     di, ROOT_OFFSET
    cld

    .loop:
        push    cx
        mov     cx, 11
        mov     si, bx
        push    di
        rep     cmpsb
        pop     di

        je      .found

        pop     cx
        add     di, 32
        loop    .loop

    .not_found:
        pop     bx
        pop     dx
        pop     cx
        mov     ax, -1
        ret

    .found:
        pop     ax          ; This is CX (Entry Number)
        pop     bx
        pop     dx
        pop     cx
        ret

; LoadFile() - Load a file from the filesystem
; IN:
;   ES:SI => File to load
;   BX:BP => Location to load to
; OUT:
;   AX => -1 on Error, 0 on Success
;   CX => Number of sectors loaded
LoadFile:
    xor     ecx, ecx        ; Clear ECX
    push    ecx

    .find:
        push    bx          ; We will pop this into ES:BX later
        push    bp

        call    FindFile    ; Check if file exists first

        cmp     ax, -1      ; -1 = Error
        jne     .load_image_pre

        pop     bp
        pop     bx
        pop     ecx
        mov     ax, -1
        ret

    .load_image_pre:
        sub     edi, ROOT_OFFSET
        sub     eax, ROOT_OFFSET

        ; Get the starting cluster
        push    word ROOT_SEG
        pop     es
        mov     dx, word [es:di + 0x001A]
        mov     word [Cluster], dx

        pop     bx          ; Pop BP into BX
        pop     es          ; Pop BX into ES
        push    bx          ; Push these back on the stack (to restore BX:BP)
        push    es

        call    LoadFAT         ; Load the FAT into memory

    .load_image:
        mov     ax, word [Cluster]
        pop     es
        pop     bx
        call    ClusterLBA

        xor     cx, cx
        mov     cl, byte [SectorsPerCluster]
        call    ReadSectors

        pop     ecx
        inc     ecx
        push    ecx

        push    bx
        push    es

        mov     ax, FAT_SEG
        mov     es, ax
        xor     bx, bx

        ; Compute next cluster
        mov     ax, word [Cluster]
        mov     cx, ax
        mov     dx, ax
        shr     dx, 0x0001
        add     cx, dx

        mov     bx, 0x0000
        add     bx, cx
        mov     dx, word [es:bx]
        test    ax, 0x0001
        jnz     .odd

    .even:
        and     dx, 0000111111111111b
        jmp     .done

    .odd:
        shr     dx, 0x0004

    .done:
        mov     word [Cluster], dx
        cmp     dx, 0x0FF0
        jb      .load_image

Done:
    pop     es
    pop     bx
    pop     ecx
    xor     ax, ax
    ret

%endif