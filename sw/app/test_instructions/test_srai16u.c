#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>
#include "print_bits.h"

uint32_t srai16_u_sw(uint32_t a, uint32_t imm) {
    int16_t a1 = (int16_t)(a & 0xFFFF);
    int16_t a2 = (int16_t)((a >> 16) & 0xFFFF);
    uint8_t shift = imm & 0xF;

    int32_t temp_r1 = sign_extend_17((int32_t)a1  >> (shift - 1)) + 1;
    int32_t temp_r2 = sign_extend_17((int32_t)a2  >> (shift - 1)) + 1;

    int32_t r1 = temp_r1 >> 1;
    int32_t r2 = temp_r2 >> 1;

    return ((uint16_t)r2 << 16) | ((uint16_t)r1);
}

int run_single_srai16_u_test(int test_num) {
    uint32_t a = ((uint32_t)rand() << 16) | (uint32_t)rand();
    uint8_t imm = rand() % 32;

    volatile int result;

    uint32_t expected = srai16_u_sw(a, 10);
    asm volatile (
        "srai16.u %0, %1, 10"
        : "=r" (result)
        : "r" (a), "r" (imm)
    );

    int pass = (result == expected);

    printf("Test %5d: %-8s %s (Result: 0x%08x, Expected: 0x%08x, a=0x%08x, imm=%u)\n",
           test_num, "srai16.u", pass ? "PASS" : "FAIL", result, expected, a, 10);

    return pass;
}
