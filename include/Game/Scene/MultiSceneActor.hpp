#pragma once

#include "Game/Effect/EffectSystem.hpp"
#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/ModelManager.hpp"
#include "Game/Scene/MultiSceneEffectKeeper.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class Nerve;
class Spine;

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

    TVec3f mTranslation;                        // 0xC
    TVec3f mRotation;                           // 0x18
    TVec3f mScale;                              // 0x24
    bool _30;
    ModelManager* mModelMgr;                    // 0x34
    EffectSystem* mEffectSystem;                // 0x38
    MultiSceneEffectKeeper* mEffectKeeper;      // 0x3C
    Spine* mNerve;                              // 0x40
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