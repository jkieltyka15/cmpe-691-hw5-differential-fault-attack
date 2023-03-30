/**
 * File: aes-crack.h
 *
 * Prototypes of functions useful for cracking AES.
 */

#ifndef _AES_CRACK_H_
#define _AES_CRACK_H_

#include <stdint.h>

/**
 * Perform an inverse AES row shift.
 * 
 * @param rows - number of rows for the array
 * @param cols - number of columns for the array
 * @param text - array to perform inverse row shift on  
 */
void aes_inverse_row_shift(uint32_t rows, uint32_t cols, uint8_t text[rows][cols]);

/**
 * Perform the inverse AES key schedule operation for one round.
 * 
 * @param rows - number of rows for key
 * @param cols - number of columns for key
 * @param key - input key used to calculate key for previous round
 * @param prev_key - output key/previous round key
 *  
 */
void aes_inverse_key_scheduler(uint32_t rows,
    uint32_t cols, uint8_t key[rows][cols], uint8_t prev_key[rows][cols]);

#endif // _AES_CRACK_H_