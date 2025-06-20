#ifndef SRLI16_H
#define SRLI16_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>
#include "print_bits.h"

uint32_t srli16_sw(uint32_t a, uint8_t imm){
    uint16_t a1 = (uint16_t)(a & 0xFFFF);
    uint16_t a2 = (uint16_t)((a >> 16) & 0xFFFF);
    uint8_t shift = imm & 0xF;

    uint16_t r1 = a1 >> shift;
    uint16_t r2 = a2 >> shift;

    return ((uint32_t)r2 << 16) | r1;
}

int run_single_srli16_test(int test_num){
    uint32_t a = ((uint32_t)rand() << 16) | (uint32_t)rand();
    uint8_t imm = 3;

    volatile int result;

    uint32_t expected = srli16_sw(a, imm);
    __asm__ volatile (
        "srli16 %0, %1, 3"
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

    // printf("Test %3d: %-10s %s (\n\rResult: %s, \n\rExpected: %s, \n\ra=%s, \n\rb=%2d)\n\r",
    //        test_num, "srli16", pass ? "PASS" : "FAIL", result_bit, expected_bit, a_bit, imm);

    return pass;
}

#endif // SRL16_U_H