#ifndef TEST_UCLIP16_H
#define TEST_UCLIP16_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "print_bits.h"

// Software reference model for UCLIP16
uint32_t uclip16_sw(uint32_t rs1, uint8_t imm4u, uint8_t* ov) {
    int16_t a1 = (int16_t)(rs1 & 0xFFFF);
    int16_t a2 = (int16_t)((rs1 >> 16) & 0xFFFF);

    uint16_t max_val = (1 << imm4u);
    uint16_t r1, r2;

    *ov = 0;

    // Clamp a1
    if (a1 < 0) {
        r1 = 0;
        *ov = 1;
    } else if (a1 > max_val - 1) {
        r1 = max_val - 1;
        *ov = 1;
    } else {
        r1 = (uint16_t)a1;
    }

    // Clamp a2
    if (a2 < 0) {
        r2 = 0;
        *ov = 1;
    } else if (a2 > max_val - 1) {
        r2 = max_val - 1;
        *ov = 1;
    } else {
        r2 = (uint16_t)a2;
    }

    return ((uint32_t)r2 << 16) | r1;
}

// Perform a single UCLIP16 test with random inputs
int run_single_uclip16_test(int test_num) {
    uint32_t a = ((uint32_t)rand() << 16) | (uint32_t)rand();
    uint8_t imm = 11; // 0–15 range

    uint32_t expected;
    uint8_t expected_ov = 0;
    uint32_t result;
    uint8_t result_ov = 0;

    expected = uclip16_sw(a, imm, &expected_ov);

    __asm__ volatile (
        "uclip16 %0, %1, 11"
        : "=r" (result)
        : "r" (a)
    );

    int pass = (result == expected);

    char a_bit[33], result_bit[33], expected_bit[33];
    int32_to_binary_str(a, a_bit);
    int32_to_binary_str(result, result_bit);
    int32_to_binary_str(expected, expected_bit);

    // printf("Test %3d: %-10s %s (\n\r"
    //     "Result_lo   : %6u (unsigned)\n\r"
    //     "Result_hi   : %6u (unsigned)\n\r"
    //     "Result_bit: %s,\n\r"
    //     "Expected_lo : %6u (unsigned)\n\r"
    //     "Expected_hi : %6u (unsigned)\n\r"
    //     "Expected_bit: %s,\n\r"
    //     "a_lo        : %6d (signed)\n\r"
    //     "a_hi        : %6d (signed)\n\r"
    //     "a_bit       = %s,\n\r"
    //     "max         = %2d,\n\r"
    //     "min         = %2d)\n\r",
    //     test_num, "uclip16", pass ? "PASS" : "FAIL",
    //     (uint16_t)(result & 0xFFFF),
    //     (uint16_t)((result >> 16) & 0xFFFF),
    //     result_bit,
    //     (uint16_t)(expected & 0xFFFF),
    //     (uint16_t)((expected >> 16) & 0xFFFF),
    //     expected_bit,
    //     (int16_t)(a & 0xFFFF),
    //     (int16_t)((a >> 16) & 0xFFFF),
    //     a_bit,
    //     (1 << imm) - 1, 0);

    return pass;
}

#endif
