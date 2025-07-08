#ifndef KSLL16_H
#define KSLL16_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "print_bits.h"

uint32_t ksll16_sw(uint32_t a, uint32_t b) {
    int32_t a1 = (int16_t)(a & 0xFFFF);
    int32_t a2 = (int16_t)((a >> 16) & 0xFFFF);
    uint8_t shift = b & 0xF;

    int32_t s1 = a1;
    int32_t s2 = a2;

    if (shift != 0) {
        int32_t r1 = ((uint32_t)a1) << shift;
        int32_t r2 = ((uint32_t)a2) << shift;

        s1 = saturate_16(r1);
        s2 = saturate_16(r2);
    }

    return ((uint32_t)(uint16_t)s2 << 16) | (uint16_t)s1;
}

int run_single_ksll16_test(int test_num) {
    uint32_t a = ((uint32_t)rand() << 16) | rand();
    uint32_t b = rand();
    b = b & 0xF;

    volatile int result;

    uint32_t expected = ksll16_sw(a, b);
    __asm__ volatile (
        "ksll16 %0, %1, %2"
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

    // printf("Test %3d: %-14s %s (\n\rResult: %s, \n\rExpected: %s, \n\ra=%s, \n\rb=%2d)\n\r",
    //        test_num, "ksll16", pass ? "PASS" : "FAIL", result_bit, expected_bit, a_bit, b);

    return pass;
}

#endif // SLL16_SAT_H