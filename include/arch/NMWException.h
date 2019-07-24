#ifndef NMWEXCEPTION_H
#define NMWEXCEPTION_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

void* __construct_new_array(void *block, void *ctor, void *dtor, u32, u32);

#ifdef __cplusplus
}
#endif

#endif // NMWEXCEPTION_H