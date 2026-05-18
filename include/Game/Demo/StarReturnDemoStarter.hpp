#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class NameObjArchiveListCollector;
class ReturnDemoRailMove;
class StageResultInformer;
class PowerStar;

class StarReturnDemoStarter : public LiveActor {
public:
    StarReturnDemoStarter(const char*);
    
    virtual ~StarReturnDemoStarter();
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void kill();
    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);
    virtual void control();

    ModelObj* createSubModel(const char*, const char*, bool);
    void tryRegisterDemo(const char*, const JMapInfoIter&);
    void tryStartStageResult();
    void tryStartResultWait();

    void exeAppearWait();
    void exeMove();
    void exeFlyWaitPowerStar();
    void exeFlyWaitGrandStar();
    void exeFall();
    void exeLand();
    void exeWait();
    void exeStageResult();
    void exeFadeOut();
    void exeStageResultAfter();

private:
    /* 0x8C */ ReturnDemoRailMove* mReturnDemoRailMove;
    /* 0x90 */ StageResultInformer* mStageResultInformer;
    /* 0x94 */ TPos3f mTransform;
    /* 0xC4 */ TVec3f mPosition;
    /* 0xD0 */ PowerStar* mPowerStar;
    /* 0xD4 */ ModelObj* mLuma;
    /* 0xD8 */ ModelObj* mHair;
    /* 0xDC */ s32 mFrame;
};
