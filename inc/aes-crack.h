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

#endif // _AES_CRACK_H_