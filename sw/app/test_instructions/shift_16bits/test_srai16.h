#ifndef TEST_SRAI16_H
#define TEST_SRAI16_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>
#include "print_bits.h"

uint32_t srai16_sw(uint32_t a, uint8_t imm) {
    int16_t a1 = (int16_t)(a & 0xFFFF);
    int16_t a2 = (int16_t)((a >> 16) & 0xFFFF);
    uint8_t shift = imm & 0xF;

    int32_t r1 = a1 >> shift;
    int32_t r2 = a2 >> shift;

    return ((uint16_t)r2 << 16) | ((uint16_t)r1);
}

// Perform a single srai16 test with random inputs.
// Return 1 if test passed, 0 otherwise.
// Also print result info.
int run_single_srai16_test(int test_num) {
    uint32_t a = ((uint32_t)rand() << 16) | (uint32_t)rand();
    uint8_t imm = 3;

    volatile int result;

    uint32_t expected = srai16_sw(a, imm);
    __asm__ volatile (
        "srai16 %0, %1, %2"
        : "=r" (result)
        : "r" (a), "i" (imm)
    );

    int pass = (result == expected);

    char a_bit[33];
    int32_to_binary_str(a, a_bit);
    char imm_bit[33];
    int32_to_binary_str(imm, imm_bit);
    char result_bit[33];
    int32_to_binary_str(result, result_bit);
    char expected_bit[33];
    int32_to_binary_str(expected, expected_bit);


    // printf("Test %3d: %-10s %s (\n\rResult: %s, \n\rExpected: %s, \n\ra=%s, \n\rb=%2d)\n\r",
    //        test_num, "srai16", pass ? "PASS" : "FAIL", result_bit, expected_bit, a_bit, imm);

    return pass;
}

#endif
