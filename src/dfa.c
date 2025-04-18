#include <stdio.h>
#include <stdint.h>

#include "aes-crack.h"
#include "aes-sbox.h"
#include "mvalue.h"

#define COL_LEN  4
#define ROW_LEN  4

#define NUM_OF_FAULTY 3
#define NUM_OF_KEYS   11
#define NUM_OF_FAULTS 8

#define MAX_BYTE_VAL 255 

#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {

    char* sbox_file_path = NULL;
    char* input_file_path = NULL;
    char* output_file_path = NULL;

    FILE* input_file = NULL;
    FILE* output_file = NULL;

    uint8_t ciphertext[ROW_LEN][COL_LEN];
    uint8_t ciphertext_f[NUM_OF_FAULTY][ROW_LEN][COL_LEN];

    char buffer[BUFFER_SIZE];
    uint32_t buffer_index = 0;

    uint8_t mvalue_matched[ROW_LEN][COL_LEN];
    uint8_t key[NUM_OF_KEYS][ROW_LEN][COL_LEN];
    uint8_t rc = 128;

    char byte_as_str[3];

    // check for correct number of arguments
    if (4 != argc) 
    {
        perror("Incorrect number of inputs. Expecting: \'./dfa <sbox file path> <input file path> <output file path>\'");
        return 1;
    }

    sbox_file_path = argv[1];
    input_file_path = argv[2];
    output_file_path = argv[3];
       
    // open input file
    input_file = fopen(input_file_path, "r");
    if (NULL == input_file) {
        perror("failed to open input file");
        return 1;
    }

    // open output file
    output_file = fopen(output_file_path, "w");
    if (NULL == input_file) {
        perror("failed to open output file");
        fclose(input_file);
        return 1;
    }

    // read line of input file with fault free ciphertext
    if (NULL == fgets(buffer, sizeof(buffer), input_file) && !feof(input_file)) {
        perror("failed to read input file line for fault free ciphertext");
        return 1;
    }

    // parse line of input file with fault free ciphertext
    buffer_index = 0;
    for (int32_t j = 0; COL_LEN > j; j++) {
    
        for (int32_t i = 0; ROW_LEN > i; i++) {
        
            byte_as_str[0] = buffer[buffer_index++];
            byte_as_str[1] = buffer[buffer_index++];
            byte_as_str[2] = '\0';
            if(1 != sscanf(byte_as_str, "%hhx", &ciphertext[i][j])) {
                perror("failed to parse hex value");
                return 1;
            }
        }
    }

    // retrieve faulty cipher text
    for (int32_t z = 0; NUM_OF_FAULTY > z; z++) {

        // read line of input file with faulty ciphertext
        if (NULL == fgets(buffer, sizeof(buffer), input_file) && !feof(input_file)) {
            perror("failed to read input file line for faulty ciphertext");
            return 1;
        }

        // parse line of input file with faulty ciphertext
        buffer_index = 0;
        for (int32_t j = 0; COL_LEN > j; j++) {
    
            for (int32_t i = 0; ROW_LEN > i; i++) {

                uint8_t hex = 0;
        
                byte_as_str[0] = buffer[buffer_index++];
                byte_as_str[1] = buffer[buffer_index++];
                byte_as_str[2] = '\0';
                if(1 != sscanf(byte_as_str, "%hhx", &hex)) {
                    perror("failed to parse hex value");
                    return 1;
                }
                ciphertext_f[z][i][j] = hex;
            }
        }
    }

    // load sbox
    if (0 == sbox_load(sbox_file_path)) {
        perror("failed to load sbox");
        return 1;
    }

    // discover mvalues for each faulty ciphertext
    for (uint32_t i = 0; ROW_LEN > i; i++) {
        for (uint32_t j = 0; COL_LEN > j; j++) {

            mvalue_t mvalue[NUM_OF_FAULTY];
            uint8_t fault = 1;

            // initialize mvalues
            for (uint32_t y = 0; NUM_OF_FAULTY > y; y++) {
                mvalue_init(&mvalue[y]);
            }

            // calculate XORs of faulty and non-faulty ciphertexts
            uint32_t c_xor_d[NUM_OF_FAULTY];
            for (uint32_t y = 0; NUM_OF_FAULTY > y; y++) {
                c_xor_d[y] = ciphertext[i][j] ^ ciphertext_f[y][i][j];
            }

            for (uint32_t z = 0; 8 > z; z++) {

                // find mvalue
                uint32_t value = 0;
                for (uint32_t value_32 = 0; MAX_BYTE_VAL >= value_32; value_32++) {
                    for (uint32_t y = 0; NUM_OF_FAULTY > y; y++) {
                        if (c_xor_d[y] == (sbox_get(value) ^ sbox_get(value ^ fault))) {
                            mvalue_add(&mvalue[y], value);
                        }
                    }
                    value += 1;
                }

                // get next fault value
                fault <<= 1;
            }

            // find intersection to determine key byte
            for (uint32_t a = 0; mvalue[0].length > a; a++) {
                uint32_t key_byte_found = 0;
                for (uint32_t b = 0; mvalue[1].length > b && !key_byte_found; b++) {
                    for (uint32_t c = 0; mvalue[2].length > c && !key_byte_found; c++) {
                        if (mvalue[0].value[a] == mvalue[1].value[b]
                            && mvalue[1].value[b] == mvalue[2].value[c]) {
                                mvalue_matched[i][j] = sbox_get(mvalue[0].value[a]);
                                key_byte_found = 1;
                        }
                    }
                }
                if (key_byte_found) {
                    break;
                }
            }
        }
    }

    // perform AES rowshift
    aes_inverse_row_shift(COL_LEN, ROW_LEN, mvalue_matched);

    // perform xor with ciphertext
    for (uint32_t i = 0; COL_LEN > i; i++) {
        for (uint32_t j = 0; ROW_LEN > j; j++) {
            key[NUM_OF_KEYS - 1][i][j] = mvalue_matched[i][j] ^ ciphertext[i][j]; 
        }
    }

    // perform the inverse AES key scheduler
    aes_inverse_key_scheduler(ROW_LEN, COL_LEN, 54, key[NUM_OF_KEYS - 1], key[NUM_OF_KEYS - 2]);
    for (int32_t round = NUM_OF_KEYS - 3; 0 <= round; round--) {
        aes_inverse_key_scheduler(ROW_LEN, COL_LEN, rc, key[round + 1], key[round]);
        rc >>= 1;
    }

    // print keys to file
    for (uint32_t round = 0; NUM_OF_KEYS > round; round++) {
        fprintf(output_file, "round %u key: ", round);
        for (uint32_t j = 0; COL_LEN > j; j++) {
            for (uint32_t i = 0; ROW_LEN > i; i++) {
                fprintf(output_file, "%.2hhx", key[round][i][j]);
            }
        }
        fprintf(output_file, "\n");
    }

    // cleanup
    fclose(input_file);
    fclose(output_file);

    return 0;
}