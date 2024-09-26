#ifndef RVL_SDK_NWC24_UTILS_H
#define RVL_SDK_NWC24_UTILS_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct NWC24Data {
    const void* ptr; // at 0x0
    u32 size;        // at 0x4
} NWC24Data;

typedef struct NWC24Date {
    u16 year; // at 0x0
    u8 month; // at 0x2
    u8 day;   // at 0x3
    u8 hour;  // at 0x4
    u8 sec;   // at 0x5
    u8 min;   // at 0x6
    u8 BYTE_0x7;
} NWC24Date;

void NWC24Data_Init(NWC24Data* data);
void NWC24Data_SetDataP(NWC24Data* data, const void* ptr, u32 size);
void NWC24Date_Init(NWC24Date* date);
void NWC24iConvIdToStr(u64 addr, char* out);

#ifdef __cplusplus
}
#endif
#endif