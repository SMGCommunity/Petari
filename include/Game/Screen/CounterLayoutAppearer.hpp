#pragma once

#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/LayoutActor.hpp"
#include "Game/System/NerveExecutor.hpp"

class CounterLayoutAppearer : public NerveExecutor {
public:
    CounterLayoutAppearer(LayoutActor *, const TVec2f &);
    
    virtual ~CounterLayoutAppearer();

    void update();
    void appear(const TVec2f &);
    void disappear();
    bool isAppeared() const;
    bool isDisappeared() const;
    void reset();
    void updateLayoutOffset(f32);
    void exeAppear();
    void exeDisappear();

    LayoutActor* mLayout;   // 0x8
    f32 _C;
    f32 _10;
    TVec2f mFollowPos;      // 0x14
    f32 _1C;
    f32 _20;
};

namespace NrvCounterLayoutAppearer {
    NERVE(CounterLayoutAppearerNrvHide);
    NERVE(CounterLayoutAppearerNrvAppear);
    NERVE(CounterLayoutAppearerNrvShow);
    NERVE(CounterLayoutAppearerNrvDisappear);
};