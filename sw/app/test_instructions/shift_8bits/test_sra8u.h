#ifndef SRA8_U_H
#define SRA8_U_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "print_bits.h"

uint32_t sra8_u_sw(uint32_t a, uint32_t b) {
    int8_t a0 = (int8_t)(a & 0xFF);
    int8_t a1 = (int8_t)((a >> 8) & 0xFF);
    int8_t a2 = (int8_t)((a >> 16) & 0xFF);
    int8_t a3 = (int8_t)((a >> 24) & 0xFF);

    uint8_t shift = b & 0xF;

    uint8_t r0, r1, r2, r3;

    if (shift == 0) {
        r0 = (uint8_t)a0;
        r1 = (uint8_t)a1;
        r2 = (uint8_t)a2;
        r3 = (uint8_t)a3;
    } else {
        // Shift right with rounding for each lane:
        // temp = (val >> (shift - 1)) + 1
        // then arithmetic right shift by 1
        int32_t temp0 = sign_extend_9(((int32_t)a0) >> (shift - 1)) + 1;
        int32_t temp1 = sign_extend_9(((int32_t)a1) >> (shift - 1)) + 1;
        int32_t temp2 = sign_extend_9(((int32_t)a2) >> (shift - 1)) + 1;
        int32_t temp3 = sign_extend_9(((int32_t)a3) >> (shift - 1)) + 1;

        r0 = (uint8_t)(temp0 >> 1);
        r1 = (uint8_t)(temp1 >> 1);
        r2 = (uint8_t)(temp2 >> 1);
        r3 = (uint8_t)(temp3 >> 1);
    }

    return ((uint32_t)r3 << 24) | ((uint32_t)r2 << 16) | ((uint32_t)r1 << 8) | r0;
}

int run_single_sra8u_test(int test_num) {
    uint32_t a = ((uint32_t)rand() << 16) | (uint32_t)rand();
    uint32_t b = rand() & 0x7;

    volatile int result;

    uint32_t expected = sra8_u_sw(a, b);
    asm volatile (
        "sra8.u %0, %1, %2"
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
    //        test_num, "sra8.u", pass ? "PASS" : "FAIL", result_bit, expected_bit, a_bit, b);

    return pass;
}

#endif // SRA8_U_H
