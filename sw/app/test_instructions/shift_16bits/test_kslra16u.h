#ifndef KSLRA16_U_H
#define KSLRA16_U_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "print_bits.h"

uint32_t kslra16_u_sw(uint32_t a, int32_t b) {
    int16_t a1 = (int16_t)(a & 0xFFFF);
    int16_t a2 = (int16_t)((a >> 16) & 0xFFFF);
    int shift = b;

    int16_t res1, res2;

    if (b>0) {
        // Positive: logical left shift, with saturation
        res1 = saturate_16((int32_t)a1 << shift);
        res2 = saturate_16((int32_t)a2 << shift);
    } else {
        // Negative: arithmetic right shift (sign-extended)
        shift = -shift; // Convert to positive for right shift
        if (shift == 0){
            res1 = a1;
            res2 = a2;
        }
        else{
            int32_t temp_r1 = sign_extend_17((int32_t)a1  >> (shift - 1)) + 1;
            int32_t temp_r2 = sign_extend_17((int32_t)a2  >> (shift - 1)) + 1;

            res1 = (uint16_t)(temp_r1 >> 1);
            res2 = (uint16_t)(temp_r2 >> 1);
        }
    }

    return ((uint32_t)(uint16_t)res2 << 16) | (uint16_t)res1;
}

int run_single_kslra16_u_test(int test_num) {
    uint32_t a = ((uint32_t)rand() << 16) | rand();
    int b = (rand() % 32) - 16; // b in range -16 to 15

    if(b == -16){
        b = -15;
    }

    volatile int result;

    uint32_t expected = kslra16_u_sw(a, b);
    __asm__ volatile (
        "kslra16.u %0, %1, %2"
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
    //        test_num, "kslra16_u", pass ? "PASS" : "FAIL", result_bit, expected_bit, a_bit, b);

    return pass;
}

#endif // KSLRA16_U_H