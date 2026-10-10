#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Screen/CaptureScreenDirector.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class CameraCover : public NameObj {
public:
    CameraCover(const char*);

    virtual ~CameraCover();
    virtual void movement();
    virtual void draw() const;

    void cover(u32);
    bool isCameraHopping() const;
    void copyCamera();

    inline s32 getThing() {
        return _3C;
    }

    /* 0x0C */ TMtx34f _C;
    /* 0x3C */ volatile s32 _3C;
    /* 0x40 */ u8 _40;
    /* 0x41 */ bool _41;
    /* 0x42 */ u8 _42[2];
    /* 0x44 */ u32 _44;
    /* 0x48 */ CaptureScreenActor* mActor;
};
