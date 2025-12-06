#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry.hpp>

class Camera;
class CameraPoseParam;

class CameraHeightArrange : public NameObj {
public:
    CameraHeightArrange(Camera*);

    void resetJump();

    void resetParameter();

    void chase();
    void updateHeightAndOffset();
    TVec3f* getGlobalAxis();

    Camera* mCamera;  // 0xC
    u8 _10;
    u8 _11;
    u8 _12;
    u8 _13;
    CameraPoseParam* _14;
    CameraPoseParam* _18;
    u8 _1C[12];
    f32 _28;
    f32 _2C;
    f32 _30;
    f32 _34;
    f32 _38;
    f32 _3C;
    u32 _40;
    u32 _44;
    u8 _48[4];
    u8 _4C;
    u8 _4D[3];
    s32 _50;
    u32 _54;
    u32 _58;
    u32 _5C;
    u8 _60;
    u8 _61[3];
    TVec3f mGlobalAxis;  // 0x64
    f32 _70;
    f32 _74;
    f32 mUpper;         // 0x78
    f32 mLower;         // 0x7C
    u32 mGndInt;        // 0x80
    f32 mUPlay;         // 0x84
    f32 mLPlay;         // 0x88
    u32 mPushDelay;     // 0x8C
    u32 mPushDelayLow;  // 0x90
    u32 mUDown;         // 0x94
    u8 mVPanUse;        // 0x98
    u8 _99[3];
    TVec3f mVPanAxis;  // 0x9C
};
