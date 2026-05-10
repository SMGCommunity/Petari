#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ReturnDemoRailMove;
class StageResultInformer;
class PowerStar;
class ActorCameraInfo;

class GrandStarReturnDemoStarter : public LiveActor {
public:
    GrandStarReturnDemoStarter(const char*);
    virtual ~GrandStarReturnDemoStarter();
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void control();

    void updateRailMoveEndDir();
    void calcOffsetStarToCore(TVec3f*) const;
    void emitEffectRush();
    void updateRushStarPos(const TVec3f&, s32);
    void tryStartStageResult(const char*);

    void exeMove();
    void exeFlyWait();
    void exeRushToCore();
    void exeRevival();
    void exeStageResult();
    void exeFadeOut();
    void exeWaitDemoEnd();

    /* 0x8C */ ReturnDemoRailMove* mReturnDemoRailMove;
    /* 0x90 */ StageResultInformer* mStageResultInformer;
    /* 0x94 */ TPos3f mPrevTransform;
    /* 0xC4 */ TPos3f mTransform;
    /* 0xF4 */ PowerStar* mPowerStar;
    /* 0xF8 */ TVec3f mDistanceToCore;
    /* 0x104 */ TVec3f mPowerStarPosition;
    /* 0x110 */ ActorCameraInfo* mActorCameraInfo;
};
