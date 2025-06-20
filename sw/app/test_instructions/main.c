// main.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "shift_16bits/shift_16bits.h"
#include "shift_8bits/shift_8bits.h"
#include "misc_instruction/misc_instruction.h"
#include "print_bits.h"

int main() {
    srand((unsigned int)time(NULL));

    //TEST Inctruction SHIFT 16 bits

    int pass_count_sra16 = 0;
    printf("Testing sra16:\n\r");
    for (int i = 1; i <= 100; i++) {
        pass_count_sra16 += run_single_sra16_test(i);
    }
    printf("sra16 summary: %d/%d tests passed.\n\r", pass_count_sra16, 100);

    int pass_count_srai16 = 0;
    printf("Testing srai16:\n\r");
    for (int i = 1; i <= 100; i++) {
        pass_count_srai16 += run_single_srai16_test(i);
    }
    printf("srai16 summary: %d/%d tests passed.\n\r", pass_count_srai16, 100);

    // int pass_count_sra16u = 0;
    // printf("Testing sra16.u:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_sra16u += run_single_sra16u_test(i);
    // }
    // printf("sra16.u summary: %d/%d tests passed.\n\r", pass_count_sra16u, 100);

    // int pass_count_srai16u = 0;
    // printf("Testing srai16.u:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_srai16u += run_single_srai16u_test(i);
    // }
    // printf("srai16.u summary: %d/%d tests passed.\n\r", pass_count_srai16u, 100);

    // int pass_count_srl16 = 0;
    // printf("Testing srl16:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_srl16 += run_single_srl16_test(i);
    // }
    // printf("srl16 summary: %d/%d tests passed.\n\r", pass_count_srl16, 100);

    // int pass_count_srli16 = 0;
    // printf("Testing srli16:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_srli16 += run_single_srli16_test(i);
    // }
    // printf("srli16 summary: %d/%d tests passed.\n\r", pass_count_srli16, 100);

    // int pass_count_srl16u = 0;
    // printf("Testing srl16.u:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_srl16u += run_single_srl16u_test(i);
    // }
    // printf("srl16.u summary: %d/%d tests passed.\n\r", pass_count_srl16u, 100);

    // int pass_count_srli16u = 0;
    // printf("Testing srli16.u:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_srli16u += run_single_srli16u_test(i);
    // }
    // printf("srli16.u summary: %d/%d tests passed.\n\r", pass_count_srli16u, 100);

    // int pass_count_sll16 = 0;
    // printf("Testing sll16:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_sll16 += run_single_sll16_test(i);
    // }
    // printf("sll16 summary: %d/%d tests passed.\n\r", pass_count_sll16, 100);

    // int pass_count_slli16 = 0;
    // printf("Testing slli16:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_slli16 += run_single_slli16_test(i);
    // }
    // printf("slli16 summary: %d/%d tests passed.\n\r", pass_count_slli16, 100);

    // int pass_count_ksll16 = 0;
    // printf("Testing ksll16:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_ksll16 += run_single_ksll16_test(i);
    // }
    // printf("ksll16 summary: %d/%d tests passed.\n\r", pass_count_ksll16, 100);

    // int pass_count_kslli16 = 0;
    // printf("Testing kslli16:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_kslli16 += run_single_kslli16_test(i);
    // }
    // printf("kslli16 summary: %d/%d tests passed.\n\r", pass_count_kslli16, 100);

    // //TEST Inctruction SHIFT 16 bits

    // int pass_count_sra8 = 0;
    // printf("Testing sra8:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_sra8 += run_single_sra8_test(i);
    // }
    // printf("sra8 summary: %d/%d tests passed.\n\r", pass_count_sra8, 100);

    // int pass_count_srai8 = 0;
    // printf("Testing srai8:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_srai8 += run_single_srai8_test(i);
    // }
    // printf("srai8 summary: %d/%d tests passed.\n\r", pass_count_srai8, 100);

    // int pass_count_sra8u = 0;
    // printf("Testing sra8.u:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_sra8u += run_single_sra8u_test(i);
    // }
    // printf("sra8.u summary: %d/%d tests passed.\n\r", pass_count_sra8u, 100);

    // int pass_count_srai8u = 0;
    // printf("Testing srai8.u:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_srai8u += run_single_srai8u_test(i);
    // }
    // printf("srai8.u summary: %d/%d tests passed.\n\r", pass_count_srai8u, 100);

    // int pass_count_srl8 = 0;
    // printf("Testing srl8:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_srl8 += run_single_srl8_test(i);
    // }
    // printf("srl8 summary: %d/%d tests passed.\n\r", pass_count_srl8, 100);

    // int pass_count_srli8 = 0;
    // printf("Testing srli8:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_srli8 += run_single_srli8_test(i);
    // }
    // printf("srli8 summary: %d/%d tests passed.\n\r", pass_count_srli8, 100);

    // int pass_count_srl8u = 0;
    // printf("Testing srl8.u:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_srl8u += run_single_srl8u_test(i);
    // }
    // printf("srl8.u summary: %d/%d tests passed.\n\r", pass_count_srl8u, 100);

    // int pass_count_srli8u = 0;
    // printf("Testing srli8.u:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_srli8u += run_single_srli8u_test(i);
    // }
    // printf("srli8.u summary: %d/%d tests passed.\n\r", pass_count_srli8u, 100);

    // int pass_count_sll8 = 0;
    // printf("Testing sll8:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_sll8 += run_single_sll8_test(i);
    // }
    // printf("sll8 summary: %d/%d tests passed.\n\r", pass_count_sll8, 100);

    // int pass_count_slli8 = 0;
    // printf("Testing slli8:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_slli8 += run_single_slli8_test(i);
    // }
    // printf("slli8 summary: %d/%d tests passed.\n\r", pass_count_slli8, 100);

    // int pass_count_ksll8 = 0;
    // printf("Testing ksll8:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_ksll8 += run_single_ksll8_test(i);
    // }
    // printf("ksll8 summary: %d/%d tests passed.\n\r", pass_count_ksll8, 100);

    // int pass_count_kslli8 = 0;
    // printf("Testing kslli8:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_kslli8 += run_single_kslli8_test(i);
    // }
    // printf("kslli8 summary: %d/%d tests passed.\n\r", pass_count_kslli8, 100);

    int pass_count_sclip16 = 0;
    printf("Testing sclip16:\n\r");
    for (int i = 1; i <= 100; i++) {
        pass_count_sclip16 += run_single_sclip16_test(i);
    }
    printf("sclip16 summary: %d/%d tests passed.\n\r", pass_count_sclip16, 100);

    int pass_count_sclip32 = 0;
    printf("Testing sclip32:\n\r");
    for (int i = 1; i <= 100; i++) {
        pass_count_sclip32 += run_single_sclip32_test(i);
    }
    printf("sclip32 summary: %d/%d tests passed.\n\r", pass_count_sclip32, 100);


    return 0;
}
