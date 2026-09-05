#pragma once

#include "Game/RhythmLib/AudMeChannelMgr.hpp"
#include "Game/RhythmLib/AudMeSeqCtrl.hpp"
#include "JSystem/JAudio2/JASDSPInterface.hpp"
#include "JSystem/JGadget/linklist.hpp"
#include <JSystem/JAudio2/JASHeapCtrl.hpp>
#include <JSystem/JAudio2/JASOscillator.hpp>

class AudMeTrack : public JASPoolAllocObject< AudMeTrack > {
public:
    static const int OSC_NUM = 2;

    struct TrackInfo {
        void init();

        /* 0x00 */ u8 mPitch;
        /* 0x02 */ u16 mBankNumber;
        /* 0x04 */ u16 mProgNumber;
        /* 0x06 */ u8 mBendSense;
        /* 0x08 */ u16 mRelease;
        /* 0x0A */ u8 _A;
        /* 0x0B */ u8 _B;
        /* 0x0C */ u32 mNoteReg;
        /* 0x10 */ u8 mNoteRangeLower;
        /* 0x11 */ u8 mNoteRangeUpper;
        /* 0x12 */ u8 mNoteRangeStart;
    };

    struct TList : JGadget::TLinkList< AudMeTrack, -0x384 > {
        TList() : mCallbackRegistered(false) {
        }

        ~TList() {
        }

        static s32 cbSeqMain(void*);

        void append(AudMeTrack*);
        void seqMain();
        void getSameME(AudMeTrack*, u32);

        /* 0xC */ bool mCallbackRegistered;
    };

    AudMeTrack() : mChannelMgr(this){};

    void init();
    void initPlayParam();
    void inherit(AudMeTrack*);
    AudMeTrack* newChild(s32);
    void deleteChild(s32);
    bool isHaveChild() const;
    void setSeqData(void*, u32);
    void startSeq();
    void stopSeq();
    void stopAtCurrentNoteEnd();
    void rhythmProc(s32, s32);
    void setPauseFlag(bool);
    void channelStart(AudMeChannelMgr*, u32, u32, u32);
    void noteOn(u32, u32, u32);
    void noteOff(u32, u32);
    void noteOffAll(u16);
    void startNote();
    void channelUpdateCallback(u32, JASChannel*, JASDsp::TChannel*, void*);
    void initRegister();
    void overWriteOsc(JASChannel*);
    void setOscAdsr(s16, s16, s16, s16, u16);
    void updateChannelParams();
    void gframeProc();
    void setBaseNoteFromChord(u8*, s32, bool);
    void setBaseNoteFromScale(u8*, s32, bool);
    void setBaseNoteFromChromatic(u8*, s32, bool);
    void setBaseNoteFromOtherPart(s32);
    void setBaseNoteFromUpperPart();
    void setBaseNoteFix(u8);
    void setBaseNoteFromRegister(s32);
    void adjustNote(s32, s32);
    void modifyNote(s32, s32, s32);
    void modifyNoteRnd(s32, s32, s32);
    u32 getRandom(u32);
    void makeRandomSearchList(s32*, s32, bool);
    void adjustInNoteRange(u8&);
    void adjustInStartNoteRange(u8&);
    s32 getMyOrderInBrother();
    AudMeTrack* getBrother(s32);

    AudMeSeqCtrl* getSeqCtrl() {
        return &mSeqCtrl;
    }

    bool getFlagZ() const {
        return mJumpFlag == 0;
    }

    bool getFlagNZ() const {
        return mJumpFlag != 0;
    }

    void setJumpFlag(bool flag) {
        mJumpFlag = flag;
    }
    void clearJumpFlag() {
        mJumpFlag = false;
    }

    u8 getPitch() const {
        return mTrackInfo.mPitch;
    }
    void setPitch(u8 pitch) {
        mTrackInfo.mPitch = pitch;
    }
    u16 getBankNumber() const {
        return mTrackInfo.mBankNumber;
    }
    void setBankNumber(u16 bankNo) {
        mTrackInfo.mBankNumber = bankNo;
    }
    u16 getProgNumber() const {
        return mTrackInfo.mProgNumber;
    }
    void setProgNumber(u16 progNo) {
        mTrackInfo.mProgNumber = progNo;
    }
    u16 getRelease() const {
        return mTrackInfo.mRelease;
    }
    void setRelease(u16 release) {
        mTrackInfo.mRelease = release;
    }
    u8 getNoteRangeLower() const {
        return mTrackInfo.mNoteRangeLower;
    }
    void setNoteRangeLower(u8 lower) {
        mTrackInfo.mNoteRangeLower = lower;
    }
    u8 getNoteRangeUpper() const {
        return mTrackInfo.mNoteRangeUpper;
    }
    void setNoteRangeUpper(u8 upper) {
        mTrackInfo.mNoteRangeUpper = upper;
    }
    u8 getNoteRangeStart() const {
        return mTrackInfo.mNoteRangeStart;
    }
    void setNoteRangeStart(u8 start) {
        mTrackInfo.mNoteRangeStart = start;
    }
    void setNoteNoStartRange(bool noStart) {
        mNoStartRange = noStart;
    }
    void setPan(u8 pan) {
        mPan = pan / 127.0f;
    }
    void setDolby(u8 dolby) {
        mDolby = dolby / 127.0f;
    }
    void setVolume(u16 volume) {
        mVolume = volume / 32767.0f;
    }
    void setPitchBend(s16 bend) {
        mPitchBend = bend / 32767.0f;
    }
    void setFxMix(u16 fxMix) {
        mFxMix = fxMix / 32767.0f;
    }
    void setBendSense(u8 sense) {
        mTrackInfo.mBendSense = sense;
    }
    void setNoteReg(u32 reg) {
        mTrackInfo.mNoteReg = reg;
    }

    u8 getBaseNotePrev() const {
        return mBaseNotePrev;
    }
    void setBaseNote(u8 note) {
        mBaseNote = note;
    }

    static u8 sReg[0x40];

    /* 0x000 */ AudMeSeqCtrl mSeqCtrl;
    /* 0x088 */ AudMeChannelMgr mChannelMgr;
    /* 0x0E8 */ u32 _E8;
    /* 0x0EC */ bool _EC;
    /* 0x0EF */ bool _ED;
    /* 0x0F0 */ u32 _F0;
    /* 0x0F0 */ u32 _F4;
    /* 0x0F8 */ JASOscillator::Data mOscParam[OSC_NUM];
    /* 0x128 */ JASOscillator::Point mOscPoint[4];
    /* 0x140 */ f32 mVolume;
    /* 0x144 */ f32 mPitchBend;
    /* 0x148 */ f32 mFxMix;
    /* 0x14C */ f32 mPan;
    /* 0x150 */ f32 mDolby;
    /* 0x154 */ u8 mBaseNote;
    /* 0x155 */ u8 mBaseNotePrev;
    /* 0x156 */ bool _156;
    /* 0x157 */ bool _157;
    /* 0x158 */ bool mNoStartRange;
    /* 0x15C */ u32 _15C;
    /* 0x160 */ u16 _160;
    /* 0x162 */ bool mJumpFlag;
    /* 0x163 */ u8 _163[8];  // AudMeSeqParser::cmdCmpCI
    /* 0x16C */ TrackInfo mTrackInfo;
};
