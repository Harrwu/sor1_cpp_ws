.section .data
msg: .string "Hello World"

.section .text
.global _start

_start:
    li a0, 1 # Load 1 into a0 (File Descriptor: stdout)
    la a1, msg # Load the address of 'msg' into a1
    li a2, 11 # Load 5 into a2 (Length of "Hello")
    li a7, 64 # Load 64 into a7 (Syscall ID for write)
    ecall # Trigger the emulator's handler

    li a0, 0 # Exit code 0
    li a7, 93 # Syscall ID for exit
    ecall # Exit
    