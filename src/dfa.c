#include <stdio.h>
#include <stdint.h>

#include "aes-crack.h"

#define COL_SIZE  4
#define ROW_SIZE  4

#define NUM_OF_FAULTY 3

#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {

    char* sbox_file_path = NULL;
    char* input_file_path = NULL;
    char* output_file_path = NULL;

    FILE* input_file = NULL;
    FILE* output_file = NULL;

    uint8_t ciphertext[ROW_SIZE][COL_SIZE];
    uint8_t ciphertext_f[NUM_OF_FAULTY][ROW_SIZE][COL_SIZE];

    char buffer[BUFFER_SIZE];
    uint32_t buffer_index = 0;

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
    for (int32_t j = 0; COL_SIZE > j; j++) {
    
        for (int32_t i = 0; ROW_SIZE > i; i++) {
        
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
        for (int32_t j = 0; COL_SIZE > j; j++) {
    
            for (int32_t i = 0; ROW_SIZE > i; i++) {

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

    // cleanup
    fclose(input_file);
    fclose(output_file);

    return 0;
}