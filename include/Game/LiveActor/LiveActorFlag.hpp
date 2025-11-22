#pragma once

class LiveActorFlag {
public:
    LiveActorFlag();

    bool mIsDead;             // 0x0
    bool mIsStoppedAnim;      // 0x1
    bool mIsNoCalcAnim;       // 0x2
    bool mIsNoCalcView;       // 0x3
    bool mIsHiddenModel;      // 0x4
    bool mIsNoBind;           // 0x5
    bool mIsCalcGravity;      // 0x6
    bool mIsClipped;          // 0x7
    bool mIsInvalidClipping;  // 0x8
};