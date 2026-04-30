#!/bin/bash
set -e

# File names
ASM_SRC="crt0.s"
C_SRC="main.c"
C_LBR="syscall.c"
ASM_OBJ="crt0.o"
C_OBJ="main.o"
C_LBR_OBJ="syscall.o"
ELF_FILE="program.elf"
BIN_FILE="program.bin"

echo "Assembling startup code..."
riscv64-unknown-elf-as -march=rv32i -mabi=ilp32 -o $ASM_OBJ $ASM_SRC

echo "Compiling C code..."
# -ffreestanding: Tells compiler there is no OS (no printf, etc)
# -nostdlib: Don't link against standard libraries
# -fno-builtin: Don't use built-in functions
riscv64-unknown-elf-gcc -march=rv32i -mabi=ilp32 -ffreestanding -nostdlib -fno-builtin -c $C_SRC -o $C_OBJ
riscv64-unknown-elf-gcc -march=rv32i -mabi=ilp32 -ffreestanding -nostdlib -fno-builtin -c $C_LBR -o $C_LBR_OBJ

echo "Linking..."
# NOTE: crt0.o MUST come first so _start is at address 0x0
riscv64-unknown-elf-ld -m elf32lriscv -Ttext 0x0 -o $ELF_FILE $ASM_OBJ $C_OBJ $C_LBR_OBJ

echo "Converting to Binary..."
riscv64-unknown-elf-objcopy -O binary $ELF_FILE $BIN_FILE

# Clean up
rm $ASM_OBJ $C_OBJ $ELF_FILE

echo "Build complete: $BIN_FILE"
