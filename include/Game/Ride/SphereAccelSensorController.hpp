#pragma once

#include "Game/Ride/SpherePadController.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class SphereAccelSensorController : SpherePadController {
public:
    SphereAccelSensorController();

    virtual f32 calcJumpPower() const;
    virtual f32 calcMoveVector(TVec3f*, const TVec3f&);
    virtual bool doBrake() const;
    virtual void update(const TVec3f&);
    virtual void notifyActivate();
    virtual void notifyDeactivate();
    virtual void clacXY(f32*, f32*);
    // void drawDebug(); // DEBUG
    // virtual void genMessage(JORMContext*); // DEBUG

    void getPadAcceleration(TVec3f*);
    bool testBrake() const;
    // void listenPropertyEvent(const JORPropertyEvent*); // DEBUG

    inline bool isDeadZone(const TVec2f& vec) { return vec.dot(vec) <= 0.0000038146973f; }

    // everything up to 0x58 seems like it may be inhereted memory
    // however none of the inhereted classes use anything near
    // this much memory. Unsure of what exactly this memory is...
    u32 _08, _0C;
    u32 _10, _14, _18, _1C;
    u32 _20, _24, _28, _2C;
    u32 _30, _34, _38, _3C;
    u32 _40, _44, _48, _4C;
    u32 _50, _54;

    /* 0x58 */ u32 _58;
    /* 0x5C */ u32 _5C;

    /* 0x60 */ u32 _60;
    /* 0x64 */ u32 _64;
    /* 0x68 */ u32 _68;
    /* 0x6C */ u32 _6C;
    /* 0x70 */ u32 _70;

    /* 0x74 */ u32 _74;
    /* 0x78 */ u32 _78;

    /* 0x7C */ f32 _7C;
    /* 0x80 */ f32 _80;
    /* 0x84 */ f32 _84;
    /* 0x88 */ f32 _88;

    /* 0x8C */ u32 _8C;
    /* 0x90 */ u32 _90;

    /* 0x94 */ f32 _94;
    /* 0x98 */ f32 _98;
    /* 0x9C */ f32 _9C;
    /* 0xA0 */ f32 _A0;
    /* 0xA4 */ f32 _A4;
    /* 0xA8 */ f32 _A8;
    /* 0xAC */ f32 _AC;

    /* 0xB0 */ u16 _B0;
    /* 0xB4 */ f32 _B4;
    /* 0xB8 */ u32 _B8;  // determines whether wiimote or nunchuck is used for control
};
