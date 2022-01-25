#pragma once

#include "Game/NameObj/NameObj.h"
#include "Game/LiveActor/Spine.h"
#include "Game/Screen/LayoutActorFlag.h"

class LayoutManager;
class PaneEffectKeeper;
class StarPointerLayoutTargetKeeper;

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

    void initLayoutManager(const char *, u32);
    void initNerve(const Nerve *);

    bool isNerve(const Nerve *) const;
    void setNerve(const Nerve *) const;

    LayoutManager* mManager;                                    // _C
    Spine* mSpine;                                              // _10
    PaneEffectKeeper* mPaneEffectKeeper;                        // _14
    StarPointerLayoutTargetKeeper* mStarPointerTargetKeeper;    // _18
    LayoutActorFlag mFlags;                                     // _1C
};