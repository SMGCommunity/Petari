#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SimpleEffectObj : public LiveActor {
public:
    SimpleEffectObj(const char*);

    /* 0x08 */ virtual ~SimpleEffectObj() {};
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x40 */ virtual void startClipped();
    /* 0x44 */ virtual void endClipped();

    virtual f32 getClippingRadius() const {
        return 500.0f;
    };

    virtual f32 getFarClipDistance() const {
        return 50.0f;
    };

    virtual TVec3f* getClippingCenterOffset() const {
        TVec3f vec;
        vec.set(0.0f, 0.0f, 0.0f);
        return &vec;
    };

    /* 0x78 */ virtual bool isSyncClipping() const {
        return false;
    };

    /* 0x7C */ virtual void setStateWait();
    /* 0x80 */ virtual void setStateMove();

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
