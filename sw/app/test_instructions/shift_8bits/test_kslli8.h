#ifndef KSLLI8_H
#define KSLLI8_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "print_bits.h"


uint32_t kslli8_sw(uint32_t a, uint8_t imm) {
    int32_t a0 = (int8_t)(a & 0xFF);
    int32_t a1 = (int8_t)((a >> 8) & 0xFF);
    int32_t a2 = (int8_t)((a >> 16) & 0xFF);
    int32_t a3 = (int8_t)((a >> 24) & 0xFF);
    uint8_t shift = imm & 0x7;

    int32_t s0 = a0;
    int32_t s1 = a1;
    int32_t s2 = a2;
    int32_t s3 = a3;

    if(shift!=0){

        int32_t r0 = (uint32_t)a0 << shift;
        int32_t r1 = (uint32_t)a1 << shift;
        int32_t r2 = (uint32_t)a2 << shift;
        int32_t r3 = (uint32_t)a3 << shift;

        s0 = saturate_8(r0);
        s1 = saturate_8(r1);
        s2 = saturate_8(r2);
        s3 = saturate_8(r3);
        
    }

    return ((uint32_t)(uint8_t)s3 << 24) |
           ((uint32_t)(uint8_t)s2 << 16) |
           ((uint32_t)(uint8_t)s1 << 8)  |
           (uint32_t)(uint8_t)s0;
}

int run_single_kslli8_test(int test_num) {
    uint32_t a = ((uint32_t)rand() << 16) | rand();
    uint8_t imm = 3;

    volatile int result;

    uint32_t expected = kslli8_sw(a, imm);
    __asm__ volatile (
        "kslli8 %0, %1, 3"
        : "=r" (result)
        : "r" (a)
    );

    char a_bit[33];
    int32_to_binary_str(a, a_bit);
    char result_bit[33];
    int32_to_binary_str(result, result_bit);
    char expected_bit[33];
    int32_to_binary_str(expected, expected_bit);

    int pass = (result == expected);

    // printf("Test %3d: %-14s %s (\n\rResult: %s, \n\rExpected: %s, \n\ra=%s, \n\rb=%2d)\n\r",
    //        test_num, "kslli8", pass ? "PASS" : "FAIL", result_bit, expected_bit, a_bit, imm);

    return pass;
}

#endif // KSLLI8_H
