#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class CenterScreenBlur : public LiveActor {
public:
    /// @brief Creates a new `CenterScreenBlur`.
    CenterScreenBlur();

    /// @brief Intializes the `CenterScreenBlur` while being placed into a scene.
    /// @param rIter The reference to an iterator over a `JMapInfo`.
    virtual void init(const JMapInfoIter& rIter);

    /// @brief Draws the `CenterScreenBlur` to the screen.
    virtual void draw() const;

    virtual void appear();

    void start(s32, f32, u8, s32, s32);
    void exeFadeIn();
    void exeKeep();
    void exeFadeOut();

    /* 0x8C */ s32 mTime;
    /* 0x90 */ s32 mFadeIn;
    /* 0x94 */ s32 mFadeOut;
    /* 0x98 */ f32 mOffset;
    /* 0x9C */ u8 mAlpha;
    /* 0xA0 */ f32 mBlendRate;
};
