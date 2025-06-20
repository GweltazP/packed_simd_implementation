#ifndef KSLLI16_H
#define KSLLI16_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "print_bits.h"

uint32_t kslli16_sw(uint32_t a, uint32_t imm) {
    int16_t a1 = (int16_t)(a & 0xFFFF);
    int16_t a2 = (int16_t)((a >> 16) & 0xFFFF);
    uint8_t shift = imm & 0xF;

    int32_t r1 = (int32_t)a1 << shift;
    int32_t r2 = (int32_t)a2 << shift;

    int16_t s1 = saturate_16(r1);
    int16_t s2 = saturate_16(r2);

    return ((uint32_t)(uint16_t)s2 << 16) | ((uint16_t)s1);
}

int run_single_kslli16_test(int test_num) {
    uint32_t a = ((uint32_t)rand() << 16) | rand();
    uint32_t imm = 3;

    volatile int result;

    uint32_t expected = kslli16_sw(a, imm);
    __asm__ volatile (
        "kslli16 %0, %1, 3"
        : "=r" (result)
        : "r" (a)
    );

    char a_bit[33];
    int32_to_binary_str(a, a_bit);
    char imm_bit[33];
    int32_to_binary_str(imm, imm_bit);
    char result_bit[33];
    int32_to_binary_str(result, result_bit);
    char expected_bit[33];
    int32_to_binary_str(expected, expected_bit);

    int pass = (result == expected);

    // printf("Test %3d: %-14s %s (\n\rResult: %s, \n\rExpected: %s, \n\ra=%s, \n\rb=%2d)\n\r",
    //        test_num, "kslli16", pass ? "PASS" : "FAIL", result_bit, expected_bit, a_bit, imm);

    return pass;
}

#endif // SLL16_SAT_H
