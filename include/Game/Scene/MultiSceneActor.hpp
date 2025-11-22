#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class EffectSystem;
class ModelManager;
class MultiSceneEffectKeeper;
class Nerve;
class Spine;

class MultiSceneActor : public NameObj {
public:
    MultiSceneActor(const char*, const char*, bool);

    virtual void init(const JMapInfoIter&);
    virtual void movement();
    virtual void calcAnim();
    virtual void calcViewAndEntry();
    virtual void appear();
    virtual void kill();
    virtual void control();

    ModelManager* getModelManager() const;
    EffectSystem* getEffectSystem() const;
    MultiSceneEffectKeeper* getEffectKeeper() const;
    void initEffect(EffectSystem*, int, const char*);
    void setNerve(const Nerve*);
    bool isNerve(const Nerve*) const;
    void initNerve(const Nerve*);
    s32 getNerveStep() const;

    /* 0x0C */ TVec3f mTranslation;
    /* 0x18 */ TVec3f mRotation;
    /* 0x24 */ TVec3f mScale;
    /* 0x30 */ bool _30;
    /* 0x34 */ ModelManager* mModelManager;
    /* 0x38 */ EffectSystem* mEffectSystem;
    /* 0x3C */ MultiSceneEffectKeeper* mEffectKeeper;
    /* 0x40 */ Spine* mSpine;
};

namespace MultiScene {
    void startBck(MultiSceneActor*, const char*);
    void startBtk(MultiSceneActor*, const char*);
    void startBtp(MultiSceneActor*, const char*);
    void startBva(MultiSceneActor*, const char*);
    void setBtpFrameAndStop(MultiSceneActor*, f32);
    void setBvaFrameAndStop(MultiSceneActor*, f32);
    bool isBckStopped(MultiSceneActor*);
    bool isStep(const MultiSceneActor*, s32);
    bool isFirstStep(const MultiSceneActor*);
    bool isGreaterStep(const MultiSceneActor*, s32);
    bool isLessStep(const MultiSceneActor*, s32);
    bool isLessEqualStep(const MultiSceneActor*, s32);
    void setNerveAtStep(MultiSceneActor*, const Nerve*, s32);
    void setNerveAtBckStopped(MultiSceneActor*, const Nerve*);
    f32 calcNerveRate(const MultiSceneActor*, s32);
    f32 calcNerveRate(const MultiSceneActor*, s32, s32);
    f32 calcNerveEaseOutRate(const MultiSceneActor*, s32);
    f32 calcNerveValue(const MultiSceneActor*, s32, f32, f32);
    void emitEffect(const MultiSceneActor*, const char*);
    void deleteEffectAll(const MultiSceneActor*);
    void forceDeleteEffectAll(const MultiSceneActor*);
    MtxPtr getJointMtx(const MultiSceneActor*, const char*);
};  // namespace MultiScene
