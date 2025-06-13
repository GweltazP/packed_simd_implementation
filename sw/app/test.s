.section .text
.globl _start
_start:
    li t0, 0x12345678
    srai16 t1, t0, 3