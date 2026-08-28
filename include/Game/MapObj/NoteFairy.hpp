#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ActorCameraInfo;
class Note;

class NoteFairy : public LiveActor {
public:
    NoteFairy(const char*);

    /* 0x08 */ virtual ~NoteFairy();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void killAllNotes();
    void exeHide();
    void exeStartAppearDemo();
    void exeRailMoveStart();
    void exeDemoAppear();
    void exeAppearNoteBloom();
    void exeAppearNote();
    void exeEndWait();
    void exeSuccessDemo();
    void exeSuccess();
    void enterDemoAppear(const Nerve*, bool);
    bool isDeadAllNotes();
    void setLastGotNote(Note*);
    f32 getNoteCoord(s32);

    TVec3f _8C;
    TVec3f _98;
    f32 mAppearanceSpeed;  // 0xA4
    s32 mTimeLimit;        // 0xA8
    s32 mMelodyNoteNum;    // 0xAC
    s32 _B0;
    s32 mSong;              // 0xB4
    s32 mCompletionJingle;  // 0xB8
    f32 mNoteCoord;         // 0xBC
    Note** mNoteArray;      // 0xC0
    Note* mLastGotNote;     // 0xC4
    s32 mAppearanceType;    // 0xC8
    bool _CC;
    bool _CD;
    bool mHasDemo;  // 0xCE
    bool _CF;
    bool _D0;
    bool _D1;
    s32 mPowerStarSpawnType;       // 0xD4
    ActorCameraInfo* mCameraInfo;  // 0xD8
};
