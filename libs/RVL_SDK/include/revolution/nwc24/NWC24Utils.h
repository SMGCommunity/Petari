#ifndef RVL_SDK_NWC24_UTILS_H
#define RVL_SDK_NWC24_UTILS_H
#include <revolution/types.h>
#include <cstdlib>

#include <revolution/nwc24/NWC24Types.h>
#ifdef __cplusplus
extern "C" {
#endif

void NWC24Data_Init(NWC24Data* pData);
void NWC24Data_SetDataP(NWC24Data* pData, const void* pPtr, u32 size);
void NWC24Date_Init(NWC24Date* pDate);

#ifdef __cplusplus
}
#endif
#endif
