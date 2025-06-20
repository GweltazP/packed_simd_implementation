#ifndef TEST_SRAI8_H
#define TEST_SRAI8_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "print_bits.h"

// Software reference model: 4x8-bit arithmetic right shift
uint32_t srai8_sw(uint32_t a, uint32_t imm) {
    int8_t a0 = (int8_t)((a >> 0)  & 0xFF);
    int8_t a1 = (int8_t)((a >> 8)  & 0xFF);
    int8_t a2 = (int8_t)((a >> 16) & 0xFF);
    int8_t a3 = (int8_t)((a >> 24) & 0xFF);
    uint8_t shift = imm & 0xF;

    int8_t r0 = a0 >> shift;
    int8_t r1 = a1 >> shift;
    int8_t r2 = a2 >> shift;
    int8_t r3 = a3 >> shift;

    return ((uint32_t)(uint8_t)r3 << 24) |
           ((uint32_t)(uint8_t)r2 << 16) |
           ((uint32_t)(uint8_t)r1 << 8)  |
           ((uint32_t)(uint8_t)r0);
}

int run_single_srai8_test(int test_num) {
    uint32_t a = ((uint32_t)rand() << 16) | rand();
    uint32_t imm = 3;

    volatile int result;

    uint32_t expected = sra8_sw(a, imm);
    __asm__ volatile (
        "srai8 %0, %1, 3"  // Replace with actual SIMD instruction if supported
        : "=r" (result)
        : "r" (a)
    );

    char a_bit[33], imm_bit[33], result_bit[33], expected_bit[33];
    int32_to_binary_str(a, a_bit);
    int32_to_binary_str(imm, imm_bit);
    int32_to_binary_str(result, result_bit);
    int32_to_binary_str(expected, expected_bit);

    int pass = (result == expected);

    // printf("Test %3d: %-10s %s (\n\rResult: %s, \n\rExpected: %s, \n\ra=%s, \n\rb=%2d)\n\r",
    //        test_num, "sra8", pass ? "PASS" : "FAIL", result_bit, expected_bit, a_bit, imm);

    return pass;
}

#endif