; DarkOS Bootloader

BITS    16
ORG     0x0

start:  jmp     main

; OEM Parameter Block
OEMLabel:           db  "DarkOS  "  ; OEM Label
BytesPerSector:     dw  512         ; 512 bytes per sector
SectorsPerCluster:  db  1           ; 1 sector = 1 cluster
ReservedSectors:    dw  1           ; 1 Reserved Sector
NumberOfFATs:       db  2           ; 2 FATs
RootDirEntries:     dw  224         ; 224 total root directory entries
TotalSectors:       dw  2880        ; 2880 total sectors on the floppy
MediaDescriptor:    db  0xF0        ; 0xF0 = 3.5" Floppy
SectorsPerFAT:      dw  9           ; 9 Sectors per FAT
SectorsPerTrack:    dw  18          ; 18 Sectors per track
NumberOfHeads:      dw  2           ; 2 Heads (2 sides)
HiddenSectors:      dd  0           ; 0 Hidden Sectors
LargeSectors:       dd  0           ; 0 Large Sectors (reserved for FAT32)
DriveNumber:        db  0           ; Drive 0 (Floppy, 0x80 = HDD)
Flags:              db  0           ; Unused
BootSignature:      db  0x29        ; 0x29 = 41 = "4.1"
VolumeSerialNumber: dd  0xa0a1a2a3  ; Serial Number
VolumeLabel:        db  "DarkOS     "   ; Drive is called "DarkOS"
FileSystemType:     db  "FAT12   "      ; Label for FAT12 filesystem (does not MAKE it a FAT12 filesystem though)
; End OEM Parameter Block

; Print()
Print:
    lodsb               ; Load next byte from DS:SI
    or      al, al      ; Check for 0x00
    jz      .done       ; If AL == 0x00, string is done

    mov     ah, 0x0E    ; Teletype print function
    int     0x10
    jmp     Print

    .done:
        ret

AbsoluteSector      db  0   ; The Absolute Sector
AbsoluteTrack       db  0   ; The Absolute Track / Cylinder
AbsoluteHead        db  0   ; The Absolute Head

; ClusterLBA()
ClusterLBA:
    sub     ax, 0x0002                      ; (cluster - 2)
    xor     cx, cx
    mov     cl, byte [SectorsPerCluster]    ; CL == 1
    mul     cx                              ; (cluster - 2) * SectorsPerCluster
    add     ax, word [DataSector]           ; ((cluster - 2) * SectorsPerCluster) + DataSector
    ret

; LBACHS()
LBACHS:
    xor     dx, dx

    ; Sectors
    div     word [SectorsPerTrack]          ; AX == (LBA Address / SectorsPerTrack), DX == (LBA Address % SectorsPerTrack)
    inc     dl                              ; DL == (LBA Address % SectorsPerTrack) + 1
    mov     [AbsoluteSector], dl            ; Store this

    ; Head and Track
    xor     dx, dx
    div     word [NumberOfHeads]            ; AX == LBA Address / SectorsPerTrack / NumberOfHeads, DX == LBA Address / SectorsPerTrack % NumberOfHeads
    mov     [AbsoluteHead], dl              ; Store this
    mov     [AbsoluteTrack], al             ; Store this
    ret

; ReadSectors()
ReadSectors:
    .main:
        mov     di, 0x0005      ; 5 tries

    .loop:
        push    ax
        push    bx
        push    cx
        call    LBACHS          ; Convert LBA to CHS address
        mov     ah, 0x02        ; AH == 0x02, Read Sectors function
        mov     al, 0x01        ; Read 1 sector at a times
        mov     ch, byte [AbsoluteTrack]    ; CH == Track / Cylinder
        mov     cl, byte [AbsoluteSector]   ; CL == Sector
        mov     dh, byte [AbsoluteHead]     ; DH == Head
        mov     dl, byte [DriveNumber]      ; DL == Drive Number
        int     0x13            ; Read Sectors
        jnc     .success        ; If carry flag is not set, success!
        xor     ax, ax
        int     0x13            ; Reset the disk and try again
        dec     di              ; Decrease DI (number of tries)
        pop     cx
        pop     bx
        pop     ax
        jnz     .loop           ; Loop again until ES:DI == 0x00
        mov     si, MSG_ERROR
        call    Print
        int     0x18            ; Soft boot

    .success:
        pop     cx              ; Pop everything
        pop     bx
        pop     ax
        add     bx, word [BytesPerSector]   ; Add BytesPerSector (512) to ES:BX (read location)
        inc     ax                          ; Increment the starting sector
        loop    .main                       ; Loop until CX == 0x00
        ret

