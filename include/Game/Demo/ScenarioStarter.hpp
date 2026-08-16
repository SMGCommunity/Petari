#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SpinDriverCamera;
class SpinDriverShootPath;
class ScenarioTitle;
class SimpleLayout;

class ScenarioStarter : public LiveActor {
public:
    ScenarioStarter(const char*);

    virtual ~ScenarioStarter();
    virtual void init(const JMapInfoIter& rIter);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void initShootPath(const JMapInfoIter&);
    void exeWaitScenarioCameraEnd();
    void exeWaitToStart();
    void exeCinemaFrameBlank();
    void exeRailMove();
    void exeRailMoveCanceled();
    void exeShowWelcomeLayout();
    bool isStartBgmOnWelcome();
    void calcShootMotionTime();
    void turnBindHead(const TVec3f&, f32);
    void updateBindPosition();
    void updateShootMotion();
    bool trySkipTrigger() const;
    void updateBindActorMtx();

    /* 0x08C */  LiveActor* _8C;
    /* 0x090 */  TVec3f _90;
    /* 0x09C */  TVec3f _9C;
    /* 0x0A8 */  TQuat4f _A8;
    /* 0x0B8 */  SpinDriverShootPath* _B8;
    /* 0x0BC */  SpinDriverCamera* mSpinDriverCamera;
    /* 0x0C0 */  TVec3f _C0;
    /* 0x0CC */  TVec3f _CC;
    /* 0x0D8 */  TVec3f _D8;
    /* 0x0E4 */  f32 _E4;
    /* 0x0E8 */  f32 _E8;
    /* 0x0EC */  f32 _EC;
    /* 0x0F0 */  s32 mFlightTime;
    /* 0x0F4 */  s32 mFlyMotionStartStep;
    /* 0x0F8 */  s32 _F8;
    /* 0x0FC */  s32 mFallMotionStartStep;
    /* 0x100 */ f32 mInitalPlayerRotation;
    /* 0x104 */ SimpleLayout* mWelcomeLayout;
    /* 0x108 */ ScenarioTitle* mTitle;
    /* 0x10C */ TVec3f _10C;
    /* 0x118 */ TVec3f _118;
    /* 0x124 */ bool _124;
};
