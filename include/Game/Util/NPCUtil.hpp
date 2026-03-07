#pragma once

#include "Game/System/NerveExecutor.hpp"
#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/types.h>

class JMapInfoIter;
class LiveActor;
class ModelObj;
class NPCActor;
class NPCActorItem;
class PartsModel;
class TalkMessageCtrl;

namespace MR {
    bool getNPCItemData(NPCActorItem*, s32);

    bool isNPCItemFileExist(const char*);

    void startNPCTalkCamera(const TalkMessageCtrl*, MtxPtr, f32, s32);

    void initDefaultPosAndQuat(NPCActor*, const JMapInfoIter&);

    PartsModel* createNPCGoods(LiveActor*, const char*, const char*);
    PartsModel* createIndirectNPCGoods(LiveActor*, const char*, const char*);

    void invalidateLodCtrl(const NPCActor*);
    void startMoveAction(NPCActor*);
    bool tryStartTurnAction(NPCActor*);
    bool tryStartMoveTurnAction(NPCActor*);
    bool tryStartReaction(NPCActor*);
    bool tryTalkNearPlayerAndStartTalkAction(NPCActor*);
    bool tryTalkNearPlayerAndStartMoveTalkAction(NPCActor*);
    bool tryTalkNearPlayerAtEndAndStartTalkAction(NPCActor*);
    bool tryTalkNearPlayerAtEndAndStartMoveTalkAction(NPCActor*);
    bool tryTalkForceAndStartMoveTalkAction(NPCActor*);
    bool tryTalkForceAtEndAndStartTalkAction(NPCActor*);
    bool tryStartReactionAndPushNerve(NPCActor*, const Nerve*);
    bool tryStartReactionAndPopNerve(NPCActor*);
    bool tryChangeTalkActionRandom(NPCActor*, const char*, const char*, const char*);
    f32 calcFloatOffset(const NPCActor*, f32, f32);
    void calcAndSetFloatBaseMtx(NPCActor*, f32);

    bool tryStartTalkAction(NPCActor*);
    bool tryStartMoveTalkAction(NPCActor*);

    void turnPlayerToActor(const LiveActor*, f32);

    void setNPCActorPos(NPCActor*, const TVec3f&);
    void setNPCActorPose(NPCActor*, const TVec3f&, const TVec3f&, const TVec3f&);
    void followRailPose(NPCActor*, f32, f32);

    void setDefaultPose(NPCActor*);
};  // namespace MR

class DemoStarter : public NerveExecutor {
public:
    DemoStarter(NPCActor*);

    bool update();
    void start();
    void exeInit();
    void exeFade();
    void exeWait();
    void exeTerm();

    /* 0x8 */ NPCActor* mActor;
};

class TakeOutStar : public NerveExecutor {
public:
    TakeOutStar(NPCActor*, const char*, const char*, const Nerve*);

    bool takeOut();
    bool isFirstStep();
    bool isLastStep();
    void exeAnim();
    void exeDemo();
    void exeTerm();

    /* 0x08 */ ModelObj* mStarModel;
    /* 0x0C */ NPCActor* mActor;
    /* 0x10 */ const Nerve* mNerve;
    /* 0x14 */ const char* mActionName;
    /* 0x18 */ const char* mAnimName;
};

class FadeStarter : public NerveExecutor {
public:
    FadeStarter(NPCActor*, s32);

    bool update();
    void exeFade();
    void exeTerm();

    /* 0x08 */ NPCActor* mActor;
    /* 0x0C */ const Nerve* _C;
    /* 0x10 */ s32 _10;
};
