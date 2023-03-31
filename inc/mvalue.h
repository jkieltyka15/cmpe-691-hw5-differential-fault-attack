/**
 * File: mvalue.h
 *
 * Prototypes of functions for mvalue.
 */

#ifndef _MVALUE_H
#define _MVALUE_H

#include <stdint.h>

#define MAX_MVALUES 8

typedef struct
{
    uint32_t length;
    uint8_t value[MAX_MVALUES];
} mvalue_t;

/**
 * Intialize mvalue.
 * 
 * @param mvalue mvalue to be initialized 
 */
void mvalue_init(mvalue_t* mvalue);

/**
 * Add a value to the mvalue list.
 * 
 * @param value - value to be added
 * @return index of where value was added, otherwise -1 
 */
int32_t mvalue_add(mvalue_t* mvalue, uint8_t value);

#endif // _MVALUE_H