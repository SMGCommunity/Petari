#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioFpView : public MarioState {
public:
    MarioFpView(MarioActor *);

    virtual bool start();
    virtual bool close();
    virtual bool update();
    virtual f32 getBlurOffset() const;
    
    void forceClose();

    u16 _12;
    bool _14;
    float mBlurOffset;      // 0x18
};
