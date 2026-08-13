#pragma once

#include "Game/LiveActor/HitSensor.hpp"
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

    /* 0x8C */  LiveActor* _8C;
    /* 0x90 */  TVec3f _90;
    /* 0x9C */  TVec3f _9C;
    /* 0xA8 */  TQuat4f _A8;
    /* 0xB8 */  SpinDriverShootPath* _B8;
    /* 0xBC */  SpinDriverCamera* _BC;
    /* 0xC0 */  TVec3f _C0;
    /* 0xCC */  TVec3f _CC;
    /* 0xD8 */  TVec3f _D8;
    /* 0xE4 */  f32 _E4;
    /* 0xE8 */  f32 _E8;
    /* 0xEC */  f32 _EC;
    /* 0xF0 */  s32 _F0;
    /* 0xF4 */  s32 _F4;
    /* 0xF8 */  s32 _F8;
    /* 0xFC */  s32 _FC;
    /* 0x100 */ f32 _100;
    /* 0x104 */ SimpleLayout* _104;
    /* 0x108 */ ScenarioTitle* _108;
    /* 0x10C */ TVec3f _10C;
    /* 0x118 */ TVec3f _118;
    /* 0x124 */ bool _124;
};
