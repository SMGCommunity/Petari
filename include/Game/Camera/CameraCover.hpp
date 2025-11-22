#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Screen/CaptureScreenDirector.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"

class CameraCover : public NameObj {
public:
    CameraCover(const char*);

    virtual ~CameraCover();
    virtual void movement();
    virtual void draw() const;

    void cover(u32);
    bool isCameraHopping() const;
    void copyCamera();

    inline s32 getThing() { return _3C; }

    TMtx34f _C;
    volatile s32 _3C;
    u8 _40;
    bool _41;
    u8 _42[2];
    u32 _44;
    CaptureScreenActor* mActor;  // 0x48
};