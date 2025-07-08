#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

void int32_to_binary_str(uint32_t value, char* out_str) {
    for (int i = 31; i >= 0; i--) {
        out_str[31 - i] = (value & (1u << i)) ? '1' : '0';
    }
    out_str[32] = '\0'; // Null-terminate the string
}

int32_t sign_extend_17(int32_t val) {
    return (val & (1 << 16)) ? (val | ~0x1FFFF) : val;
}

int32_t sign_extend_9(int32_t val) {
    return (val & (1 << 8)) ? (val | ~0x1FF) : val;
}

int16_t saturate_16(int32_t val) {
    // printf("after shift: %d\r\n", val);
    if (val > 32767) return 32767;
    if (val < -32768) return -32768;
    return (int16_t)val;
}

int8_t saturate_8(int32_t val) {
    // printf("after shift: %d\r\n", val);
    if (val > 127) return 127;
    if (val < -128) return -128;
    return (int8_t)val;
}