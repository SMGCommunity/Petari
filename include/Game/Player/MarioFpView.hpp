#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioFpView : public MarioState {
public:
    MarioFpView(MarioActor*);

    virtual bool start();
    virtual bool update();
    virtual bool close();
    virtual f32 getBlurOffset() const;

    void forceClose();

    /* 0x12 */ u16 mDelay;
    /* 0x14 */ bool mIsActive;
    /* 0x18 */ f32 mBlurOffset;
};
