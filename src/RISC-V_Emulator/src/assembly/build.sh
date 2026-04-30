#!/bin/bash
set -e

ASM_FILE="msg.s"
OBJ_FILE="msg.o"
ELF_FILE="msg.elf"
BIN_FILE="msg.bin"

echo "Assembling for RV32I..."
riscv64-unknown-elf-as -march=rv32i -mabi=ilp32 -o $OBJ_FILE $ASM_FILE

echo "Linking..."
riscv64-unknown-elf-ld -m elf32lriscv -Ttext 0x0 -o $ELF_FILE $OBJ_FILE

echo "Converting to Binary..."
riscv64-unknown-elf-objcopy -O binary $ELF_FILE $BIN_FILE

rm $OBJ_FILE $ELF_FILE

echo "Build complete: $BIN_FILE"