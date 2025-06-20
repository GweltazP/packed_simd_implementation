#ifndef KSLL8_H
#define KSLL8_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "print_bits.h"

uint32_t ksll8_sw(uint32_t a, uint32_t b) {
    int8_t a0 = (int8_t)(a & 0xFF);
    int8_t a1 = (int8_t)((a >> 8) & 0xFF);
    int8_t a2 = (int8_t)((a >> 16) & 0xFF);
    int8_t a3 = (int8_t)((a >> 24) & 0xFF);
    uint8_t shift = b & 0xF;

    int8_t r0 = saturate_8((int32_t)a0 << shift);
    int8_t r1 = saturate_8((int32_t)a1 << shift);
    int8_t r2 = saturate_8((int32_t)a2 << shift);
    int8_t r3 = saturate_8((int32_t)a3 << shift);

    return ((uint32_t)(uint8_t)r3 << 24) |
           ((uint32_t)(uint8_t)r2 << 16) |
           ((uint32_t)(uint8_t)r1 << 8)  |
           (uint32_t)(uint8_t)r0;
}

int run_single_ksll8_test(int test_num) {
    uint32_t a = ((uint32_t)rand() << 16) | rand();
    uint32_t b = rand();
    b = b & 0xF;

    volatile int result;

    uint32_t expected = ksll8_sw(a, b);
    __asm__ volatile (
        "ksll8 %0, %1, %2"
        : "=r" (result)
        : "r" (a), "r" (b)
    );

    char a_bit[33], b_bit[33], result_bit[33], expected_bit[33];
    int32_to_binary_str(a, a_bit);
    int32_to_binary_str(b, b_bit);
    int32_to_binary_str(result, result_bit);
    int32_to_binary_str(expected, expected_bit);

    int pass = (result == expected);

    // printf("Test %3d: %-14s %s (\n\rResult: %s, \n\rExpected: %s, \n\ra=%s, \n\rb=%2d)\n\r",
    //        test_num, "ksll8", pass ? "PASS" : "FAIL", result_bit, expected_bit, a_bit, b);

    return pass;
}

#endif // KSLL8_H