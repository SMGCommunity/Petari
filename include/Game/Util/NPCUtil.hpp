#pragma once

#include <revolution.h>
#include "Game/System/NerveExecutor.hpp"

class NPCActorItem;
class TalkMessageCtrl;
class NPCActor;
class ModelObj;

namespace MR {
    bool getNPCItemData(NPCActorItem *, s32);

    bool isNPCItemFileExist(const char *);

    void startNPCTalkCamera(const TalkMessageCtrl *, MtxPtr, f32, s32);

    bool tryStartReactionAndPushNerve(NPCActor *, const Nerve *);

    bool tryTalkNearPlayerAtEndAndStartTalkAction(NPCActor *);

    bool tryTalkForceAndStartMoveTalkAction(NPCActor *);

    f32 calcFloatOffset(const NPCActor *, f32, f32);

    void calcAndSetFloatBaseMtx(NPCActor *, f32);
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
