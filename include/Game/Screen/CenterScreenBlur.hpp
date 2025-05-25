#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <revolution/types.h>

class CenterScreenBlur : public LiveActor {
public:
    CenterScreenBlur();
    ~CenterScreenBlur();

    virtual void init(JMapInfoIter const &);
    virtual void appear();
    virtual void draw() const;

    void start(s32, f32, bool, s32, s32);
    void exeFadeIn();
    void exeKeep();
    void exeFadeOut();

    s32 _8C;
    s32 _90;
    s32 _94;
    f32 _98;
    bool _9C;
    f32 _A0;
};
