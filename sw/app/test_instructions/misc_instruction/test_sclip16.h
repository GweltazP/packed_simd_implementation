#ifndef TEST_SCLIP16_H
#define TEST_SCLIP16_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "print_bits.h"

// Software reference model for SCLIP16
// Returns packed result in rd, sets *ov if overflow occurred
uint32_t sclip16_sw(uint32_t rs1, uint8_t imm4u, uint8_t* ov) {
    int16_t a1 = (int16_t)(rs1 & 0xFFFF);
    int16_t a2 = (int16_t)((rs1 >> 16) & 0xFFFF);

    int16_t max_val = (1 << imm4u) - 1;
    int16_t min_val = -(1 << imm4u);

    int16_t r1 = a1;
    int16_t r2 = a2;

    *ov = 0;

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

    return ((uint16_t)r2 << 16) | ((uint16_t)r1);
}

// Perform a single SCLIP16 test with random inputs
// Returns 1 if test passed, 0 otherwise
int run_single_sclip16_test(int test_num) {
    uint32_t a = ((uint32_t)rand() << 16) | (uint32_t)rand();
    uint8_t imm = 11; // 0–15 range

    uint32_t expected;
    uint8_t expected_ov = 0;
    uint32_t result;
    uint8_t result_ov = 0;

    expected = sclip16_sw(a, imm, &expected_ov);

    // In this placeholder, we assume OV is returned in a flag register or memory
    // You’ll need to adapt this to how your hardware exposes the OV bit.
    __asm__ volatile (
        "sclip16 %0, %1, 11"
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
        "Result_lo   : %6d (signed)\n\r"
        "Result_hi   : %6d (signed)\n\r"
        "Result_bit: %s, \n\r"
        "Expected_lo : %6d (signed)\n\r"
        "Expected_hi : %6d (signed)\n\r"
        "Expected_bit: %s, \n\r"
        "a_lo : %6d (signed)\n\r"
        "a_hi : %6d (signed)\n\r"
        "a_bit = %s, \n\r"
        "max = %2d, \n\r"
        "min = %2d)\n\r",
        test_num, "sclip16", pass ? "PASS" : "FAIL",
        (int16_t)(result & 0xFFFF),
        (int16_t)((result >> 16) & 0xFFFF),
        result_bit,
        (int16_t)(expected & 0xFFFF),
        (int16_t)((expected >> 16) & 0xFFFF),
        expected_bit,
        (int16_t)(a & 0xFFFF),
        (int16_t)((a >> 16) & 0xFFFF),
        a_bit, (1 << imm) - 1, -(1 << imm));

    return pass;
}

#endif
