#pragma once

#include <revolution/types.h>

class JAISoundID;

namespace AudStageBgmWrap {
    JAISoundID changeStageNameToSoundID(const char*, const char*, s32);
    JAISoundID getCometEventBgm(const char*);
    JAISoundID getKoopaFortressAppearBgm(const char*);
};  // namespace AudStageBgmWrap
