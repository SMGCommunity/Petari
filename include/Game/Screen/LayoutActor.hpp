#pragma once

#include "Game/NameObj/NameObj.h"
#include "Game/LiveActor/Spine.h"
#include "Game/Screen/LayoutActorFlag.h"
#include "Game/Screen/LayoutManager.h"
#include "JSystem/JGeometry.h"

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

    LayoutManager* mManager;                                    // _C
    Spine* mSpine;                                              // _10
    PaneEffectKeeper* mPaneEffectKeeper;                        // _14
    StarPointerLayoutTargetKeeper* mStarPointerTargetKeeper;    // _18
    LayoutActorFlag mFlags;                                     // _1C
};