#ifndef SRA16_U_H
#define SRA16_U_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>
#include "print_bits.h"

uint32_t sra16_u_sw(uint32_t a, uint32_t b) {
    int16_t a1 = (int16_t)(a & 0xFFFF);
    int16_t a2 = (int16_t)((a >> 16) & 0xFFFF);
    uint8_t shift = b & 0xF;
    
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

int run_single_sra16u_test(int test_num) {
    uint32_t a = ((uint32_t)rand() << 16) | (uint32_t)rand();
    uint32_t b = ((uint32_t)rand() << 16) | (uint32_t)rand();
    b = (b & 0xF);

    volatile int result;

    uint32_t expected = sra16_u_sw(a, b);
    asm volatile (
        "sra16.u %0, %1, %2"
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
    //        test_num, "sra16.u", pass ? "PASS" : "FAIL", result_bit, expected_bit, a_bit, b);

    return pass;
}

#endif // SRA16_U_H
