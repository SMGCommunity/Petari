#pragma once

class LiveActorFlag {
public:
    LiveActorFlag();

    bool mIsDead;               // _0
    bool mIsStoppedAnim;        // _1
    bool mIsNoCalcAnim;         // _2
    bool mIsNoCalcView;         // _3
    bool mIsHiddenModel;        // _4
    bool mIsNoBind;             // _5
    bool mIsCalcGravity;        // _6
    bool mIsClipped;            // _7
    bool mIsInvalidClipping;    // _8
};