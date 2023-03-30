/**
 * File: aes-crack.c
 *
 * Implementation of functions useful for cracking AES.
 */

#include <string.h>

#include "aes-crack.h"
#include "aes-sbox.h"

/**
 * Perform an XOR on all elements of two byte array.
 * 
 * @param len - lenght of the arrays
 * @param xor_array - The result of the two arrays XORed. 
 * @param array_1 - first input array
 * @param array_2 - second input array
 */
void array_xor_array(uint32_t len, 
    uint8_t xor_array[len], uint8_t array_1[len], uint8_t array_2[len]) {

    for(int i = 0; len > i; i++) {
        xor_array[i] = array_1[i] ^ array_2[i];
    }
}

/**
 * Replaces columns with rows and vice versa.
 * 
 * @param rows - number of rows for src array
 * @param cols - number of columns for src array
 * @param dst - distination array for result of transpose
 * @param src - src array for input of transpose
 */
void transpose_2d_array(uint32_t rows, uint32_t cols, uint8_t* dst, uint8_t* src) {

    for(uint32_t i = 0; rows > i; i++){
        for(uint32_t j = 0; cols > j; j++){
            dst[j * rows + i] = src[i * cols + j];
        }
    }
}

/**
 * Perform the g function for the AES key scheduler.
 * 
 * @param rc - RC value for the current round
 * @param len - length of input and output column
 * @param array - array to perform inverse g operation on
 */
void aes_g(uint8_t rc, uint32_t len, uint8_t array[len]) {
    
    uint8_t tmp = 0;

    // circular left shift
    tmp = array[0];
    for (uint32_t i = 0; len - 1 > i; i++) {
        array[i] = array[i + 1];
    }
    array[len - 1] = tmp;

    // sbox
    for (uint32_t i; len > i; i++) {
        array[i] = sbox_get(array[i]);
    }

    // xor with RC
    array[0] ^= rc;
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

void aes_inverse_key_scheduler(uint32_t rows, uint32_t cols,
    uint8_t rc, uint8_t key[rows][cols], uint8_t prev_key[rows][cols]) {

    uint8_t key_transposed[cols][rows];
    uint8_t prev_key_transposed[cols][rows];

    // transpose the key so it is easier to operate on
    transpose_2d_array(rows, cols, &key_transposed[0][0], &key[0][0]);

    // calculate the remaining w's excluding w0
    for(uint32_t i = cols - 1; 0 < i; i--) {
        array_xor_array(cols, prev_key_transposed[i], key_transposed[i], key_transposed[i - 1]);
    }

    // calculate w0
    memcpy(prev_key_transposed[0], prev_key_transposed[cols - 1], cols);
    aes_g(rc, cols, prev_key_transposed[0]);
    array_xor_array(cols, prev_key_transposed[0], prev_key_transposed[0], key_transposed[0]);

    // transpose the transpose prev key to get prev key
    transpose_2d_array(cols, rows, &prev_key[0][0], &prev_key_transposed[0][0]);
}