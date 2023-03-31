/**
 * File: mvalue.h
 *
 * Implemenation of functions for mvalue.
 */

#include <stdint.h>

#include "mvalue.h"

void mvalue_init(mvalue_t* mvalue) {
    mvalue->length = 0;
}

int32_t mvalue_add(mvalue_t* mvalue, uint8_t value) {
    
    if (mvalue->length == MAX_MVALUES) {
        return -1;
    }

    // ensure only unique mvalues
    for (uint32_t i = 0; mvalue->length > i; i++) {
        if (value == mvalue->value[i]) {
            return (uint32_t)i;
        }
    }

    mvalue->value[mvalue->length] = value;
    mvalue->length += 1;
    return (int32_t)mvalue->length;
}