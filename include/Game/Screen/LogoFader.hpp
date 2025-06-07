#pragma once

#include "Game/Screen/LayoutActor.hpp"

class LogoFader : LayoutActor {
public:
    /// @brief Creates a new `LogoFader`.
    /// @param pName The pointer to the null-terminated name of the object.
    LogoFader(const char* pName);

    /// @brief Destroys the `LogoFader`.
    virtual ~LogoFader();

    virtual void draw() const;

    void exeBlank();
    void exeDisplay();
    void exeFadeIn();
    void exeFadeOut();
    void setBlank();
    void startFadeIn();
    void startFadeOut();
    bool isFadeEnd() const;

private:
    /* 0x20 */ bool _20;
    /* 0x24 */ s32 mMaxStep;
    /* 0x28 */ f32 mRate;
};
