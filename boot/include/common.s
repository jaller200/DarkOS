%ifndef __COMMON_S_INCLUDED__
%define __COMMON_S_INCLUDED__

; This is where we will load our kernel in protected mode (32-bit)
%define IMAGE_PMODE_BASE 0x100000

; This is where we will load our kernel in real mode (16-bit)
%define IMAGE_RMODE_BASE 0x3000

; Kernel Name
KernelName:     db  "KERNEL  BIN"

; Kernel Size (in bytes)
KernelSize:     db  0x00

%endif