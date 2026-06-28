#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"

class SphereSelectorHandle : public LiveActor {
public:
    SphereSelectorHandle(const char*);
    virtual ~SphereSelectorHandle();
    virtual MtxPtr getBaseMtx() const;

    void validateRotate();
    void invalidateRotate();
    bool isHolding() const;

public:
    /* 0x08C */ u32 _8C;
    /* 0x090 */ TMtx34f _90;
private:    u8 mPad2[0xC];
public:     /* 0x0CC */ f32 mRotateSpeed;
private:    u8 mPad3[0x60];

};
