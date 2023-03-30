#ifndef _AES_S_BOX_H_
#define _AES_S_BOX_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * Loads values into the sbox from a text file.
 * 
 * @param file_path - The file path for the sbox text file.
 * @return true on success, otherwise false. 
 */
bool sbox_load(char* file_path);

/**
 * Gets the sbox value of the input byte.
 * 
 * @param byte - The input byte used to get the sbox value.
 * @return The sbox value based on the input byte. 
 */
uint8_t sbox_get(uint8_t byte);

#endif // _AES_S_BOX_H_