#pragma once

#include "Game/LiveActor/LiveActor.h"

class Air : public LiveActor {
public:
    Air(const char *);

    virtual ~Air();
    virtual void init(const JMapInfoIter &);
    virtual void appear();
    virtual void initModel(const char *);
    virtual void setFarClipping();

    bool isDrawing() const;
    bool tryChange();
    void appearFadeOut();
    void appearFadeIn();
    void exeIn();
    void exeOut();

    u8 _8C;
    bool _8D;
    f32 mDistance;  // _90
};

namespace NrvAir {
    NERVE(HostTypeIn);
    NERVE(HostTypeOut);
};