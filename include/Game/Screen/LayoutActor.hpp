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

    /* 0x08 */ virtual ~LayoutActor() {
    }

    /* 0x14 */ virtual void movement();
    /* 0x18 */ virtual void draw() const;
    /* 0x1C */ virtual void calcAnim();
    /* 0x24 */ virtual void appear();
    /* 0x28 */ virtual void kill();

    /* 0x2C */ virtual void control() {
    }

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

    /* 0x0C */ LayoutManager* mLayoutManager;
    /* 0x10 */ Spine* mSpine;
    /* 0x14 */ PaneEffectKeeper* mEffectKeeper;
    /* 0x18 */ StarPointerLayoutTargetKeeper* mPointingTarget;
    /* 0x1C */ LayoutActorFlag mFlag;
};
