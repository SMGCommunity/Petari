#pragma once

#include "Actor/Nerve/NerveExecutor.h"

#include <nw4r/lyt.h>
#include <revolution.h>
#include <__ppc_eabi_init.h>

#ifdef __cplusplus
extern "C" {
#endif

void main();

#ifdef __cplusplus
}
#endif

class GameSystem : public NerveExecutor
{
public:
    GameSystem();

    void* mFIFOBase; // _8
    u32* _C; // GameSequenceDirector*
    u32* _10; // GameSystemDimmingWatcher*
    u32* _14; // GameSystemErrorWatcher*
    u32* _18; // GameSystemFontHolder*
    u32* _1C; // GameSystemFrameControl*
    u32* _20; // GameSystemObjHolder*
    u32* _24; // GameSystemSceneController*
    u32* _28; // GameSystemStationedArchiveLoader*
    u32* _2C; // HomeButtonLayout*
    u32* _30; // SystemWipeHolder*
    u32* _34; // HomeButtonStateNotifier*

    bool mIsLoadingSystemArchive; // _38
};