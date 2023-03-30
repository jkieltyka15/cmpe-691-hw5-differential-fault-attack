/**
 * File: aes-sbox.c
 *
 * Implementation of sbox functions.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "aes-sbox.h"

#define BUFFER_SIZE 1024

#define SBOX_ROW_LEN 16
#define SBOX_COL_LEN 16

uint8_t sbox[SBOX_ROW_LEN * SBOX_COL_LEN];

bool sbox_load(char* file_path) {

    char buffer[BUFFER_SIZE];
    uint32_t sbox_index = 0;

    // open sbox file
    FILE* sbox_file = fopen(file_path, "r");
    if (NULL == sbox) {
        perror("failed to open sbox file");
        return false;
    }

    // load sbox
    for (uint32_t i = 0; SBOX_ROW_LEN > i; i++) {

        // read line of sbox file
        if (NULL == fgets(buffer, sizeof(buffer), sbox_file) && !feof(sbox_file)) {
            perror("failed to read sbox file line");
            return false;
        }

        // load buffer to current sbox row
        if (SBOX_COL_LEN != sscanf(buffer,
            "%hhx %hhx %hhx %hhx %hhx %hhx %hhx %hhx %hhx %hhx %hhx %hhx %hhx %hhx %hhx %hhx",
            &sbox[sbox_index], &sbox[sbox_index + 1], &sbox[sbox_index + 2], &sbox[sbox_index + 3],
            &sbox[sbox_index + 4], &sbox[sbox_index + 5], &sbox[sbox_index + 6], &sbox[sbox_index + 7],
            &sbox[sbox_index + 8], &sbox[sbox_index + 9], &sbox[sbox_index + 10], &sbox[sbox_index + 11],
            &sbox[sbox_index + 12], &sbox[sbox_index + 13], &sbox[sbox_index + 14], &sbox[sbox_index + 15])) {

            perror("failed to parse sbox line");
            return false;
        }
        sbox_index += SBOX_COL_LEN;
    }

    return true;
}

uint8_t sbox_get(uint8_t byte) {
    return sbox[byte];
}