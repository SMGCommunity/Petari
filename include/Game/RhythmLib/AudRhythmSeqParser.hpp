#pragma once

#include "Game/RhythmLib/AudBgmTempoAdjuster.hpp"
#include <JSystem/JAudio2/JASSeqParser.hpp>

class JASTrack;

class AudRhythmSeqParser : public JASSeqParser {
public:
    struct NoteInfo {
        /* 0x00 */ bool _0;
        /* 0x04 */ u32 _4;
        /* 0x08 */ u32 _8;
        /* 0x0C */ u32 _C;
        /* 0x10 */ u32 _10;
        /* 0x14 */ u32 _14;
    };

    struct NoteOnInfo {
        /* 0x0 */ s32 _0;
        /* 0x4 */ s32 _4;
        /* 0x8 */ s32 _8;
        /* 0xC */ s32 _C;
    };

    AudRhythmSeqParser();

    virtual s32 parse(JASTrack*);
    virtual void execNoteOnMidi(JASTrack*, u32, u32, u32);
    virtual void execNoteOff(JASTrack*, u32);

    void noteOnProcess(u32, u32, u32, u32);
    void noteOffProcess(u32);

    void checkNoteOn(u32, u32&, u32&, u32&, s64&, bool);
    void checkNoteOnKey(u32, u32&, u32&, s64&, u32&);
    void getTempo();
    void getModifiedTempo();
    void getRhythmType(u32);
    void getTempoData(JASTrack*);
    void getChordNumberData(JASTrack*);
    void getScaleNumberData(JASTrack*);
    void getNextNoteOff(JASTrack*, u32);
    void passRegCommand(JASTrack*, int);
    void passCommand(JASTrack*, u8, u16);
    void callTmp(JASTrack*, u32);
    void retTmp(JASTrack*);

    void initNoteInfo(u32);
    void init();
    void activate();
    void setNoteInfo(u32, u32, u32, u32);
    void registChordNumToInfo();
    void registScaleNumToInfo();
    void flushChordScale();

    AudBgmTempoAdjuster* getTempoAdjuster() {
        return &mTempoAdjuster;
    }

    /* 0x004 */ f32 mTempo;
    /* 0x008 */ f32 mModifiedTempo;
    /* 0x00C */ bool _C;
    /* 0x00D */ bool _D;
    /* 0x010 */ NoteInfo mNoteInfo[8];
    /* 0x0D0 */ u8 _D0;
    /* 0x0D1 */ u8 _D1;
    /* 0x0D2 */ u8 _D2;
    /* 0x0D3 */ u8 _D3;
    /* 0x0D4 */ u8 _D4;
    /* 0x0D8 */ AudBgmTempoAdjuster mTempoAdjuster;
    /* 0x100 */ u32 _100;
    /* 0x104 */ JASTrack* mCallStack[8];
    /* 0x124 */ NoteOnInfo mNoteOnInfo[8];
    /* 0x1A4 */ u32 _1A4;
    /* 0x1A8 */ bool _1A8;
    /* 0x1A9 */ bool _1A9;
    /* 0x1AC */ u32 _1AC;
};
