#ifndef SRL16_U_H
#define SRL16_U_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>
#include "print_bits.h"

uint32_t srl16u_sw(uint32_t a, uint8_t b) {
    uint16_t a1 = (uint16_t)(a & 0xFFFF);
    uint16_t a2 = (uint16_t)((a >> 16) & 0xFFFF);
    uint8_t shift = b & 0xF;

    uint16_t r1, r2;

    if (shift == 0) {
        r1 = a1;
        r2 = a2;
    } else {
        uint16_t rounding_bias = 1 << (shift - 1);
        r1 = (a1 + rounding_bias) >> shift;
        r2 = (a2 + rounding_bias) >> shift;
    }

    return ((uint32_t)r2 << 16) | r1;
}

int run_single_srl16u_test(int test_num){
    uint32_t a = ((uint32_t)rand() << 16) | (uint32_t)rand();
    uint32_t b = ((uint32_t)rand() << 16) | (uint32_t)rand();
    b = (b & 0xF);

    volatile int result;

    uint32_t expected = srl16u_sw(a, b);
    __asm__ volatile (
        "srl16.u %0, %1, %2"
        : "=r" (result)
        : "r" (a), "r" (b)
    );

    char a_bit[33];
    int32_to_binary_str(a, a_bit);
    char b_bit[33];
    int32_to_binary_str(b, b_bit);
    char result_bit[33];
    int32_to_binary_str(result, result_bit);
    char expected_bit[33];
    int32_to_binary_str(expected, expected_bit);

    int pass = (result == expected);

    // printf("Test %3d: %-10s %s (\n\rResult: %s, \n\rExpected: %s, \n\ra=%s, \n\rb=%2d)\n\r",
    //        test_num, "srl16.u", pass ? "PASS" : "FAIL", result_bit, expected_bit, a_bit, b);

    return pass;
}

#endif // SRL16_U_H