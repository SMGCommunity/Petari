#pragma once

#include <JSystem/JAudio2/JASGlobal.hpp>

class JAISoundHandle;
class JKRHeap;
class AudRhythmSeqParser;

class AudRhythmMeSystem : public JASGlobalInstance< AudRhythmMeSystem > {
public:
    AudRhythmMeSystem(JKRHeap*, u32, bool);

    AudRhythmMeSystem();

    void setSeq(JAISoundHandle&, s32);
    void rejectSeq(s32);
    void rhythmProc();
    void update();
    void setIdle(bool);
    void setUsingRhythmParser(u32);

    s32 getBgmIdx() const {
        return mBgmIdx;
    }

    // TODO: memory mapping
    /* 0x00 */ AudRhythmSeqParser* mSeqParsers;
    /* 0x04 */ u32 mNumSeqParsers;
    /* 0x08 */ s32 mBgmIdx;
    /* 0x0C */ u32 _C;
    /* 0x10 */  // AudRhythmHolder mRhythmHolder[2];
};
