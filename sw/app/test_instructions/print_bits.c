#include <stdio.h>
#include <stdint.h>

void int32_to_binary_str(uint32_t value, char* out_str) {
    for (int i = 31; i >= 0; i--) {
        out_str[31 - i] = (value & (1u << i)) ? '1' : '0';
    }
    out_str[32] = '\0'; // Null-terminate the string
}

int32_t sign_extend_17(int32_t val) {
    return (val & (1 << 16)) ? (val | ~0x1FFFF) : val;
}