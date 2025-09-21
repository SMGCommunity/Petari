#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <revolution/types.h>

class CenterScreenBlur : public LiveActor {
public:
    /// @brief Creates a new `CenterScreenBlur`.
    CenterScreenBlur();

    /// @brief Destroys the `CenterScreenBlur`.
    virtual ~CenterScreenBlur() {}

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

    s32 _8C;
    s32 _90;
    s32 _94;
    f32 _98;
    u8 _9C;
    f32 _A0;
};
