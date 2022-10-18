#pragma once

#include "Game/LiveActor/LiveActor.h"
#include "Game/LiveActor/LiveActorGroup.h"
#include "Game/LiveActor/FlashingCtrl.h"

class NoteFairy;

class NoteGroup : public LiveActorGroup {
public:
    NoteGroup();

    virtual ~NoteGroup();
    virtual void init(const JMapInfoIter &);
    virtual void movement();

    f32 mRotation;  // _18
};

class Note : public LiveActor {
public:
    Note(const char *, const TVec3f &, NoteFairy *);

    virtual ~Note();
    virtual void init(const JMapInfoIter &);
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor *, HitSensor *);

    void startCountDown();
    void exeWait();
    void exeCountDown();
    void exeFlyUp();

    NoteFairy* mParentFairy;    // _8C
    TVec3f _90;
    TVec3f mRailDirection;      // _9C
    int mCounter;               // _A8
    f32 _AC;
    bool mIsCountdown;          // _B0            
    FlashingCtrl* mFlashCtrl;   // _B4
};

namespace NrvNote {
    NERVE_DECL(NoteNrvWait, Note, Note::exeWait);
    NERVE_DECL(NoteNrvCountDown, Note, Note::exeCountDown);
    NERVE_DECL(NoteNrvFlyUp, Note, Note::exeFlyUp);
};