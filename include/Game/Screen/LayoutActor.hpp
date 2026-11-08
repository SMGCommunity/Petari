#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Screen/LayoutActorFlag.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class EffectSystem;
class LayoutManager;
class Nerve;
class PaneEffectKeeper;
class Spine;
class StarPointerLayoutTargetKeeper;

class LayoutActor : public NameObj {
public:
    LayoutActor(const char* pName, bool = true);

    virtual ~LayoutActor() {
    }

    virtual void movement();
    virtual void draw() const;
    virtual void calcAnim();
    virtual void appear();
    virtual void kill();

    virtual void control() {
    }

    void setNerve(const Nerve* pNerve) const;
    bool isNerve(const Nerve* pNerve) const;
    s32 getNerveStep() const;
    TVec2f getTrans() const;
    void setTrans(const TVec2f& rTrans);
    LayoutManager* getLayoutManager() const;
    void createPaneMtxRef(const char* pName);
    MtxPtr getPaneMtxRef(const char* pName);
    void initLayoutManager(const char* pName, u32 rootPaneAnimLayerNum);
    void initLayoutManagerNoConvertFilename(const char* pName, u32 rootPaneAnimLayerNum);
    void initLayoutManagerWithTextBoxBufferLength(const char* pName, u32 textBoxBufferLength, u32 rootPaneAnimLayerNum);
    void initNerve(const Nerve* pNerve);
    void initEffectKeeper(int extraEmitterNum, const char* pName, const EffectSystem* pEffectSystem);
    void initPointingTarget(int maxNumTargets);
    void updateSpine();

    /* 0x0C */ LayoutManager* mLayoutManager;
    /* 0x10 */ Spine* mSpine;
    /* 0x14 */ PaneEffectKeeper* mEffectKeeper;
    /* 0x18 */ StarPointerLayoutTargetKeeper* mPointingTarget;
    /* 0x1C */ LayoutActorFlag mFlag;
};
