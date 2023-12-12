#pragma once

#include <revolution/types.h>
#include "Game/LiveActor/LiveActor.h"

class Mario;
class MarioActor;
class MarioAnimator;
class MarioState;
class Triangle;

class MarioModule {
public:
    inline MarioModule() {
        
    }

    //virtual f32 getStickY() const;
    virtual void addVelocity(const TVec3f &);
    virtual void addVelocity(const TVec3f &, f32);
    virtual void init() = 0;
    virtual bool proc(u32) = 0;
    virtual bool start() = 0;
    virtual bool close() = 0;
    virtual bool update() = 0;
    virtual bool notice() = 0;
    virtual bool keep() = 0;
    virtual bool postureCtrl(MtxPtr) = 0;
    virtual void hitWall(const TVec3f &, HitSensor *) = 0;
    virtual void hitPoly(u8, const TVec3f &, HitSensor *);
    virtual bool passRing(const HitSensor *) = 0;
    virtual f32 getBlurOffset() const = 0;
    virtual void draw3D() const = 0;

    Mario* getPlayer() const;
    MarioAnimator* getAnimator() const;
    TVec3f& getShadowPos() const;
    TVec3f& getGravityVec() const;
    TVec3f& getAirGravityVec() const;
    TVec3f& getFrontVec() const;
    bool isStatusActiveID(u32) const;
    bool isStatusActiveS(MarioState *) const;
    u16 getPlayerMode() const;
    bool isPlayerModeBee() const;
    bool isPlayerModeIce() const;
    bool isPlayerModeTeresa() const;
    bool isPlayerModeHopper() const;
    bool isPlayerModeInvincible() const;
    bool isPlayerModeFoo() const;
    void changeAnimation(const char *, const char *);
    void changeAnimationNonStop(const char *);
    void changeAnimationWithAttr(const char *, const char *);
    void stopAnimation(const char *, const char *);
    bool isDefaultAnimationRun(const char *) const;
    void changeAnimationInterpoleFrame(u32);
    bool isAnimationRun(const char *) const;
    bool isAnimationTerminate(const char *) const;
    bool isAnimationTerminateUPper(const char *) const;
    const char* getAnimationStringPointer(const char *) const;
    const char* getCurrentBckName() const;
    void changeAnimationUpper(const char *, const char *);
    void changeAnimationUpperWeak(const char *, const char *);
    void stopAnimationUpper(const char *, const char *);
    void stopAnimationUpperForce();
    f32 getAnimationFrame() const;
    void changeAnimation(const char *, u32);
    bool isAnimationRun(const char *, u32);
    void stopAnimation(const char *, u32);
    void setJointGlobalMtx(u16, MtxPtr);
    void setYangleOffset(f32);
    bool isStickOn() const;
    bool isStickFull() const;
    TVec3f& getWorldPadDir() const;
    bool calcWorldPadDir(TVec3f *, f32, f32, bool);
    void addVelocityAfter(const TVec3f &);
    void clearVelocity();
    TVec3f& getVelocity() const;
    void addTrans(const TVec3f &, const char *);
    void cutGravityElementFromJumpVec(bool);
    void cutVecElementFromJumpVec(const TVec3f &);
    TVec3f& getJumpVec() const;
    void setJumpVec(const TVec3f &);
    void playEffect(const char *);
    void playEffectTarns(const char *, const TVec3f &);
    void playEffectRT(const char *, const TVec3f &, const TVec3f &);
    void playEffectRTZ(const char *, const TVec3f &, const TVec3f &);
    void playEffectRTW(const char *, const TVec3f &, const TVec3f &);
    void playEffectSRT(const char *, f32, const TVec3f &, const TVec3f &);
    void stopEFfect(const char *);
    void stopEffectForce(const char *);
    void playSound(const char *, s32);
    void stopSound(const char *, u32);
    void startCamVib(u32);
    TVec3f& getCamPos() const;
    TVec3f& getCamDirX() const;
    TVec3f& getCamDirY() const;
    TVec3f& getCamDirZ() const;
    void startPadVib(u32);
    void startPadVib(const char *);
    f32 getStickX() const;
    f32 getStickY() const;
    f32 getStickP() const;
    bool checkTrgA() const;
    bool checkLvlA() const;
    bool checkTrgZ() const;
    bool checkLvlZ() const;
    bool checkPreLvlZ() const;
    bool checkSquat(bool);
    TVec3f& getTrans() const;
    const Triangle* getGroundPolygon() const;
    bool isOnSlipGround() const;
    bool isSlipFloorCode(s32) const;
    bool isSlipPolygon(const Triangle *) const;
    u32 getFloorCode() const;
    void calcPolygonAngleD(const Triangle *) const;
    void calcAngleD(const TVec3f &) const;
    Triangle* getTmpPolygon() const;
    bool sendPunch(HitSensor *, bool);
    HitSensor* getSensor(const Triangle *) const;
    f32 marioAcos(f32) const;
    bool isInputDisable() const;

    MarioActor* mActor;     // _4
};
