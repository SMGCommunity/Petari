#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/RailRider.hpp"
#include "Game/Map/RailGraphEdge.hpp"
#include "Game/Map/RailGraphIter.hpp"
#include "Game/NPC.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/MultiEventCamera.hpp"
#include "Game/Util/ParabolicPath.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include <cstddef>
#include "Game/Util/FootPrint.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "revolution/mtx.h"
#include "Game/NPC/RabbitStateCaught.hpp"
#include "Game/NPC/RabbitStateWaitStart.hpp"
namespace TrickRabbitUtil {
    FootPrint* createRabbitFootPrint(LiveActor* pActor);
}

class SpotMarkLight : public PartsModel {
    public:
    SpotMarkLight(LiveActor* pActor, f32 f1, f32 f2, MtxPtr mtx);
    virtual ~SpotMarkLight();
    virtual void control();
    virtual void calcAndSetBaseMtx();
};

class RailGraphNodeSelecter {
    public:

};

namespace {
    class RunnawayNodeSelector : public RailGraphNodeSelecter {
    public:
        virtual bool isSatisfy(RailGraphIter& rIter);
    };
};

class TrickRabbit : public LiveActor {
public:
    TrickRabbit(const char* pName);
    virtual ~TrickRabbit();
    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    void initModel();
    void initSensor();
    void initBind();
    void initRoute(const JMapInfoIter& rIter);
    void initDemoCamera(const JMapInfoIter& rIter);
    void initState();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg,HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    bool receiveMsgTrample();
    bool requestCaught();
    bool tryStartDemoRunnaway();
    void endDemoRunnaway();
    bool tryRunaway();
    bool tryBrakeTurn();
    bool tryBrakeTurnEnd();
    bool tryStop();
    bool tryJumpStart();
    bool tryJump();
    bool tryJumpEnd();
    bool tryEndFallDown();
    bool tryEndFallDownLand();
    bool tryComebackRouteStart();
    bool tryComebackRoute();
    bool tryEndTumble();
    bool tryEndGetUp();
    bool tryDemoJumpStart();
    bool tryDemoJump();
    bool tryDemoJumpEnd();
    bool tryEndPowerStarDemo();
    void exeWaitStart();
    void exeDemoRunaway();
    void exeDemoJumpStart();
    void exeDemoJump();
    void exeDemoEnd();
    void exeWait();
    void exeRunaway();
    void exeBrakeTurn();
    void exeJumpStart();
    void exeJump();
    void exeFallDown();
    void exeGetUpFromFallDown();
    void exeComebackRouteStart();
    void exeComebackRoute();
    void exeTumble();
    void exeGetUp();
    void exeCaught();
    void exeWaitPowerStarDemo();
    void exeGiveUp();
    void doWait();
    void doRunaway();
    void doJumpStart();
    void doJump();
    void tearDownWait();
    bool checkDivingThicket(const TVec3f& rVec1, const TVec3f& rVec2);
    bool selectEdgeStartEvent(RailGraphIter* pIter);
    void selectEdgeRunnaway(RailGraphIter* pIter, const TVec3f& rVec, f32 f);
    void startRouteLevelControl(s32 l1, s32 l2) NO_INLINE;
    void controlRouteLevel();
    void addMovingAccel(const TVec3f& rVec, f32 f);
    void addKeepRouteRange(f32 f1, f32 f2, f32 f3);
    void updateFootPrint();
    bool isCaughtable() const;
    bool isTumbable() const;
    bool isNextEdgeJump() const;
    void setUpJumpParam();
    void setUpJumpParamFromJMap();
    s32 calcDefaultJumpTime() const;
    s32 calcJumpStartTime() const;
    s32 calcJumpTime() const;
    f32 calcJumpHeight() const;
    void initTimer();
    void updateTime();

    TQuat4f _8C;
    TVec3f _9C; // 0x9C, 0xA0, 0xA4
    RabbitStateWaitStart* mRabbitStateWaitStart; // 0xA8
    RabbitStateCaught* mRabbitStateCaught; // 0xAC
    MultiEventCamera* mMultiEventCamera; // 0xB0
    TalkMessageCtrl* mTalkMessageCtrl; // 0xB4
    FootPrint* mFootPrint; // 0xB8
    RailGraph* mRailGraph; // 0xBC
    RailGraphIter* mRailGraphIter; // 0xC0
    ParabolicPath* mParabolicPath; // 0xC4
    SpotMarkLight* mSpotMarkLight; // 0xC8
    s32 _CC;
    f32 _D0;
    s32 _D4;
    s32 _D8;
    s32 _DC;
    bool _E0;
    bool _E1;
    s32 _E4;
    bool _E8;
    bool _E9;
};

namespace NrvTrickRabbit {
    NERVE_DECL_EXE(TrickRabbitNrvWaitStart, TrickRabbit, WaitStart);
    NERVE(TrickRabbitNrvTryStartDemoRunnaway);
    NERVE_DECL_EXE(TrickRabbitNrvDemoRunaway, TrickRabbit, DemoRunaway);
    NERVE_DECL_EXE(TrickRabbitNrvDemoJumpStart, TrickRabbit, DemoJumpStart);
    NERVE_EXECEND(TrickRabbitNrvDemoJump);
    NERVE_DECL_EXE(TrickRabbitNrvDemoEnd, TrickRabbit, DemoEnd);
    NERVE_EXECEND(TrickRabbitNrvWait);
    NERVE_DECL_EXE(TrickRabbitNrvRunaway, TrickRabbit, Runaway);
    NERVE_DECL_EXE(TrickRabbitNrvJumpStart, TrickRabbit, JumpStart);
    NERVE_EXECEND(TrickRabbitNrvJump);
    NERVE_DECL_EXE(TrickRabbitNrvBrakeTurn, TrickRabbit, BrakeTurn);
    NERVE_DECL_EXE(TrickRabbitNrvFallDown, TrickRabbit, FallDown);
    NERVE(TrickRabbitNrvFallDownLand);
    NERVE_DECL_EXE(TrickRabbitNrvGetUpFromFallDown, TrickRabbit, GetUpFromFallDown);
    NERVE_DECL_EXE(TrickRabbitNrvComebackRouteStart, TrickRabbit, ComebackRouteStart);
    NERVE_EXECEND(TrickRabbitNrvComebackRoute);
    NERVE_DECL_EXE(TrickRabbitNrvTumble, TrickRabbit, Tumble);
    NERVE_DECL_EXE(TrickRabbitNrvGetUp, TrickRabbit, GetUp);
    NERVE_DECL_EXE(TrickRabbitNrvCaught, TrickRabbit, Caught);
    NERVE_DECL_EXE(TrickRabbitNrvWaitPowerStarDemo, TrickRabbit, WaitPowerStarDemo);
    NERVE_DECL_EXE(TrickRabbitNrvGiveUp, TrickRabbit, GiveUp);
};
