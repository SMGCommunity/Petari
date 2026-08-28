#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class CubeCameraArea;
class GravityInfo;
class MarioActor;
class Triangle;

class CameraTargetObj : public NameObj {
public:
    CameraTargetObj(const char*);

    /* 0x08 */ virtual ~CameraTargetObj(){};

    /* 0x0C */ virtual void init(const JMapInfoIter&){};

    /* 0x24 */ virtual const TVec3f& getPosition() const = 0;
    /* 0x28 */ virtual const TVec3f& getUpVec() const = 0;
    /* 0x2C */ virtual const TVec3f& getFrontVec() const = 0;
    /* 0x30 */ virtual const TVec3f& getSideVec() const = 0;
    /* 0x34 */ virtual const TVec3f& getLastMove() const = 0;
    /* 0x38 */ virtual const TVec3f& getGroundPos() const = 0;
    /* 0x3C */ virtual const TVec3f& getGravityVector() const = 0;

    virtual f32 getRadius() const {
        return 150.0f;
    }
    /* 0x40 */ virtual bool isTurning() const {
        return false;
    }
    /* 0x44 */ virtual bool isJumping() const {
        return false;
    }
    /* 0x48 */ virtual bool isLongDrop() const {
        return false;
    }
    /* 0x4C */ virtual bool isFastDrop() const {
        return false;
    }
    /* 0x50 */ virtual bool isFastRise() const {
        return false;
    }
    /* 0x54 */ virtual bool isWaterMode() const {
        return false;
    }
    /* 0x58 */ virtual bool isOnWaterSurface() const {
        return false;
    }
    /* 0x5C */ virtual bool isBeeMode() const {
        return false;
    }
    /* 0x60 */ virtual bool isFooFighterMode() const {
        return false;
    }
    virtual u32 getSpecialMode() const {
        return 0;
    }
    /* 0x64 */ virtual bool isCameraStateOn(u32) const {
        return false;
    };
    /* 0x68 */ virtual CubeCameraArea* getCubeCameraArea() const {
        return nullptr;
    }
    /* 0x6C */ virtual Triangle* getGroundTriangle() const {
        return nullptr;
    }
    /* 0x70 */ virtual GravityInfo* getGravityInfo() const {
        return nullptr;
    }
    /* 0x74 */ virtual void enableCameraWall() {
        mCameraWall = true;
    }
    /* 0x78 */ virtual void disableCameraWall() {
        mCameraWall = false;
    }
    /* 0x7C */ virtual void setCameraWall(bool enable) {
        mCameraWall = enable;
    }
    /* 0x80 */ virtual bool isDebugMode() const {
        return false;
    }
    virtual TPos3f* getMapBaseMtx() const {
        return nullptr;
    }
    /* 0x84 */ virtual void resetStatus() {
    }

    /* 0xC */ bool mCameraWall;
};

class CameraTargetActor : public CameraTargetObj {
public:
    CameraTargetActor(const char*);

    /* 0x14 */ virtual void movement();

    /* 0x24 */ virtual const TVec3f& getPosition() const;
    /* 0x28 */ virtual const TVec3f& getUpVec() const;
    /* 0x2C */ virtual const TVec3f& getFrontVec() const;
    /* 0x30 */ virtual const TVec3f& getSideVec() const;
    /* 0x34 */ virtual const TVec3f& getLastMove() const;
    /* 0x38 */ virtual const TVec3f& getGroundPos() const;
    /* 0x3C */ virtual const TVec3f& getGravityVector() const;

    virtual f32 getRadius() const {
        return 150.0f;
    }
    /* 0x68 */ virtual CubeCameraArea* getCubeCameraArea() const;
    /* 0x6C */ virtual Triangle* getGroundTriangle() const;

    /* 0x10 */ const LiveActor* mActor;
    /* 0x14 */ TVec3f mUp;
    /* 0x20 */ TVec3f mFront;
    /* 0x2C */ TVec3f mSide;
    /* 0x38 */ CubeCameraArea* mCameraArea;
};

class CameraTargetPlayer : public CameraTargetObj {
public:
    CameraTargetPlayer(const char*);

    /* 0x14 */ virtual void movement();

    /* 0x24 */ virtual const TVec3f& getPosition() const;
    /* 0x28 */ virtual const TVec3f& getUpVec() const;
    /* 0x2C */ virtual const TVec3f& getFrontVec() const;
    /* 0x30 */ virtual const TVec3f& getSideVec() const;
    /* 0x34 */ virtual const TVec3f& getLastMove() const;
    /* 0x38 */ virtual const TVec3f& getGroundPos() const;
    /* 0x3C */ virtual const TVec3f& getGravityVector() const;

    /* 0x40 */ virtual bool isTurning() const;
    /* 0x44 */ virtual bool isJumping() const;
    /* 0x48 */ virtual bool isLongDrop() const;
    /* 0x4C */ virtual bool isFastDrop() const;
    /* 0x50 */ virtual bool isFastRise() const;
    /* 0x54 */ virtual bool isWaterMode() const;
    /* 0x58 */ virtual bool isOnWaterSurface() const;
    /* 0x60 */ virtual bool isFooFighterMode() const;
    virtual u32 getSpecialMode() const;
    /* 0x64 */ virtual bool isCameraStateOn(u32) const;
    /* 0x68 */ virtual CubeCameraArea* getCubeCameraArea() const;
    /* 0x6C */ virtual Triangle* getGroundTriangle() const;
    /* 0x70 */ virtual GravityInfo* getGravityInfo() const;
    /* 0x80 */ virtual bool isDebugMode() const;
    virtual TPos3f* getMapBaseMtx() const;

    /* 0x10 */ const MarioActor* mActor;
    /* 0x14 */ TVec3f mSide;
    /* 0x20 */ TVec3f mUp;
    /* 0x2C */ TVec3f mFront;
    /* 0x38 */ TVec3f mGravity;
    /* 0x44 */ TVec3f mGroundPos;
    /* 0x50 */ CubeCameraArea* mCameraArea;
    /* 0x54 */ Triangle* mGroundTriangle;
    /* 0x58 */ u16 mPlayerMovementTimer;
    /* 0x5A */ bool mIsPlayerMoving;
};

class CameraTargetDemoActor : public LiveActor {
public:
    CameraTargetDemoActor(MtxPtr, const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x38 */ virtual MtxPtr getBaseMtx() const {
        return (const MtxPtr)mMtx.mMtx;
    }

    void setTargetMtx(MtxPtr);

    /* 0x8C */ TPos3f mMtx;
};
