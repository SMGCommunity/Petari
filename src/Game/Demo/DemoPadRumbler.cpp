#include "Game/Demo/DemoPadRumbler.hpp"
#include "Game/Util/ObjUtil.hpp"
#include <cstdio>

DemoPadRumbler::DemoPadRumbler(const char *pName) : mNumPadRumbleEntries(0), mPadRumbleEntries(nullptr), _8(0) {
    if (pName == nullptr) return;

    char buf[0x100];
    snprintf(buf, sizeof(buf), "MoviePadRumble%s.arc", pName);
    JMapInfo* map = MR::createCsvParser(buf, "PadRumbleData.bcsv");

    mNumPadRumbleEntries = MR::getCsvDataElementNum(map);
    PadRumbleInfo *infos = new PadRumbleInfo[mNumPadRumbleEntries];
    mPadRumbleEntries = infos;

    for (int i = 0; i < mNumPadRumbleEntries; i++) {
        PadRumbleInfo *rumble = &infos[i];
        MR::getCsvDataStrOrNULL(&rumble->mName, map, "PadRumbleName", i);
        MR::getCsvDataS32(&rumble->mStartFrame, map, "StartFrame", i);
    }
}

void DemoPadRumbler::update(s32 frame) {
    for (int i = 0; i < mNumPadRumbleEntries; i++) {
        PadRumbleInfo *rumble = &mPadRumbleEntries[i];
        if (rumble->mStartFrame == frame) {
            MR::tryRumblePad(this, rumble->mName, 0);
        }
    }
}
