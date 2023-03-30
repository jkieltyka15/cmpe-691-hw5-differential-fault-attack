/**
 * File: aes-crack.c
 *
 * Implementation of functions useful for cracking AES.
 */

#include "aes-crack.h"
#include "aes-sbox.h"

/**
 * Perform the inverse g function for the AES key scheduler.
 * 
 * @param rc - RC value for the current round
 * @param len - length of input and output column
 * @param array - array to perform inverse g operation on
 */
void aes_inverse_g(uint8_t rc, uint32_t len, uint8_t array[len]) {
    
    uint8_t tmp = 0;

    // xor with RC
    array[0] ^= rc;

    // inverse sbox
    for (uint32_t i; len > i; i++) {
        array[i] = inverse_sbox_get(array[i]);
    }

    // circular right shift
    tmp = array[len - 1];
    for (uint32_t i = len - 1; 0 < i; i--) {
        array[i] = array[i - 1];
    }
    array[0] = tmp;
}

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