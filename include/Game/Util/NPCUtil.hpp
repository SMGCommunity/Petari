#pragma once

#include <revolution.h>
#include <JSystem/JGeometry.hpp>
#include "Game/System/NerveExecutor.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class NPCActorItem;
class TalkMessageCtrl;
class NPCActor;
class ModelObj;
class LiveActor;

namespace MR {
    bool getNPCItemData(NPCActorItem *, s32);

    bool isNPCItemFileExist(const char *);

    void startNPCTalkCamera(const TalkMessageCtrl *, MtxPtr, f32, s32);

    void invalidateLodCtrl(const NPCActor *);
    void startMoveAction(NPCActor *);
    bool tryStartTurnAction(NPCActor *);
    bool tryStartMoveTurnAction(NPCActor *);
    bool tryStartReaction(NPCActor *);
    bool tryTalkNearPlayerAndStartTalkAction(NPCActor *);
    bool tryTalkNearPlayerAndStartMoveTalkAction(NPCActor *);
    bool tryTalkNearPlayerAtEndAndStartTalkAction(NPCActor *);
    bool tryTalkNearPlayerAtEndAndStartMoveTalkAction(NPCActor *);
    bool tryTalkForceAndStartMoveTalkAction(NPCActor *);
    bool tryTalkForceAtEndAndStartTalkAction(NPCActor *);
    bool tryStartReactionAndPushNerve(NPCActor *, const Nerve *);
    bool tryStartReactionAndPopNerve(NPCActor *);
    bool tryChangeTalkActionRandom(NPCActor *, const char *, const char *, const char *);
    f32 calcFloatOffset(const NPCActor *, f32, f32);
    void calcAndSetFloatBaseMtx(NPCActor *, f32);

    bool tryStartTalkAction(NPCActor *);

    void turnPlayerToActor(const LiveActor *, f32);

    void setNPCActorPos(NPCActor *, const TVec3f &);
    void setNPCActorPose(NPCActor *, const TVec3f &, const TVec3f &, const TVec3f &);

    void setDefaultPose(NPCActor *);
};

class DemoStarter : public NerveExecutor {
public:
    DemoStarter(NPCActor *);

    virtual ~DemoStarter();

    bool update();
    void start();
    void exeFade();
    inline void exeTerm();
    inline void exeWait();
    inline void exeInit();

    NPCActor* mActor;           // 0x08
};

class TakeOutStar : public NerveExecutor {
public:
    TakeOutStar(NPCActor *, const char *, const char *, const Nerve *);

    virtual ~TakeOutStar();

    bool takeOut();
    bool isFirstStep();
    bool isLastStep();
    void exeAnim();
    void exeDemo();
    inline void exeTerm();

    ModelObj* mStarModel;           // 0x08
    NPCActor* mActor;               // 0x0C
    const Nerve* mNerve;            // 0x10
    const char* mActionName;        // 0x14
    const char* mAnimName;          // 0x18
};

class FadeStarter : public NerveExecutor {
public:
    FadeStarter(NPCActor *, s32);

    virtual ~FadeStarter();

    bool update();
    void exeFade();
    inline void exeTerm();

    NPCActor* mActor;           // 0x08
    const Nerve* _C;
    s32 _10;
};
