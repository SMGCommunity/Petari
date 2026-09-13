#pragma once

#include "Game/RhythmLib/AudBgmTempoAdjuster.hpp"
#include <JSystem/JAudio2/JASSeqParser.hpp>

class JASTrack;

class AudRhythmSeqParser : public JASSeqParser {
public:
    static const int MAX_CHANNELS = 8;

    struct NoteInfo {
        /* 0x00 */ bool mIsOn;
        /* 0x04 */ u32 mNoteOff;
        /* 0x08 */ u32 mPitch;
        /* 0x0C */ u32 mVelocity;
        /* 0x10 */ s64 mTime;
    };

    typedef void (*NoteOnFunc)(u32, u32);
    typedef void (*NoteOffFunc)();

    struct NoteOnInfo {
        /* 0x0 */ s32 mChannel;
        /* 0x4 */ s32 _4;
        /* 0x8 */ NoteOnFunc mNoteOnFunc;
        /* 0xC */ NoteOffFunc mNoteOffFunc;
    };

    enum RhythmType {
        RhythmType_0 = 0,
        RhythmType_1 = 1,
        RhythmType_2 = 2,
        RhythmType_3 = 3,
        RhythmType_4 = 4,
        RhythmType_5 = 5,
    };

    AudRhythmSeqParser();

    virtual s32 parse(JASTrack*);
    virtual bool execNoteOnMidi(JASTrack*, u32, u32, u32);
    virtual void execNoteOff(JASTrack*, u32);

    void noteOnProcess(u32, u32, u32, u32) NO_INLINE;  // FIXME
    void noteOffProcess(u32);

    bool checkNoteOn(u32, u32&, u32&, u32&, s64&, bool);
    bool checkNoteOnKey(u32, u32&, u32&, s64&, u32&);
    f32 getTempo() const;
    f32 getModifiedTempo() const;
    s32 getRhythmType(u32);
    u16 getTempoData(JASTrack*);
    u8 getChordNumberData(JASTrack*);
    u8 getScaleNumberData(JASTrack*);
    s32 getNextNoteOff(JASTrack*, u32);
    u32 passRegCommand(JASTrack*, int);
    u32 passCommand(JASTrack*, u8, u16);
    bool callTmp(JASTrack*, u32);
    bool retTmp(JASTrack*);

    void initNoteInfo(u32);
    void init();
    void activate();
    void setNoteInfo(u32, u32, u32, u32);
    bool registChordNumToInfo();
    bool registScaleNumToInfo();
    void flushChordScale();

    AudBgmTempoAdjuster* getTempoAdjuster() {
        return &mTempoAdjuster;
    }

    /* 0x004 */ f32 mTempo;
    /* 0x008 */ f32 mModifiedTempo;
    /* 0x00C */ u8 mChordId;
    /* 0x00D */ u8 mScaleId;
    /* 0x010 */ NoteInfo mNoteInfo[8];
    /* 0x0D0 */ u8 mRhythmType0;
    /* 0x0D1 */ u8 mRhythmType1;
    /* 0x0D2 */ u8 mRhythmType2;
    /* 0x0D3 */ u8 mRhythmType3;
    /* 0x0D4 */ u8 mRhythmType4;
    /* 0x0D8 */ AudBgmTempoAdjuster mTempoAdjuster;
    /* 0x100 */ u32 mCallDepth;
    /* 0x104 */ u8* mCallStack[8];
    /* 0x124 */ NoteOnInfo mNoteOnInfo[8];
    /* 0x1A4 */ s32 _1A4;
    /* 0x1A8 */ bool mIsUsed;
    /* 0x1A9 */ bool mIsActive;
    /* 0x1AC */ u32 _1AC;
};
