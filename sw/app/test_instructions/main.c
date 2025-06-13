// main.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "test_sra16.h"
#include "test_srai16.h"
#include "test_sra16u.h"
#include "test_srai16u.h"
#include "print_bits.h"

int main() {
    srand((unsigned int)time(NULL));

    int pass_count_sra16 = 0;
    printf("Testing sra16:\n\r");
    for (int i = 1; i <= 100; i++) {
        pass_count_sra16 += run_single_sra16_test(i);
    }
    printf("sra16 summary: %d/%d tests passed.\n\r", pass_count_sra16, 100);

    // int pass_count_srai16 = 0;
    // printf("Testing srai16:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_srai16 += run_single_srai16_test(i);
    // }
    // printf("srai16 summary: %d/%d tests passed.\n\r", pass_count_srai16, 100);

    int pass_count_sra16u = 0;
    printf("Testing sra16.u:\n\r");
    for (int i = 1; i <= 100; i++) {
        pass_count_sra16u += run_single_sra16_u_test(i);
    }
    printf("srai16.u summary: %d/%d tests passed.\n\r", pass_count_sra16u, 100);

    // int pass_count_srai16u = 0;
    // printf("Testing srai16.u:\n\r");
    // for (int i = 1; i <= 100; i++) {
    //     pass_count_srai16u += run_single_srai16_u_test(i);
    // }
    // printf("srai16.u summary: %d/%d tests passed.\n\r", pass_count_srai16u, 100);

    return 0;
}
