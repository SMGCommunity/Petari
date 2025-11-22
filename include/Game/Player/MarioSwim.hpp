#pragma once

#include "Game/Map/WaterInfo.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioState.hpp"

class MarioActor;
class MarineSnow;

class MarioSwim : public MarioState {
public:
    MarioSwim(MarioActor*);

    virtual void init();
    virtual bool proc(u32);
    virtual bool start();
    virtual bool close();
    virtual bool update();
    virtual bool notice();
    virtual bool keep();
    virtual bool postureCtrl(MtxPtr);
    virtual void hitWall(const TVec3f&, HitSensor*);
    virtual void hitPoly(u8, const TVec3f&, HitSensor*);
    virtual bool passRing(const HitSensor*);
    virtual f32  getBlurOffset() const;
    virtual void draw3D() const;

    bool checkWaterCube(bool);
    void onSurface();
    void doJetJump(u8);
    void updateLifeByTime();
    void surfacePaddle();
    void flowOnWave(f32);
    void checkWaterBottom();
    void spin();
    void decideVelocity();
    void procBuoyancy();
    void decideAnimation();
    void decideEffect(bool);
    void updateTilt();
    void jet();
    void pushedByWaterWall();
    void setDamage(const TVec3f&, u16);
    void updateUnderwater();
    void startJet(u32);
    void addDamage(const TVec3f&);
    void addFaint(const TVec3f&);

    f32 getSurface() const;

    virtual TVec3f& getGravityVec() const;

    static inline f32 getWorthlessNumber() {
        return 0.523598790169f;
    }

    /* The worthlesser variable, as its name suggests, has no purpose.
       It achieves nothing in life. Its existence is completely devoid of value.
       Nevertheless, the compiler left evidence of this forgettable variable behind.
       As a result, we know that it existed, but we do not know how precisely it achieved nothing.

       Summary:
       worthlesser does not impact the behavior of this function.

     */

    inline void funReferenceTime(bool& worthlesser) {
        if (_5C > 1.57079637051f) {
            worthlesser = true;
        } else {
            if (_3C > 0x1E) {
                _1E = 1;
            }
            worthlesser = false;
            if (getStickY() > 0.0f) {
                f32 stickY = getStickY();
                // Note: The binary does not tell us whether this comparison is > or <.
                // It is not == because that generates an fcmpu instruction, not fcmpo.
                // It is not <= or >= because those generate cror instructions.
                if (getWorthlessNumber() + getWorthlessNumber() * stickY < _5C

                    // This comparison needs to be present for the compiler to optimize the condition
                    // accurately.
                    && worthlesser) {
                    return;
                }
            }
        }
        worthlesser = true;
    }

    inline TVec3f getPlayer380() const {
        return getPlayer()->_380;
    }

    inline bool check7Aand7C() const {
        return _7A || _7C;
    }

    MarineSnow* _14;
    u8          _18;
    u8          _19;
    u8          _1A;
    u8          _1B;
    u8          _1C;
    u8          _1D;
    u8          _1E;
    u8          _1F;
    u8          _20;
    u8          _21;
    u8          _22;
    u32         _24;
    u32         _28;
    u16         _2C;
    u16         _2E;
    u16         _30;
    u16         _32;
    u16         _34;
    u16         _36;
    u16         _38;
    u16         _3A;
    u16         _3C;
    u16         _3E;
    u16         _40;
    u16         _42;
    u16         _44;
    f32         _48;
    f32         _4C;
    f32         _50;
    f32         _54;
    f32         _58;
    f32         _5C;
    TVec3f      _60;
    TVec3f      _6C;
    u8          _78;
    u16         _7A;
    u16         _7C;
    f32         _80;
    f32         _84;
    u8          _88;
    u16         _8A;
    u16         _8C;
    u16         _8E;
    u16         _90;
    f32         _94;
    f32         _98;
    u8          _9C;
    u8          _9D;
    u8          _9E;
    u8          _9F;
    TVec3f      _A0;
    u8          _AC;
    u8          _AD;
    u16         _AE;
    TMtx34f     _B0;
    f32         _E0;
    f32         _E4;
    u16         _E8;
    u16         _EA;
    u16         _EC;
    u16         _EE;
    u16         _F0;
    WaterInfo   _F4;
    s32         _144;
    TVec3f      _148;
    TVec3f      _154;
    TVec3f      _160;
    TVec3f      _16C;
    TVec3f      _178;
    TVec3f      _184;
    TVec3f      _190;
    f32         _19C;
    f32         _1A0;
    f32         _1A4;
    f32         _1A8;
    f32         _1AC;
    u16         _1B0;
    u8          _1B2;
    f32         _1B4;
    f32         _1B8;
};
