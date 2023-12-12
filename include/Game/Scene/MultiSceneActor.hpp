#pragma once

#include "Game/Effect/EffectSystem.h"
#include "Game/NameObj/NameObj.h"
#include "Game/LiveActor/ModelManager.h"
#include "Game/Scene/MultiSceneEffectKeeper.h"

class MultiSceneActor : public NameObj {
public:
    MultiSceneActor(const char *, const char *, bool);

    virtual ~MultiSceneActor() {

    }

    virtual void init(const JMapInfoIter &);
    virtual void movement();
    virtual void calcAnim();
    virtual void calcViewAndEntry();
    virtual void appear();
    virtual void kill();
    virtual void control();

    void initEffect(EffectSystem *, int, const char *);
    void setNerve(const Nerve *);
    bool isNerve(const Nerve *) const;
    void initNerve(const Nerve *);

    TVec3f mTranslation;                        // _C
    TVec3f mRotation;                           // _18
    TVec3f mScale;                              // _24
    bool _30;
    ModelManager* mModelMgr;                    // _34
    EffectSystem* mEffectSystem;                // _38
    MultiSceneEffectKeeper* mEffectKeeper;      // _3C
    Spine* mNerve;                              // _40
};

class MultiScene {
public:
    static void startBck(MultiSceneActor *, const char *);
    static void startBtk(MultiSceneActor *, const char *);
    static void startBtp(MultiSceneActor *, const char *);
    static void startBva(MultiSceneActor *, const char *);
    static void setBtpFrameAndStop(MultiSceneActor *, f32);
    static void setBvaFrameAndStop(MultiSceneActor *, f32);
    static bool isBckStopped(MultiSceneActor *);
    static bool isStep(const MultiSceneActor *, s32);
    static bool isFirstStep(const MultiSceneActor *);
    static bool isGreaterStep(const MultiSceneActor *, s32);
    static bool isLessStep(const MultiSceneActor *, s32);
    static bool isLessEqualStep(const MultiSceneActor *, s32);
    static void setNerveAtStep(MultiSceneActor *, const Nerve *, s32);
    static void setNerveAtBckStopped(MultiSceneActor *, const Nerve *);
    static f32 calcNerveRate(const MultiSceneActor *, s32);
    static f32 calcNerveRate(const MultiSceneActor *, s32, s32);
    static f32 calcNerveEaseOutRate(const MultiSceneActor *, s32);
    static f32 calcNerveValue(const MultiSceneActor *, s32, f32, f32);
    static void emitEffect(const MultiSceneActor *, const char *);
    static void deleteEffectAll(const MultiSceneActor *);
    static void forceDeleteEffectAll(const MultiSceneActor *);
    static MtxPtr getJointMtx(const MultiSceneActor *, const char *);
};