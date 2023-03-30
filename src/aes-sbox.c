#include <stdint.h>
#include <stdbool.h>

#include "aes-sbox.h"

uint8_t sbox[256];


bool sbox_load(char* file_path) {
    return false;
}

uint8_t sbox_get(uint8_t byte) {
    return sbox[byte];
}