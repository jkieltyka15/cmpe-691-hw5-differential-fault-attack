/**
 * File: aes-array.c
 *
 * Implementation of array functions.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "aes-sbox.h"

#define BUFFER_SIZE 1024

#define SBOX_ROW_LEN 16
#define SBOX_COL_LEN 16

uint8_t sbox[SBOX_ROW_LEN * SBOX_COL_LEN];

/**
 * Loads an array that is the length of an sbox from a text file.
 * 
 * @param file_path - File path used to load the array
 * @param array - Array to load values into
 * 
 * @return True on success, otherwise false
 */
bool array_load(char* file_path, uint8_t array[SBOX_ROW_LEN * SBOX_COL_LEN]) {

    char buffer[BUFFER_SIZE];
    uint32_t array_index = 0;

    // open array file
    FILE* array_file = fopen(file_path, "r");
    if (NULL == array) {
        perror("failed to open array file");
        return false;
    }

    // load array
    for (uint32_t i = 0; SBOX_ROW_LEN > i; i++) {

        // read line of array file
        if (NULL == fgets(buffer, sizeof(buffer), array_file) && !feof(array_file)) {
            perror("failed to read array file line");
            return false;
        }

        // load buffer to current array row
        if (SBOX_COL_LEN != sscanf(buffer,
            "%hhx %hhx %hhx %hhx %hhx %hhx %hhx %hhx %hhx %hhx %hhx %hhx %hhx %hhx %hhx %hhx",
            &array[array_index], &array[array_index + 1], &array[array_index + 2], &array[array_index + 3],
            &array[array_index + 4], &array[array_index + 5], &array[array_index + 6], &array[array_index + 7],
            &array[array_index + 8], &array[array_index + 9], &array[array_index + 10], &array[array_index + 11],
            &array[array_index + 12], &array[array_index + 13], &array[array_index + 14], &array[array_index + 15])) {

            perror("failed to parse array line");
            return false;
        }
        array_index += SBOX_COL_LEN;
    }

    return true;
}

bool sbox_load(char* file_path) {
    return array_load(file_path, sbox);
}

uint8_t sbox_get(uint8_t byte) {
    return sbox[byte];
}