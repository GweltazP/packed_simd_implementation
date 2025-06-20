#ifndef SRL8_H
#define SRL8_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "print_bits.h"

uint32_t srl8_sw(uint32_t a, uint32_t b) {
    uint8_t a0 = (uint8_t)(a & 0xFF);
    uint8_t a1 = (uint8_t)((a >> 8) & 0xFF);
    uint8_t a2 = (uint8_t)((a >> 16) & 0xFF);
    uint8_t a3 = (uint8_t)((a >> 24) & 0xFF);
    uint8_t shift = b & 0xF;

    uint8_t r0 = a0 >> shift;
    uint8_t r1 = a1 >> shift;
    uint8_t r2 = a2 >> shift;
    uint8_t r3 = a3 >> shift;

    return ((uint32_t)r3 << 24) | ((uint32_t)r2 << 16) | ((uint32_t)r1 << 8) | r0;
}

int run_single_srl8_test(int test_num) {
    uint32_t a = ((uint32_t)rand() << 16) | (uint32_t)rand();
    uint32_t b = rand();
    b = b & 0x7;

    volatile int result;

    uint32_t expected = srl8_sw(a, b);
    __asm__ volatile (
        "srl8 %0, %1, %2"
        : "=r" (result)
        : "r" (a), "r" (b)
    );

    char a_bit[33], b_bit[33], result_bit[33], expected_bit[33];
    int32_to_binary_str(a, a_bit);
    int32_to_binary_str(b, b_bit);
    int32_to_binary_str(result, result_bit);
    int32_to_binary_str(expected, expected_bit);

    int pass = (result == expected);

    // printf("Test %3d: %-10s %s (\n\rResult: %s, \n\rExpected: %s, \n\ra=%s, \n\rb=%2d)\n\r",
    //        test_num, "srl8", pass ? "PASS" : "FAIL", result_bit, expected_bit, a_bit, b);

    return pass;
}

#endif // SRL8_H
