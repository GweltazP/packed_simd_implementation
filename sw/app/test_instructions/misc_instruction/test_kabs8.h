#ifndef TEST_KABS8_H
#define TEST_KABS8_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "print_bits.h"

// Software reference model for KABS8
// Calculates absolute value of 8-bit signed integers packed in rs1
// Returns packed result in rd, sets *ov if any input == -128 (0x80)
uint32_t kabs8_sw(uint32_t rs1, uint8_t* ov) {
    int8_t a0 = (int8_t)(rs1 & 0xFF);
    int8_t a1 = (int8_t)((rs1 >> 8) & 0xFF);
    int8_t a2 = (int8_t)((rs1 >> 16) & 0xFF);
    int8_t a3 = (int8_t)((rs1 >> 24) & 0xFF);

    *ov = 0;

    uint8_t r0 = (a0 == -128) ? (*ov = 1, 127) : (uint8_t)(a0 < 0 ? -a0 : a0);
    uint8_t r1 = (a1 == -128) ? (*ov = 1, 127) : (uint8_t)(a1 < 0 ? -a1 : a1);
    uint8_t r2 = (a2 == -128) ? (*ov = 1, 127) : (uint8_t)(a2 < 0 ? -a2 : a2);
    uint8_t r3 = (a3 == -128) ? (*ov = 1, 127) : (uint8_t)(a3 < 0 ? -a3 : a3);

    return  ((uint32_t)r3 << 24) |
            ((uint32_t)r2 << 16) |
            ((uint32_t)r1 << 8)  |
            (uint32_t)r0;
}

// Perform a single KABS8 test with random input
int run_single_kabs8_test(int test_num) {
    uint32_t a = ((uint32_t)rand() << 16) | (uint32_t)rand();

    uint32_t expected;
    uint8_t expected_ov = 0;
    uint32_t result;
    uint8_t result_ov = 0;

    expected = kabs8_sw(a, &expected_ov);

    // Replace this with actual hardware implementation if available
    __asm__ volatile (
        "kabs8 %0, %1"
        : "=r" (result)
        : "r" (a)
    );

    int pass = (result == expected); // Extend this to compare OV flags when hardware support is ready

    char a_bit[33], result_bit[33], expected_bit[33];
    int32_to_binary_str(a, a_bit);
    int32_to_binary_str(result, result_bit);
    int32_to_binary_str(expected, expected_bit);

    // printf("Test %3d: %-10s %s (\n\r"
    //     "Result_b0 : %4u (abs)\n\r"
    //     "Result_b1 : %4u (abs)\n\r"
    //     "Result_b2 : %4u (abs)\n\r"
    //     "Result_b3 : %4u (abs)\n\r"
    //     "Result_bit: %s,\n\r"
    //     "Expected_b0 : %4u (abs)\n\r"
    //     "Expected_b1 : %4u (abs)\n\r"
    //     "Expected_b2 : %4u (abs)\n\r"
    //     "Expected_b3 : %4u (abs)\n\r"
    //     "Expected_bit: %s,\n\r"
    //     "a_b0 : %4d (signed)\n\r"
    //     "a_b1 : %4d (signed)\n\r"
    //     "a_b2 : %4d (signed)\n\r"
    //     "a_b3 : %4d (signed)\n\r"
    //     "a_bit = %s,\n\r"
    //     "OV = %d)\n\r",
    //     test_num, "kabs8", pass ? "PASS" : "FAIL",
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
    //     a_bit,
    //     expected_ov);

    return pass;
}

#endif
