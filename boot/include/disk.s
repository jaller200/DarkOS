%ifndef __DISK_S_INCLUDED__
%define __DISK_S_INCLUDED__

; This is where we will define our disk functions
%include "boot/include/bpb.s"

; LBACHS()
LBACHS:
    xor     dx, dx

    div     word [SectorsPerTrack]
    inc     dl
    mov     byte [AbsoluteSector], dl

    xor     dx, dx
    div     word [NumberOfHeads]
    mov     byte [AbsoluteHead], dl
    mov     byte [AbsoluteTrack], al

    ret

; ClusterLBA()
ClusterLBA:
    sub     ax, 0x0002
    xor     cx, cx
    mov     cl, byte [SectorsPerCluster]
    mul     cx
    add     ax, word [DataSector]

    ret

; ReadSectors()
ReadSectors:
    .main:
        mov     di, 5

    .read:
        push    ax
        push    bx
        push    cx

        call    LBACHS

        mov     ah, 0x02
        mov     al, 0x01
        mov     ch, byte [AbsoluteTrack]
        mov     cl, byte [AbsoluteSector]
        mov     dh, byte [AbsoluteHead]
        mov     dl, byte [DriveNumber]

        int     0x13
        jnc     .success

        xor     ax, ax
        int     0x13

        dec     di
        pop     cx
        pop     bx
        pop     ax

        jnz     .read
        xor     ax, ax
        int     0x16
        int     0x19

    .success:
        pop     cx
        pop     bx
        pop     ax

        add     bx, word [BytesPerSector]
        inc     ax
        loop    .main

        ret

AbsoluteSector:     db  0x00
AbsoluteTrack:      db  0x00
AbsoluteHead:       db  0x00

DataSector:         dw  0x0000
Cluster:            dw  0x0000

%endif