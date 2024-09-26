#pragma once

#include "Game/NameObj/NameObj.hpp"

class ScreenPreserver : public NameObj {
public:
    ScreenPreserver();

    virtual ~ScreenPreserver();
    virtual void draw() const;

    void captureIfAllow();
    void activate();
    void deactivate();

    bool mIsActive;     // 0xC
    u8 _D[3];
};