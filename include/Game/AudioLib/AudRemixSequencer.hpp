#pragma once
#include "Game/GameAudio/AudTalkSoundData.hpp"
#include <revolution/types.h>

struct RemixNoteTrackData {
    u32 _0;
};

struct RemixNoteData {
    s32 _0;
    s32 _4;
    s32 _8;
    s32 _C;
};

class AudRmxSeqNoteOnTimer {
public:
    AudRmxSeqNoteOnTimer();
    void initData();
    void setData(const RemixNoteTrackData*, const RemixNoteData*);
    bool update(f32);
    JAISoundID getFreeSeID();
    f32 _0;
    f32 _4;
    const RemixNoteData* _8;
    const RemixNoteTrackData* _C;
};

class AudRemixSequencer {
public:
    AudRemixSequencer();
    void initNoteOnBuff();
    void update();
    void setTempo(f32) NO_INLINE;
    AudRmxSeqNoteOnTimer* newNoteOnTimer();
    void addNoteData(const RemixNoteTrackData*, const RemixNoteData*);
    AudRmxSeqNoteOnTimer _0[0x20];
    f32 _200;
    f32 _204;
};
