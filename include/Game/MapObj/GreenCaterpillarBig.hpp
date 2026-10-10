#pragma once

#include "Game/LiveActor/ModelObj.hpp"

class GreenCaterpillarBigBody : public ModelObj {
public:
    GreenCaterpillarBigBody(LiveActor*, MtxPtr);

    virtual ~GreenCaterpillarBigBody();
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void calcAndSetBaseMtx();

    void setPosAndDirection(LiveActor*);
    void calcBodyDir(LiveActor*, TVec3f*);

    /* 0x90 */ LiveActor* mCaterpillar;
    /* 0x94 */ TVec3f mFrontVec;
    /* 0xA0 */ LodCtrl* mPlanetLOD;
};

class GreenCaterpillarBig : public LiveActor {
public:
    GreenCaterpillarBig(const char*);

    virtual ~GreenCaterpillarBig();
    virtual void init(const JMapInfoIter&);
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();
    virtual void calcAndSetBaseMtx();

    void startWriggle();
    void exeHide();
    void exeWriggle();
    void exeRest();
    void exeEndAdjust();
    void exeEnd();
    void initBodyParts(const JMapInfoIter&);
    bool tryGenerateBodyParts();
    void fixBodyPartsOnRail();
    void leaveApple();

    /* 0x8C */ GreenCaterpillarBigBody** mBodyArray;
    /* 0x90 */ s32 mBodyArrayLength;
    /* 0x94 */ s32 mCurBodyParts;
    /* 0x98 */ s32 _98;
    /* 0x9C */ u8 _9C;
    /* 0x9D */ u8 _9D;
    /* 0xA0 */ LodCtrl* mPlanetLOD;
};
