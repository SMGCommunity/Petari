#pragma once

#include "Game/AudioLib/AudRemixSequencer.hpp"
#include "Game/AudioLib/AudSoundObject.hpp"

class JKRHeap;
struct RemixTrack {
    s32 _0;
    u32* _4;
};

struct RemixNoteGroupData {
    /* 0x0  */ s32 mIndex;
    /* 0x4  */ s32 mTrackCount;
    /* 0x8  */ u32 mNoteCount;
    u32* _C;
    /* 0x10 */ RemixTrack* mRemixTracks;
};

class AudRemixMgr {
public:
    AudRemixMgr(JKRHeap* pHeap);
    void init();
    void update();
    void setRemixSeqResource(void* ptr);
    RemixNoteGroupData* getRemixNoteGroupDataFromMelodyNo(s32 melodyNo) const;

    /* 0x0  */ JKRHeap* mHeap;
    /* 0x4  */ RemixNoteGroupData* mGroups;
    /* 0x8  */ s32 mGroupCount;
    /* 0xC  */ AudSoundObject* mSoundObj;
    /* 0x10 */ AudRemixSequencer* mRemixSeq;
};
