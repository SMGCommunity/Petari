#pragma once

#include "Game/System/NerveExecutor.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class LayoutActor;

class CounterLayoutAppearer : public NerveExecutor {
public:
    CounterLayoutAppearer(LayoutActor*, const TVec2f&);

    void update();
    void appear(const TVec2f&);
    void disappear();
    bool isAppeared() const;
    bool isDisappeared() const;
    void reset();
    void updateLayoutOffset(f32);
    void setAppearOffset(const TVec2f&);
    void exeHide();
    void exeAppear();
    void exeShow();
    void exeDisappear();

private:
    /* 0x08 */ LayoutActor* mLayout;
    /* 0x0C */ TVec2f _C;
    /* 0x14 */ TVec2f mFollowPos;
    /* 0x1C */ TVec2f _1C;
};
