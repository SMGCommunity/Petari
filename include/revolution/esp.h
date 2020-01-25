#ifndef ESP_H
#define ESP_H

#include "types.h"

s32 ESP_InitLib();
s32 ESP_CloseLib();

static s32 __esFd = 0xFFFFFFFF;

#endif // ESP_H