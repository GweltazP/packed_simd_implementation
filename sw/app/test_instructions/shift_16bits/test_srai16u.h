#ifndef SRAI16_U_H
#define SRAI16_U_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>
#include "print_bits.h"

uint32_t srai16_u_sw(uint32_t a, uint32_t imm) {
    int16_t a1 = (int16_t)(a & 0xFFFF);
    int16_t a2 = (int16_t)((a >> 16) & 0xFFFF);
    uint8_t shift = imm & 0xF;
    
    uint16_t r1, r2;

    if (shift == 0){
        r1 = a1;
        r2 = a2;
    }
    else{
        int32_t temp_r1 = sign_extend_17((int32_t)a1  >> (shift - 1)) + 1;
        int32_t temp_r2 = sign_extend_17((int32_t)a2  >> (shift - 1)) + 1;

        r1 = temp_r1 >> 1;
        r2 = temp_r2 >> 1;
    }

    return ((uint32_t)r2 << 16) | r1;
}

int run_single_srai16u_test(int test_num) {
    uint32_t a = ((uint32_t)rand() << 16) | (uint32_t)rand();
    uint8_t imm = 3;

    volatile int result;

    uint32_t expected = srai16_u_sw(a, 3);
    asm volatile (
        "srai16.u %0, %1, 3"
        : "=r" (result)
        : "r" (a)
    );

    int pass = (result == expected);

    char a_bit[33];
    int32_to_binary_str(a, a_bit);
    char imm_bit[33];
    int32_to_binary_str(imm, imm_bit);
    char result_bit[33];
    int32_to_binary_str(result, result_bit);
    char expected_bit[33];
    int32_to_binary_str(expected, expected_bit);

    // printf("Test %3d: %-10s %s (\n\rResult: %s, \n\rExpected: %s, \n\ra=%s, \n\rb=%2d)\n\r",
    //        test_num, "srai16u", pass ? "PASS" : "FAIL", result_bit, expected_bit, a_bit, imm);

    return pass;
}

#endif // SRAI16_U_H
