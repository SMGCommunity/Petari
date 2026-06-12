#pragma once

class LiveActorFlag {
public:
    /// @brief Creates a new `LiveActorFlag`.
    LiveActorFlag();

    /* 0x00 */ bool mIsDead;
    /* 0x01 */ bool mIsStoppedAnim;
    /* 0x02 */ bool mIsNoCalcAnim;
    /* 0x03 */ bool mIsNoCalcView;
    /* 0x04 */ bool mIsHiddenModel;
    /* 0x05 */ bool mIsNoBind;
    /* 0x06 */ bool mIsCalcGravity;
    /* 0x07 */ bool mIsClipped;
    /* 0x08 */ bool mIsInvalidClipping;
};
