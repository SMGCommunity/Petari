#pragma once

#include "Game/LiveActor/LiveActor.h"

class Note;

class NoteFairy : public LiveActor {
public:
    NoteFairy(const char *);

    virtual ~NoteFairy();
    virtual void init(const JMapInfoIter &);
    virtual void kill();
    virtual void control();
    virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);

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
    void enterDemoAppear(const Nerve *, bool);
    bool isDeadAllNotes();
    void setLastGotNote(Note *);
    f32 getNoteCoord(s32);

    TVec3f _8C;
    TVec3f _98;
    f32 mAppearanceSpeed;           // _A4
    s32 mTimeLimit;                 // _A8
    s32 mMelodyNoteNum;             // _AC
    s32 _B0;
    s32 mSong;                      // _B4
    s32 mCompletionJingle;          // _B8
    f32 mNoteCoord;                 // _BC
    Note** mNoteArray;              // _C0
    Note* mLastGotNote;             // _C4
    s32 mAppearanceType;            // _C8
    bool _CC;
    bool _CD;
    bool mHasDemo;                  // _CE
    bool _CF;
    bool _D0;
    bool _D1;
    s32 mPowerStarSpawnType;        // _D4
    ActorCameraInfo* mCameraInfo;   // _D8
};

namespace NrvNoteFairy {
    NERVE_DECL(NoteFairyNrvHide, NoteFairy, NoteFairy::exeHide);
    NERVE_DECL(NoteFairyNrvStartAppearDemo, NoteFairy, NoteFairy::exeStartAppearDemo);
    NERVE_DECL(NoteFairyNrvRailMoveStart, NoteFairy, NoteFairy::exeRailMoveStart);
    NERVE_DECL(NoteFairyNrvDemoAppear, NoteFairy, NoteFairy::exeDemoAppear);
    NERVE_DECL(NoteFairyNrvAppearNoteBloom, NoteFairy, NoteFairy::exeAppearNoteBloom);
    NERVE_DECL(NoteFairyNrvAppearNote, NoteFairy, NoteFairy::exeAppearNote);
    NERVE_DECL(NoteFairyNrvEndWait, NoteFairy, NoteFairy::exeEndWait);
    NERVE_DECL(NoteFairyNrvSuccessDemo, NoteFairy, NoteFairy::exeSuccessDemo);
    NERVE_DECL(NoteFairyNrvSuccess, NoteFairy, NoteFairy::exeSuccess);
};
