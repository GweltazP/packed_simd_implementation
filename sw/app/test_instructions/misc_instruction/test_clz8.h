#ifndef TEST_CLZ8_H
#define TEST_CLZ8_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "print_bits.h"

// Count leading zeros in 8-bit value
uint8_t count_leading_zeros_8(uint8_t val) {
    uint8_t count = 0;
    for (int i = 7; i >= 0; --i) {
        if ((val >> i) & 1)
            break;
        count++;
    }
    return count;
}

// Software reference model for CLZ8
uint32_t clz8_sw(uint32_t rs1) {
    uint8_t a0 = (uint8_t)(rs1 & 0xFF);
    uint8_t a1 = (uint8_t)((rs1 >> 8) & 0xFF);
    uint8_t a2 = (uint8_t)((rs1 >> 16) & 0xFF);
    uint8_t a3 = (uint8_t)((rs1 >> 24) & 0xFF);

    uint8_t r0 = count_leading_zeros_8(a0);
    uint8_t r1 = count_leading_zeros_8(a1);
    uint8_t r2 = count_leading_zeros_8(a2);
    uint8_t r3 = count_leading_zeros_8(a3);

    return ((uint32_t)r3 << 24) |
           ((uint32_t)r2 << 16) |
           ((uint32_t)r1 << 8) |
           r0;
}

// Perform a single CLZ8 test with random input
int run_single_clz8_test(int test_num) {
    uint32_t a = ((uint32_t)rand() << 16) | (uint32_t)rand();

    uint32_t expected = clz8_sw(a);
    uint32_t result;

    // Replace with actual hardware instruction if available
    __asm__ volatile (
        "clz8 %0, %1"
        : "=r" (result)
        : "r" (a)
    );

    int pass = (result == expected);

    char a_bit[33], result_bit[33], expected_bit[33];
    int32_to_binary_str(a, a_bit);
    int32_to_binary_str(result, result_bit);
    int32_to_binary_str(expected, expected_bit);

    // printf("Test %3d: %-10s %s (\n\r"
    //        "a0 = 0x%02X, CLZ = %u\n\r"
    //        "a1 = 0x%02X, CLZ = %u\n\r"
    //        "a2 = 0x%02X, CLZ = %u\n\r"
    //        "a3 = 0x%02X, CLZ = %u\n\r"
    //        "Result    : %s\n\r"
    //        "Expected  : %s\n\r"
    //        "a_bits    : %s\n\r"
    //        ")\n\r",
    //        test_num, "clz8", pass ? "PASS" : "FAIL",
    //        (uint8_t)(a & 0xFF),     (uint8_t)(result & 0xFF),
    //        (uint8_t)((a >> 8) & 0xFF),  (uint8_t)((result >> 8) & 0xFF),
    //        (uint8_t)((a >> 16) & 0xFF), (uint8_t)((result >> 16) & 0xFF),
    //        (uint8_t)((a >> 24) & 0xFF), (uint8_t)((result >> 24) & 0xFF),
    //        result_bit, expected_bit, a_bit);

    return pass;
}

#endif