DataSector          db  0x0000

main:
    cli
    mov     ax, 0x07C0      ; AX == 0x07C0 (0x0000:0x7C00)
    mov     ds, ax          ; Point each segment register to AX
    mov     es, ax
    mov     fs, ax
    mov     gs, ax

    mov     ax, 0x0000      ; Stack segment starts at 0x0000
    mov     ss, ax
    mov     sp, 0xFFFF      ; Stack starts at 0x0000:0xFFFF and grows towards 0x0000:0x0000
    sti

    mov     byte [DriveNumber], dl  ; Save the Drive Number (stored in DL) for later

    mov     si, MSG_WELCOME         ; Print welcome message
    call    Print

; Load the Root Directory
LoadRoot:
    ; Get number of sectors in root dir
    xor     cx, cx                  ; Clear CX and DX
    xor     dx, dx
    mov     ax, 32                  ; 32 bytes in each Root Dir entry
    mul     word [RootDirEntries]   ; AX == 32 * RootDirEntries (32 * 224 == 7,168 bytes) - 0x1C00
    div     word [BytesPerSector]   ; AX == (32 * RootDirEntries) / BytesPerSector (7,168 bytes / 512 bytes/sector == 14 sectors) - 0x000E
    xchg    ax, cx                  ; AX == 0x0000, CX == 0x000E

    ; Now get the start of the Root Directory
    mov     al, byte [NumberOfFATs]     ; AL == NumberOfFATs (2) - 0x0002
    mul     word [SectorsPerFAT]        ; AX == NumberOfFATs * SectorsPerFAT (2 * 9 = 18 sectors) - 0x0012
    add     ax, word [ReservedSectors]  ; AX == (NumberOfFATs * SectorsPerFAT) + ReservedSectors (18 sectors + 1 sectors = sector 19) - 0x0013
    mov     word [DataSector], ax       ; [DataSector] == 0x0013 (sector 19)
    add     word [DataSector], cx       ; [DataSector] == 0x0021 (sector 33)

    ; Now read the sectors into memory at offset 0x0200
    mov     bx, 0x0200              ; ES == 0x07C0, BX == 0x0200
    call    ReadSectors             ; Read the sectors

; Now search the root directory for image file
SearchRootDir:
    mov     cx, word [RootDirEntries]
    mov     di, 0x0200

    .search:
        push    cx
        mov     cx, 11
        mov     si, KernelLoader
        push    di
        rep     cmpsb
        pop     di
        je      LoadFAT
        pop     cx
        add     di, 0x0020
        loop    .search
        jmp     Failure

; Now Load the FAT
LoadFAT:
    mov     dx, word [di + 0x001A]  ; Cluster address == 0x000D (13)
    mov     word [Cluster], dx

    xor     ax, ax                  ; Clear AX
    mov     al, byte [NumberOfFATs] ; AL == NumberOfFATs (2)
    mul     word [SectorsPerFAT]    ; AX == NumberOfFATs * SectorsPerFAT (2 * 9 == 18)
    mov     cx, ax                  ; AX == 18, CX == 18

    mov     ax, word [ReservedSectors]  ; AX == ReservedSectors (1)

    mov     bx, 0x0200
    call    ReadSectors

    mov     ax, 0x0050
    mov     es, ax
    mov     bx, 0x0000
    push    bx

LoadImage:
    mov     ax, word [Cluster]
    pop     bx
    call    ClusterLBA
    xor     cx, cx
    mov     cl, byte [SectorsPerCluster]
    call    ReadSectors
    push    bx

    ; Now compute the next cluster
    mov     ax, word [Cluster]
    mov     cx, ax
    mov     dx, ax
    shr     dx, 0x0001
    add     cx, dx
    mov     bx, 0x0200
    add     bx, cx
    mov     dx, word [bx]
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
        jb      LoadImage

Done:
    mov     si, MSG_DONE
    call    Print
    push    word 0x0050
    push    word 0x0000
    retf

    cli
    hlt

Failure:
    mov     si, MSG_NOT_FOUND
    call    Print

    xor     ax, ax
    int     0x16
    int     0x19

KernelLoader        db  "KRNLDR  BIN"
Cluster             db  0x0000

; Constants
MSG_WELCOME:        db  "[BOOT] Welcome!",10,13,0
MSG_DONE:           db  "[BOOT] Finished!",10,13,10,13,0
MSG_ERROR:          db  "[BOOT] Error!",10,13,0
MSG_NOT_FOUND:      db  "[BOOT] KRNLDR.BIN not found...",0

times 510-($-$$) db 0
dw 0xAA55