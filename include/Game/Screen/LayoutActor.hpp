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
    LayoutActor(const char*, bool);

    virtual ~LayoutActor() {}

    virtual void movement();
    virtual void draw() const;
    virtual void calcAnim();
    virtual void appear();
    virtual void kill();

    virtual void control() {}

    void setNerve(const Nerve*) const;
    bool isNerve(const Nerve*) const;
    s32 getNerveStep() const;
    TVec2f getTrans() const;
    void setTrans(const TVec2f&);
    LayoutManager* getLayoutManager() const;
    void createPaneMtxRef(const char*);
    MtxPtr getPaneMtxRef(const char*);
    void initLayoutManager(const char*, u32);
    void initLayoutManagerNoConvertFilename(const char*, u32);
    void initLayoutManagerWithTextBoxBufferLength(const char*, u32, u32);
    void initNerve(const Nerve*);
    void initEffectKeeper(int, const char*, const EffectSystem*);
    void initPointingTarget(int);
    void updateSpine();

    /* 0x0C */ LayoutManager* mManager;
    /* 0x10 */ Spine* mSpine;
    /* 0x14 */ PaneEffectKeeper* mPaneEffectKeeper;
    /* 0x18 */ StarPointerLayoutTargetKeeper* mStarPointerTargetKeeper;
    /* 0x1C */ LayoutActorFlag mFlag;
};
