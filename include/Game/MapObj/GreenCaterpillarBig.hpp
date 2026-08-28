#pragma once

#include "Game/LiveActor/ModelObj.hpp"

class GreenCaterpillarBigBody : public ModelObj {
public:
    GreenCaterpillarBigBody(LiveActor*, MtxPtr);

    /* 0x08 */ virtual ~GreenCaterpillarBigBody();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();

    void setPosAndDirection(LiveActor*);
    void calcBodyDir(LiveActor*, TVec3f*);

    LiveActor* mCaterpillar;  // 0x90
    TVec3f mFrontVec;         // 0x94
    LodCtrl* mPlanetLOD;      // 0xA0
};

class GreenCaterpillarBig : public LiveActor {
public:
    GreenCaterpillarBig(const char*);

    /* 0x08 */ virtual ~GreenCaterpillarBig();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x40 */ virtual void startClipped();
    /* 0x44 */ virtual void endClipped();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();

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

    GreenCaterpillarBigBody** mBodyArray;  // 0x8C
    s32 mBodyArrayLength;                  // 0x90
    s32 mCurBodyParts;                     // 0x94
    s32 _98;
    u8 _9C;
    u8 _9D;
    LodCtrl* mPlanetLOD;  // 0xA0
};
