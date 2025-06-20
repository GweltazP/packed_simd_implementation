#ifndef TEST_SCLIP32_H
#define TEST_SCLIP32_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "print_bits.h"

/*--------------------------------------------------------------------
 * Software reference model – SCLIP32
 *--------------------------------------------------------------------
 *  rs1      : 32‑bit signed input
 *  imm4u    : 0‑15   (we’ll use 7 in the test harness below)
 *  *ov      : overflow flag (set to 1 if clipping occurred)
 *  return   : clipped result (packed into a uint32_t for convenience)
 */
static inline uint32_t sclip32_sw(int32_t rs1, uint8_t imm4u, uint8_t *ov)
{
    int32_t max_val =  (1 << imm4u) - 1;   /*  2^imm − 1 */
    int32_t min_val = -(1 << imm4u);       /* −2^imm      */

    *ov = 0;
    if (rs1 > max_val) { rs1 = max_val; *ov = 1; }
    else if (rs1 < min_val) { rs1 = min_val; *ov = 1; }

    return (uint32_t)rs1;                  /* keep bit‑pattern intact */
}

/*--------------------------------------------------------------------
 * Single SCLIP32 test
 *-------------------------------------------------------------------*/
static inline int run_single_sclip32_test(int test_num)
{
    int32_t  a   =  (int32_t)rand();   /* random 32‑bit signed input   */
    uint8_t  imm = 15;                 /* clip range = [−128, 127]     */

    /* --- Reference model ----------------------------------------- */
    uint8_t  expected_ov = 0;
    uint32_t expected     = sclip32_sw(a, imm, &expected_ov);

    /* --- DUT: assembly version ----------------------------------- */
    int32_t  result;                  /* destination register */

    __asm__ volatile (
        "sclip32 %0, %1, 15"           /* imm is a *literal* 7          */
        : "=r" (result)
        : "r"  (a)
    );

    /* --- Pass / fail --------------------------------------------- */
    int pass = (result == (int32_t)expected);
    /* If you can read the OV flag in your environment, compare it
       with expected_ov here as well. */

    /* --- Pretty printing ----------------------------------------- */
    char a_bit[33], result_bit[33], expected_bit[33];
    int32_to_binary_str((uint32_t)a,        a_bit);
    int32_to_binary_str((uint32_t)result,   result_bit);
    int32_to_binary_str((uint32_t)expected, expected_bit);

    printf("Test %3d: %-8s %s (\n\r"
           "  result_bit    : %s\n\r"
           "  result_val    : %11d (signed)\n\r"
           "  expected_bit  : %s\n\r"
           "  expected_val  : %11d (signed)\n\r"
           "  a             : 0x%08X\n\r"
           "  a_bit         : %11d (signed)\n\r"
           "  clip max      : %11d\n\r"
           "  clip min      : %11d)\n\r",
           test_num, "sclip32", pass ? "PASS" : "FAIL",
           (uint32_t)result,   result_bit,   result,
           expected,          expected_bit, (int32_t)expected,
           (uint32_t)a,        a_bit,
           (1 << imm) - 1, -(1 << imm));

    return pass;
}

#endif /* TEST_SCLIP32_H */
