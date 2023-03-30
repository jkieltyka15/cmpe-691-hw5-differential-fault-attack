
#include <stdint.h>

#include "mvalue.h"

int32_t mvalue_add(mvalue_t* mvalue, uint8_t value) {
    
    if (mvalue->length == MAX_MVALUES) {
        return -1;
    }
    mvalue->value[mvalue->length] = value;
    mvalue->length += 1;
    return (int32_t)mvalue->length;
}