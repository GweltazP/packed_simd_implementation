#ifndef TEST_KABS16_H
#define TEST_KABS16_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "print_bits.h"

// Software reference model for KABS16
uint32_t kabs16_sw(uint32_t rs1, uint8_t* ov) {
    int16_t a0 = (int16_t)(rs1 & 0xFFFF);
    int16_t a1 = (int16_t)((rs1 >> 16) & 0xFFFF);

    *ov = 0;

    uint16_t r0 = (a0 == -32768) ? (*ov = 1, 32767) : (uint16_t)(a0 < 0 ? -a0 : a0);
    uint16_t r1 = (a1 == -32768) ? (*ov = 1, 32767) : (uint16_t)(a1 < 0 ? -a1 : a1);

    return ((uint32_t)r1 << 16) | (uint32_t)r0;
}

// Perform a single KABS16 test with random input
int run_single_kabs16_test(int test_num) {
    uint32_t a = ((uint32_t)rand() << 16) | (uint32_t)rand();

    uint32_t expected;
    uint8_t expected_ov = 0;
    uint32_t result;
    uint8_t result_ov = 0;

    expected = kabs16_sw(a, &expected_ov);

    // Replace with real hardware execution if supported
    __asm__ volatile (
        "kabs16 %0, %1"
        : "=r" (result)
        : "r" (a)
    );

    int pass = (result == expected); // Add OV comparison when hardware flag is accessible

    char a_bit[33], result_bit[33], expected_bit[33];
    int32_to_binary_str(a, a_bit);
    int32_to_binary_str(result, result_bit);
    int32_to_binary_str(expected, expected_bit);

    // printf("Test %3d: %-10s %s (\n\r"
    //        "Result_lo : %6u (abs)\n\r"
    //        "Result_hi : %6u (abs)\n\r"
    //        "Result_bit: %s,\n\r"
    //        "Expected_lo: %6u (abs)\n\r"
    //        "Expected_hi: %6u (abs)\n\r"
    //        "Expected_bit: %s,\n\r"
    //        "a_lo : %6d (signed)\n\r"
    //        "a_hi : %6d (signed)\n\r"
    //        "a_bit = %s,\n\r"
    //        "OV = %d)\n\r",
    //        test_num, "kabs16", pass ? "PASS" : "FAIL",
    //        (uint16_t)(result & 0xFFFF),
    //        (uint16_t)((result >> 16) & 0xFFFF),
    //        result_bit,
    //        (uint16_t)(expected & 0xFFFF),
    //        (uint16_t)((expected >> 16) & 0xFFFF),
    //        expected_bit,
    //        (int16_t)(a & 0xFFFF),
    //        (int16_t)((a >> 16) & 0xFFFF),
    //        a_bit,
    //        expected_ov);

    return pass;
}

#endif
