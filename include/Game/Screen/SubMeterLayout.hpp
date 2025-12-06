#pragma once

#include "Game/Screen/LayoutActor.hpp"

class SubMeterLayout : public LayoutActor {
public:
    SubMeterLayout(const char*, const char*);

    virtual void init(const JMapInfoIter&);
    virtual void control();

    void setLifeRatio(f32);
    void requestDeactivate();
    void requestAppear();
    void requestFrameIn();
    void requestFrameOut();

    u8 _20;
    u8 _21;
    u16 _22;
    u16 _24;
    u16 _26;
};
