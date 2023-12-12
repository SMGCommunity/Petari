#pragma once

#include "Game/LiveActor/ModelObj.h"

class GreenCaterpillarBigBody : public ModelObj {
public:
    GreenCaterpillarBigBody(LiveActor *, MtxPtr);

    virtual ~GreenCaterpillarBigBody();
    virtual void init(const JMapInfoIter &);
    virtual void control();
    virtual void calcAndSetBaseMtx();

    void setPosAndDirection(LiveActor *);
    void calcBodyDir(LiveActor *, TVec3f *);

    LiveActor* mCaterpillar;    // _90
    TVec3f mFrontVec;           // _94
    LodCtrl* mPlanetLOD;        // _A0
};

class GreenCaterpillarBig : public LiveActor {
public:
    GreenCaterpillarBig(const char *);

    virtual ~GreenCaterpillarBig();
    virtual void init(const JMapInfoIter &);
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();
    virtual void calcAndSetBaseMtx();

    void startWriggle();
    void exeWriggle();
    void initBodyParts(const JMapInfoIter &);
    bool tryGenerateBodyParts();
    void fixBodyPartsOnRail();
    void leaveApple();

    GreenCaterpillarBigBody** mBodyArray;   // _8C
    s32 mBodyArrayLength;                   // _90
    s32 mCurBodyParts;                      // _94
    s32 _98;
    u8 _9C;
    u8 _9D;
    LodCtrl* mPlanetLOD;                    // _A0
};

namespace NrvGreenCaterpillarBig {
    NERVE(GreenCaterpillarBigNrvHide);
    NERVE(GreenCaterpillarBigNrvWriggle);
    NERVE(GreenCaterpillarBigNrvRest);
    NERVE(GreenCaterpillarBigNrvEndAdjust);
    NERVE(GreenCaterpillarBigNrvEnd);
};