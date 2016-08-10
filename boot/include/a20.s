%ifndef __A20_S_INCLUDED__
%define __A20_S_INCLUDED__

; EnableA20()
EnableA20:
    cli
    pusha

    call    wait_input

    ; Disable the keyboard
    mov     al, 0xAD
    out     0x64, al
    call    wait_input

    ; Send read output port command
    mov     al, 0xD0        ; Read Output Port (Keyboard Controller)
    out     0x64, al
    call    wait_output     ; Wait for the output buffer to be clear

    ; Send input buffer and store on stack
    in      al, 0x60        ; Now get the value in port 0x60
    push    eax             ; ... And store it in EAX
    call    wait_input      ; Wait for input buffer to be clear

    ; Send write output port command
    mov     al, 0xD1        ; Now prepare to write the value to the port
    out     0x64, al
    call    wait_input      ; Wait for input buffer to be clear

    ; Pop the output port data from the stack
    pop     eax             ; Get the data that was in port 0x60
    or      al, 2           ; And set Bit 1 to "1"
    out     0x60, al        ; Now write the data itself to port 0x60
    call    wait_input

    ; Enable the keyboard
    mov     al, 0xAE
    out     0x60, al

    call    wait_input

    popa
    sti
    ret

wait_input:
    in      al, 0x64
    test    al, 0x02
    jnz     wait_input
    ret

wait_output:
    in      al, 0x64
    test    al, 0x01
    jz      wait_output
    ret

%endif