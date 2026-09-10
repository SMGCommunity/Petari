#include <revolution/nwc24.h>
#include <revolution/nwc24/NWC24Internal.h>

void NWC24Data_Init(NWC24Data* pData) {
    pData->pData = NULL;
    pData->size = 0;
}

void NWC24Data_SetDataP(NWC24Data* pData, const void* pPtr, u32 size) {
    pData->pData = pPtr;
    pData->size = size;
}

void NWC24Date_Init(NWC24Date* pDate) {
    pDate->year = 2000;
    pDate->month = 1;
    pDate->day = 1;
    pDate->hour = 12;
    pDate->min = 0;
    pDate->sec = 0;
    pDate->BYTE_0x7 = 0;
}

void NWC24iConvIdToStr(u64 addr, char* pBuffer) {
    u64 temp = addr;
    int i;

    for (i = NWC24i_WII_ID_LEN - 1; i >= 0; i--) {
        pBuffer[i] = '0' + temp % 10;
        temp /= 10;
    }

    pBuffer[NWC24i_WII_ID_LEN] = '\0';
}
