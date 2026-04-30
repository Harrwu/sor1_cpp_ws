.section .text
.global _start

_start:

    li sp, 1024

    # call main
    call main

    # Prepare to exit 
    li a0, 0
    li a7, 93
    ecall 
