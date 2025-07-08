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

    // int pass_count_sra16 = 0;
    // printf("Testing sra16:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_sra16 += run_single_sra16_test(i);
    // }
    // printf("sra16 summary: %d/%d tests passed.\n\r", pass_count_sra16, 100);

    // int pass_count_srai16 = 0;
    // printf("Testing srai16:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_srai16 += run_single_srai16_test(i);
    // }
    // printf("srai16 summary: %d/%d tests passed.\n\r", pass_count_srai16, 100);

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

    // int pass_count_kslra16 = 0;
    // printf("Testing kslra16:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_kslra16 += run_single_kslra16_test(i);
    // }
    // printf("kslra16 summary: %d/%d tests passed.\n\r", pass_count_kslra16, 100);

    // int pass_count_kslra16_u = 0;
    // printf("Testing kslra16_u:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_kslra16_u += run_single_kslra16_u_test(i);
    // }
    // printf("kslra16_u summary: %d/%d tests passed.\n\r", pass_count_kslra16_u, 100);

    // //TEST Inctruction SHIFT 8 bits

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

    // int pass_count_kslra8 = 0;
    // printf("Testing kslra8:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_kslra8 += run_single_kslra8_test(i);
    // }
    // printf("kslra8 summary: %d/%d tests passed.\n\r", pass_count_kslra8, 100);

    // int pass_count_kslra8_u = 0;
    // printf("Testing kslra8_u:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_kslra8_u += run_single_kslra8_u_test(i);
    // }
    // printf("kslra8_u summary: %d/%d tests passed.\n\r", pass_count_kslra8_u, 100);

    //TEST Inctruction MISC

    int pass_count_sclip16 = 0;
    printf("Testing sclip16:\n\r");
    for (int i = 1; i <= 100; i++) {
        pass_count_sclip16 += run_single_sclip16_test(i);
    }
    printf("sclip16 summary: %d/%d tests passed.\n\r", pass_count_sclip16, 100);

    int pass_count_sclip8 = 0;
    printf("Testing sclip8:\n\r");
    for (int i = 1; i <= 100; i++) {
        pass_count_sclip8 += run_single_sclip8_test(i);
    }
    printf("sclip8 summary: %d/%d tests passed.\n\r", pass_count_sclip8, 100);

    int pass_count_uclip16 = 0;
    printf("Testing uclip16:\n\r");
    for (int i = 1; i <= 100; i++) {
        pass_count_uclip16 += run_single_uclip16_test(i);
    }
    printf("uclip16 summary: %d/%d tests passed.\n\r", pass_count_uclip16, 100);

    int pass_count_uclip8 = 0;
    printf("Testing uclip8:\n\r");
    for (int i = 1; i <= 100; i++) {
        pass_count_uclip8 += run_single_uclip8_test(i);
    }
    printf("uclip8 summary: %d/%d tests passed.\n\r", pass_count_uclip8, 100);

    // int pass_count_kabs16 = 0;
    // printf("Testing kabs16:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_kabs16 += run_single_kabs16_test(i);
    // }
    // printf("kabs16 summary: %d/%d tests passed.\n\r", pass_count_kabs16, 100);

    // int pass_count_kabs8 = 0;
    // printf("Testing kabs8:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_kabs8 += run_single_kabs8_test(i);
    // }
    // printf("kabs8 summary: %d/%d tests passed.\n\r", pass_count_kabs8, 100);

    int pass_count_clrs16 = 0;
    printf("Testing clrs16:\n\r");
    for (int i = 1; i <= 100; i++) {
        pass_count_clrs16 += run_single_clrs16_test(i);
    }
    printf("clrs16 summary: %d/%d tests passed.\n\r", pass_count_clrs16, 100);

    int pass_count_clrs8 = 0;
    printf("Testing clrs8:\n\r");
    for (int i = 1; i <= 100; i++) {
        pass_count_clrs8 += run_single_clrs8_test(i);
    }
    printf("clrs8 summary: %d/%d tests passed.\n\r", pass_count_clrs8, 100);

    // int pass_count_clz16 = 0;
    // printf("Testing clz16:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_clz16 += run_single_clz16_test(i);
    // }
    // printf("clz16 summary: %d/%d tests passed.\n\r", pass_count_clz16, 100);

    // int pass_count_clz8 = 0;
    // printf("Testing clz8:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_clz8 += run_single_clz8_test(i);
    // }
    // printf("clz8 summary: %d/%d tests passed.\n\r", pass_count_clz8, 100);

    // int pass_count_clo16 = 0;
    // printf("Testing clo16:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_clo16 += run_single_clo16_test(i);
    // }
    // printf("clo16 summary: %d/%d tests passed.\n\r", pass_count_clo16, 100);

    // int pass_count_clo8 = 0;
    // printf("Testing clo8:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_clo8 += run_single_clo8_test(i);
    // }
    // printf("clo8 summary: %d/%d tests passed.\n\r", pass_count_clo8, 100);

    return 0;
}
