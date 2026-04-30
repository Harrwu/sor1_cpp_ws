#include <stddef.h>

long write(int fd, const void *buf, int length){
    register long a7 asm("a7") = 64;
    register long a0 asm("a0") = fd;
    register long a1 asm("a1") = (long)buf;
    register long a2 asm("a2") = length;

    asm volatile ("ecall": "+r" (a0): "r" (a7), "r" (a0), "r" (a1), "r" (a2): "memory");
    return a0;
}

long read(int fd, void *buf, int length){
    register long a7 asm("a7") = 63;
    register long a0 asm("a0") = fd;
    register long a1 asm("a1") = (long)buf;
    register long a2 asm("a2") = length;

    asm volatile ("ecall": "+r" (a0): "r" (a7), "r" (a0), "r" (a1), "r" (a2): "memory");
    return a0;
}
