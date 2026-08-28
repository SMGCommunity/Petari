#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioFpView : public MarioState {
public:
    MarioFpView(MarioActor*);

    /* 0x14 */ virtual bool start();
    /* 0x1C */ virtual bool update();
    /* 0x18 */ virtual bool close();
    virtual f32 getBlurOffset() const;

    void forceClose();

    /* 0x12 */ u16 mDelay;
    /* 0x14 */ bool mIsActive;
    /* 0x18 */ f32 mBlurOffset;
};
