#ifndef TEST_UCLIP8_H
#define TEST_UCLIP8_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "print_bits.h"

// Software reference model for UCLIP8
uint32_t uclip8_sw(uint32_t rs1, uint8_t imm4u, uint8_t* ov) {
    int8_t a0 = (int8_t)(rs1 & 0xFF);
    int8_t a1 = (int8_t)((rs1 >> 8) & 0xFF);
    int8_t a2 = (int8_t)((rs1 >> 16) & 0xFF);
    int8_t a3 = (int8_t)((rs1 >> 24) & 0xFF);

    uint8_t max_val = (1 << imm4u);

    uint8_t r0, r1, r2, r3;

    *ov = 0;

    // Clip each byte
    r0 = (a0 < 0) ? 0 : (a0 >= max_val ? max_val - 1 : (uint8_t)a0);
    *ov |= (a0 < 0 || a0 >= max_val);

    r1 = (a1 < 0) ? 0 : (a1 >= max_val ? max_val - 1 : (uint8_t)a1);
    *ov |= (a1 < 0 || a1 >= max_val);

    r2 = (a2 < 0) ? 0 : (a2 >= max_val ? max_val - 1 : (uint8_t)a2);
    *ov |= (a2 < 0 || a2 >= max_val);

    r3 = (a3 < 0) ? 0 : (a3 >= max_val ? max_val - 1 : (uint8_t)a3);
    *ov |= (a3 < 0 || a3 >= max_val);

    return  ((uint32_t)r3 << 24) |
            ((uint32_t)r2 << 16) |
            ((uint32_t)r1 << 8)  |
            (uint32_t)r0;
}

// Perform a single UCLIP8 test with random inputs
int run_single_uclip8_test(int test_num) {
    uint32_t a = ((uint32_t)rand() << 16) | (uint32_t)rand();
    uint8_t imm = 6; // Choose value between 0–7 safely for 8-bit clip

    uint32_t expected;
    uint8_t expected_ov = 0;
    uint32_t result;
    uint8_t result_ov = 0;

    expected = uclip8_sw(a, imm, &expected_ov);

    // Run actual instruction
    __asm__ volatile (
        "uclip8 %0, %1, 6"
        : "=r" (result)
        : "r" (a)
    );

    int pass = (result == expected); // Extend with ov check if needed

    char a_bit[33], result_bit[33], expected_bit[33];
    int32_to_binary_str(a, a_bit);
    int32_to_binary_str(result, result_bit);
    int32_to_binary_str(expected, expected_bit);

    // printf("Test %3d: %-10s %s (\n\r"
    //     "Result_b0 : %4u (unsigned)\n\r"
    //     "Result_b1 : %4u (unsigned)\n\r"
    //     "Result_b2 : %4u (unsigned)\n\r"
    //     "Result_b3 : %4u (unsigned)\n\r"
    //     "Result_bit: %s,\n\r"
    //     "Expected_b0 : %4u (unsigned)\n\r"
    //     "Expected_b1 : %4u (unsigned)\n\r"
    //     "Expected_b2 : %4u (unsigned)\n\r"
    //     "Expected_b3 : %4u (unsigned)\n\r"
    //     "Expected_bit: %s,\n\r"
    //     "a_b0 : %4d (signed)\n\r"
    //     "a_b1 : %4d (signed)\n\r"
    //     "a_b2 : %4d (signed)\n\r"
    //     "a_b3 : %4d (signed)\n\r"
    //     "a_bit = %s,\n\r"
    //     "max = %2d,\n\r"
    //     "min = %2d)\n\r",
    //     test_num, "uclip8", pass ? "PASS" : "FAIL",
    //     (uint8_t)((result >> 0) & 0xFF),
    //     (uint8_t)((result >> 8) & 0xFF),
    //     (uint8_t)((result >> 16) & 0xFF),
    //     (uint8_t)((result >> 24) & 0xFF),
    //     result_bit,
    //     (uint8_t)((expected >> 0) & 0xFF),
    //     (uint8_t)((expected >> 8) & 0xFF),
    //     (uint8_t)((expected >> 16) & 0xFF),
    //     (uint8_t)((expected >> 24) & 0xFF),
    //     expected_bit,
    //     (int8_t)((a >> 0) & 0xFF),
    //     (int8_t)((a >> 8) & 0xFF),
    //     (int8_t)((a >> 16) & 0xFF),
    //     (int8_t)((a >> 24) & 0xFF),
    //     a_bit, (1 << imm) - 1, 0);

    return pass;
}

#endif
