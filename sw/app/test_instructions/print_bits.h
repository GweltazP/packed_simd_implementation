#ifndef PRINT_BITS_H
#define PRINT_BITS_H

#include <stdio.h>
#include <stdint.h>

void int32_to_binary_str(uint32_t value, char* out_str);
int32_t sign_extend_17(int32_t val);

#endif