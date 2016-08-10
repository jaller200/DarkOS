BITS    32

EXTERN  main

start:
    cli
    mov     ax, 0x10
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax

    call    main

    cli
    hlt