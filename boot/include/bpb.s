%ifndef __BPB_S_INCLUDED__
%define __BPB_S_INCLUDED__

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

%endif