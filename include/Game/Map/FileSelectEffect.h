#pragma once

#include "Game/LiveActor/LiveActor.h"

class FileSelectEffect : public LiveActor {
public:
    FileSelectEffect(const char *);

    virtual ~FileSelectEffect();
    virtual void init(const JMapInfoIter &);
    virtual void appear();
    virtual void calcAndSetBaseMtx();

    void disappear();
    void exeAppear();
    void exeDisappear();

    f32 mEffectFrame;       // _8C
};

namespace {
    NERVE_DECL(FileSelectEffectNrvAppear, FileSelectEffect, FileSelectEffect::exeAppear);
    NERVE_DECL_NULL(FileSelectEffectNrvWait);
    NERVE_DECL(FileSelectEffectNrvDisappear, FileSelectEffect, FileSelectEffect::exeDisappear);
};