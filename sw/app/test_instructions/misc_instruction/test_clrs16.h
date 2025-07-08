#ifndef TEST_CLRS16_H
#define TEST_CLRS16_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "print_bits.h"

// Count redundant sign bits for a signed 16-bit value
uint16_t count_leading_sign_bits_16(int16_t val) {
    uint16_t count = 0;
    int sign = (val >> 15) & 1;

    for (int i = 14; i >= 0; --i) {
        int bit = (val >> i) & 1;
        if (bit == sign)
            count++;
        else
            break;
    }

    return count;
}

// Software reference model for CLRS16
// Takes rs1 with two signed 16-bit integers, returns packed result in rd
uint32_t clrs16_sw(uint32_t rs1) {
    int16_t a0 = (int16_t)(rs1 & 0xFFFF);
    int16_t a1 = (int16_t)((rs1 >> 16) & 0xFFFF);

    uint16_t r0 = count_leading_sign_bits_16(a0);
    uint16_t r1 = count_leading_sign_bits_16(a1);

    return ((uint32_t)r1 << 16) | r0;
}

// Perform a single CLRS16 test with random input
int run_single_clrs16_test(int test_num) {
    uint32_t a = ((uint32_t)rand() << 16) | (uint32_t)rand();

    uint32_t expected = clrs16_sw(a);
    uint32_t result;

    // Replace this with actual hardware implementation if available
    __asm__ volatile (
        "clrs16 %0, %1"
        : "=r" (result)
        : "r" (a)
    );

    int pass = (result == expected);

    char a_bit[33], result_bit[33], expected_bit[33];
    int32_to_binary_str(a, a_bit);
    int32_to_binary_str(result, result_bit);
    int32_to_binary_str(expected, expected_bit);

    printf("Test %3d: %-10s %s (\n\r"
        "a0 = %6d (signed), CLRS = %2u\n\r"
        "a1 = %6d (signed), CLRS = %2u\n\r"
        "Result    : %s\n\r"
        "Expected  : %s\n\r"
        "a_bit     : %s\n\r"
        ")\n\r",
        test_num, "clrs16", pass ? "PASS" : "FAIL",
        (int16_t)(a & 0xFFFF), (uint16_t)(result & 0xFFFF),
        (int16_t)((a >> 16) & 0xFFFF), (uint16_t)((result >> 16) & 0xFFFF),
        result_bit, expected_bit, a_bit);

    return pass;
}

#endif