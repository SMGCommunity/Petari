#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/mtx.h"

class SimpleEffectObj : public LiveActor {
public:
    SimpleEffectObj(const char*);

    virtual ~SimpleEffectObj() {};
    virtual void init(const JMapInfoIter&);
    virtual void startClipped();
    virtual void endClipped();

    virtual f32 getClippingRadius() const { return 500.0f; };

    virtual f32 getFarClipDistance() const { return 50.0f; };

    virtual TVec3f* getClippingCenterOffset() const {
        TVec3f vec;
        vec.set(0.0f, 0.0f, 0.0f);
        return &vec;
    };

    virtual bool isSyncClipping() const { return false; };

    virtual void setStateWait();
    virtual void setStateMove();

    bool isStateMove() const;
    void exeWait();
    void exeMove();

    const char* _8C;
    TVec3f _90;
};

typedef struct {
    const char* _0;
    const char* _4;
    const char* _8;
    s32 _C;
    const char* _10;
    s32 _14;
} s_effectDataTable;
