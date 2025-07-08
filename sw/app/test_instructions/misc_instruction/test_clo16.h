#ifndef TEST_CLO16_H
#define TEST_CLO16_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "print_bits.h"

// Count leading ones in a 16-bit number
uint16_t count_leading_ones_16(uint16_t val) {
    uint16_t count = 0;
    for (int i = 15; i >= 0; --i) {
        if (((val >> i) & 1) == 0)
            break;
        count++;
    }
    return count;
}

// Software reference model for CLO16
uint32_t clo16_sw(uint32_t rs1) {
    uint16_t a0 = (uint16_t)(rs1 & 0xFFFF);          // lower 16 bits
    uint16_t a1 = (uint16_t)((rs1 >> 16) & 0xFFFF);  // upper 16 bits

    uint16_t r0 = count_leading_ones_16(a0);
    uint16_t r1 = count_leading_ones_16(a1);

    return ((uint32_t)r1 << 16) | r0;
}

// Perform a single CLO16 test
int run_single_clo16_test(int test_num) {
    uint32_t a = ((uint32_t)rand() << 16) | (uint32_t)rand();

    uint32_t expected = clo16_sw(a);
    uint32_t result;

    // Replace this with your actual CLO16 hardware instruction
    __asm__ volatile (
        "clo16 %0, %1"
        : "=r" (result)
        : "r" (a)
    );

    int pass = (result == expected);

    char a_bit[33], result_bit[33], expected_bit[33];
    int32_to_binary_str(a, a_bit);
    int32_to_binary_str(result, result_bit);
    int32_to_binary_str(expected, expected_bit);

    // printf("Test %3d: %-10s %s (\n\r"
    //        "a0 = 0x%04X, CLO = %2u\n\r"
    //        "a1 = 0x%04X, CLO = %2u\n\r"
    //        "Result     : %s\n\r"
    //        "Expected   : %s\n\r"
    //        "a_bits     : %s\n\r"
    //        ")\n\r",
    //        test_num, "clo16", pass ? "PASS" : "FAIL",
    //        (uint16_t)(a & 0xFFFF),     (uint16_t)(result & 0xFFFF),
    //        (uint16_t)((a >> 16) & 0xFFFF), (uint16_t)((result >> 16) & 0xFFFF),
    //        result_bit, expected_bit, a_bit);

    return pass;
}

#endif
