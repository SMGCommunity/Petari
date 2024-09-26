#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/Spine.hpp"
#include "Game/Screen/LayoutActorFlag.hpp"
#include "Game/Screen/LayoutManager.hpp"
#include "JSystem/JGeometry.hpp"

class PaneEffectKeeper;
class StarPointerLayoutTargetKeeper;
class EffectSystem;

class LayoutActor : public NameObj {
public:
    LayoutActor(const char *, bool);

    virtual ~LayoutActor() {

    }

    virtual void movement();
    virtual void draw() const;
    virtual void calcAnim();
    virtual void appear();
    virtual void kill();

    virtual void control() {

    }

    void initNerve(const Nerve *);

    void setNerve(const Nerve *) const;
    bool isNerve(const Nerve *) const;
    s32 getNerveStep() const;
    const TVec2f getTrans() const;
    void setTrans(const TVec2f &);
    LayoutManager* getLayoutManager() const;

    void initLayoutManager(const char *, u32);
    void initLayoutManagerNoConvertFilename(const char *, u32);
    void initLayoutManagerWithTextBoxBufferLength(const char *, u32, u32);

    void initEffectKeeper(int, const char *, const EffectSystem *);

    void updateSpine();

    void initPointingTarget(int);

    LayoutManager* mManager;                                    // 0xC
    Spine* mSpine;                                              // 0x10
    PaneEffectKeeper* mPaneEffectKeeper;                        // 0x14
    StarPointerLayoutTargetKeeper* mStarPointerTargetKeeper;    // 0x18
    LayoutActorFlag mFlags;                                     // 0x1C
};