#pragma once

class LayoutActorFlag {
public:
    LayoutActorFlag();

    /* 0x0 */ bool mIsDead;
    /* 0x1 */ bool mIsStopAnimFrame;
    /* 0x2 */ bool mIsHidden;
    /* 0x3 */ bool mIsOffCalcAnim;
};
