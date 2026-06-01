#include <revolution/nwc24.h>

void NWC24Data_Init(NWC24Data* data) {
    data->ptr = 0;
    data->size = 0;
}

void NWC24Data_SetDataP(NWC24Data* data, const void* ptr, u32 size) {
    data->ptr = ptr;
    data->size = size;
}

void NWC24Date_Init(NWC24Date* date) {
    date->year = 2000;
    date->month = 1;
    date->day = 1;
    date->hour = 12;
    date->min = 0;
    date->sec = 0;
    date->BYTE_0x7 = 0;
}
