#ifndef PRINT_BITS_H
#define PRINT_BITS_H

#include <stdio.h>
#include <stdint.h>

void int32_to_binary_str(uint32_t value, char* out_str);
int32_t sign_extend_9(int32_t val);
int32_t sign_extend_17(int32_t val);
int8_t saturate_8(int32_t val);
int16_t saturate_16(int32_t val);

#endif