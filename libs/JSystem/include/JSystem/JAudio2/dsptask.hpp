#pragma once

#include <revolution/types.h>

void DspBoot(void (*)(void*));
void DspFinishWork(u16 param_0);
int DSPSendCommands2(u32* pMessages, u32 param_1, void (*pCallback)(u16));
