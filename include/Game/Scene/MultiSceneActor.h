#pragma once

#include "Game/Effect/EffectSystem.h"
#include "Game/NameObj/NameObj.h"
#include "Game/LiveActor/ModelManager.h"
#include "Game/Scene/MultiSceneEffectKeeper.h"

class MultiSceneActor : public NameObj {
public:
    MultiSceneActor(const char *, const char *, bool);

    virtual ~MultiSceneActor();
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