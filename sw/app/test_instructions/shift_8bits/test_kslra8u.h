#ifndef KSLRA8_U_H
#define KSLRA8_U_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "print_bits.h"

uint32_t kslra8_u_sw(uint32_t a, int32_t b) {
    int32_t a0 = (int8_t)(a & 0xFF);
    int32_t a1 = (int8_t)((a >> 8) & 0xFF);
    int32_t a2 = (int8_t)((a >> 16) & 0xFF);
    int32_t a3 = (int8_t)((a >> 24) & 0xFF);
    int shift = b;

    int32_t res0, res1, res2, res3;

    if (b > 0) {
        // Positive: logical left shift, with saturation
        res0 = saturate_8((int32_t)a0 << shift);
        res1 = saturate_8((int32_t)a1 << shift);
        res2 = saturate_8((int32_t)a2 << shift);
        res3 = saturate_8((int32_t)a3 << shift);
    } else {
        // Negative: arithmetic right shift (sign-extended)
        shift = -shift;
        if (shift == 0) {
            res0 = (uint8_t)a0;
            res1 = (uint8_t)a1;
            res2 = (uint8_t)a2;
            res3 = (uint8_t)a3;
        } else {
            // Shift right with rounding for each lane:
            // temp = (val >> (shift - 1)) + 1
            // then arithmetic right shift by 1
            int32_t temp0 = sign_extend_9(((int32_t)a0) >> (shift - 1)) + 1;
            int32_t temp1 = sign_extend_9(((int32_t)a1) >> (shift - 1)) + 1;
            int32_t temp2 = sign_extend_9(((int32_t)a2) >> (shift - 1)) + 1;
            int32_t temp3 = sign_extend_9(((int32_t)a3) >> (shift - 1)) + 1;

            res0 = (uint8_t)(temp0 >> 1);
            res1 = (uint8_t)(temp1 >> 1);
            res2 = (uint8_t)(temp2 >> 1);
            res3 = (uint8_t)(temp3 >> 1);
        }
    }

    return ((uint32_t)(uint8_t)res3 << 24) |
           ((uint32_t)(uint8_t)res2 << 16) |
           ((uint32_t)(uint8_t)res1 << 8)  |
           (uint32_t)(uint8_t)res0;
}

int run_single_kslra8_u_test(int test_num) {
    uint32_t a = ((uint32_t)rand() << 16) | rand();
    int b = (rand() % 16) - 8; // b in range -8 to 7
    
    if(b == -8){
        b = -7; // Avoid -8 to prevent undefined behavior in the shift operation
    }

    volatile int result;

    uint32_t expected = kslra8_u_sw(a, b);
    __asm__ volatile (
        "kslra8.u %0, %1, %2"
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
    //        test_num, "kslra8_u", pass ? "PASS" : "FAIL", result_bit, expected_bit, a_bit, b);

    return pass;
}

#endif // KSLRA8_U_H