#ifndef SRAI8_U_H
#define SRAI8_U_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "print_bits.h"

uint32_t srai8_u_sw(uint32_t a, uint8_t imm) {
    int8_t a0 = (int8_t)(a & 0xFF);
    int8_t a1 = (int8_t)((a >> 8) & 0xFF);
    int8_t a2 = (int8_t)((a >> 16) & 0xFF);
    int8_t a3 = (int8_t)((a >> 24) & 0xFF);

    uint8_t shift = imm & 0xF;

    int32_t r0, r1, r2, r3;

    if (shift == 0) {
        r0 = a0; r1 = a1; r2 = a2; r3 = a3;
    } else {
        r0 = (sign_extend_9((int32_t)a0 >> (shift - 1)) + 1) >> 1;
        r1 = (sign_extend_9((int32_t)a1 >> (shift - 1)) + 1) >> 1;
        r2 = (sign_extend_9((int32_t)a2 >> (shift - 1)) + 1) >> 1;
        r3 = (sign_extend_9((int32_t)a3 >> (shift - 1)) + 1) >> 1;
    }

    return ((uint32_t)(uint8_t)r3 << 24) |
           ((uint32_t)(uint8_t)r2 << 16) |
           ((uint32_t)(uint8_t)r1 << 8) |
           (uint32_t)(uint8_t)r0;
}

int run_single_srai8u_test(int test_num) {
    uint32_t a = ((uint32_t)rand() << 16) | (uint32_t)rand();
    uint8_t imm = 3; // can be varied for more tests

    volatile int result;

    uint32_t expected = srai8_u_sw(a, imm);
    asm volatile (
        "srai8.u %0, %1, 3"
        : "=r" (result)
        : "r" (a)
    );

    int pass = (result == expected);

    char a_bit[33], result_bit[33], expected_bit[33];
    int32_to_binary_str(a, a_bit);
    int32_to_binary_str(result, result_bit);
    int32_to_binary_str(expected, expected_bit);

    // printf("Test %3d: %-10s %s (\n\rResult: %s\n\rExpected: %s\n\ra=%s, imm=%2d)\n\r",
    //        test_num, "srai8.u", pass ? "PASS" : "FAIL", result_bit, expected_bit, a_bit, imm);

    return pass;
}

#endif // SRAI8_U_H