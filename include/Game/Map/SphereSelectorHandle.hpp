#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class SphereSelectorHandle : public LiveActor {
public:
    SphereSelectorHandle(const char*);
    virtual ~SphereSelectorHandle();
    virtual MtxPtr getBaseMtx() const {
        return (MtxPtr)&_90;
    }

    void validateRotate();
    void invalidateRotate();
    bool isHolding() const;

public:
    /* 0x08C */ u32 _8C;
    /* 0x090 */ TMtx34f _90;
    /* 0x0C0 */ u8 mPad2[0xC];
    /* 0x0CC */ f32 mRotateSpeed;
    /* 0x0D0 */ u8 mPad3[0x5C];
};
