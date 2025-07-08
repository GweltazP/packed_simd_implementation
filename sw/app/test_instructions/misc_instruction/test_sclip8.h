#ifndef TEST_SCLIP8_H
#define TEST_SCLIP8_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "print_bits.h"

// Software reference model for SCLIP8
// Returns packed result in rd, sets *ov if overflow occurred
uint32_t sclip8_sw(uint32_t rs1, uint8_t imm4u, uint8_t* ov) {
    int32_t a0 = (int8_t)(rs1 & 0xFF);
    int32_t a1 = (int8_t)((rs1 >> 8) & 0xFF);
    int32_t a2 = (int8_t)((rs1 >> 16) & 0xFF);
    int32_t a3 = (int8_t)((rs1 >> 24) & 0xFF);

    int16_t max_val = (1 << imm4u) - 1;
    int16_t min_val = -(1 << imm4u);

    int16_t r0 = a0;
    int16_t r1 = a1;
    int16_t r2 = a2;
    int16_t r3 = a3;

    *ov = 0;

    if (a0 > max_val) {
        r0 = max_val;
        *ov = 1;
    } else if (a0 < min_val) {
        r0 = min_val;
        *ov = 1;
    }

    if (a1 > max_val) {
        r1 = max_val;
        *ov = 1;
    } else if (a1 < min_val) {
        r1 = min_val;
        *ov = 1;
    }

    if (a2 > max_val) {
        r2 = max_val;
        *ov = 1;
    } else if (a2 < min_val) {
        r2 = min_val;
        *ov = 1;
    }

    if (a3 > max_val) {
        r3 = max_val;
        *ov = 1;
    } else if (a3 < min_val) {
        r3 = min_val;
        *ov = 1;
    }

    return  ((uint32_t)(uint8_t)r3 << 24) |
            ((uint32_t)(uint8_t)r2 << 16) |
            ((uint32_t)(uint8_t)r1 << 8)  |
            (uint32_t)(uint8_t)r0;
}

// Perform a single sclip8 test with random inputs
// Returns 1 if test passed, 0 otherwise
int run_single_sclip8_test(int test_num) {
    uint32_t a = ((uint32_t)rand() << 16) | (uint32_t)rand();
    uint8_t imm = 6; // 0–15 range

    uint32_t expected;
    uint8_t expected_ov = 0;
    uint32_t result;
    uint8_t result_ov = 0;

    expected = sclip8_sw(a, imm, &expected_ov);

    // In this placeholder, we assume OV is returned in a flag register or memory
    // You’ll need to adapt this to how your hardware exposes the OV bit.
    __asm__ volatile (
        "sclip8 %0, %1, 6"
        //"csrr %3, ov" // Placeholder: capture OV flag from CSR (if your arch supports this)
        : "=r" (result) //, "=r" (actual_ov)
        : "r" (a)
    );

    int pass = (result == expected); //&& (actual_ov == expected_ov);

    char a_bit[33], result_bit[33], expected_bit[33];
    int32_to_binary_str(a, a_bit);
    int32_to_binary_str(result, result_bit);
    int32_to_binary_str(expected, expected_bit);

    printf("Test %3d: %-10s %s (\n\r"
        "Result_b0 : %4d (signed)\n\r"
        "Result_b1 : %4d (signed)\n\r"
        "Result_b2 : %4d (signed)\n\r"
        "Result_b3 : %4d (signed)\n\r"
        "Result_bit: %s,\n\r"
        "Expected_b0 : %4d (signed)\n\r"
        "Expected_b1 : %4d (signed)\n\r"
        "Expected_b2 : %4d (signed)\n\r"
        "Expected_b3 : %4d (signed)\n\r"
        "Expected_bit: %s,\n\r"
        "a_b0 : %4d (signed)\n\r"
        "a_b1 : %4d (signed)\n\r"
        "a_b2 : %4d (signed)\n\r"
        "a_b3 : %4d (signed)\n\r"
        "a_bit = %s,\n\r"
        "max = %2d,\n\r"
        "min = %2d)\n\r",
        test_num, "sclip8", pass ? "PASS" : "FAIL",
        (int8_t)((result >> 0) & 0xFF),
        (int8_t)((result >> 8) & 0xFF),
        (int8_t)((result >> 16) & 0xFF),
        (int8_t)((result >> 24) & 0xFF),
        result_bit,
        (int8_t)((expected >> 0) & 0xFF),
        (int8_t)((expected >> 8) & 0xFF),
        (int8_t)((expected >> 16) & 0xFF),
        (int8_t)((expected >> 24) & 0xFF),
        expected_bit,
        (int8_t)((a >> 0) & 0xFF),
        (int8_t)((a >> 8) & 0xFF),
        (int8_t)((a >> 16) & 0xFF),
        (int8_t)((a >> 24) & 0xFF),
        a_bit, (1 << imm) - 1, -(1 << imm));

    return pass;
}

#endif
