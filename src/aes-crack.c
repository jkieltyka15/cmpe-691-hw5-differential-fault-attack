/**
 * File: aes-crack.c
 *
 * Implementation of functions useful for cracking AES.
 */

#include "aes-crack.h"

void aes_inverse_row_shift(uint32_t rows, uint32_t cols, uint8_t text[rows][cols]) {

    for (uint32_t i = 0; rows > i; i++) {
        for (uint32_t shifts = 0; i > shifts; shifts++) {
            uint8_t tmp = text[i][cols - 1];
            for (uint32_t j = cols - 1; 0 < j; j--) {
                text[i][j] = text[i][j - 1];
            }
            text[i][0] = tmp;
        }
    }
}