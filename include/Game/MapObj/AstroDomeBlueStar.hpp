#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/EffectUtil.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class ActorCameraInfo;
class GCaptureRibbon;

class AstroDomeBlueStar : public LiveActor {
public:
    class CaptureActor : public LiveActor {
    public:
        /// @brief Creates a new `CaptureActor`.
        CaptureActor();

        /* 0x0C */ virtual void init(const JMapInfoIter&);
        /* 0x2C */ virtual void kill();

        void updateTransTraction(const TVec3f&, const TVec3f&, s32);

        void forceKill() {
            MR::forceDeleteEffectAll(this);
            LiveActor::kill();
        };

        void setPosAll(TVec3f& mPos) {
            mPosition.set(mPos);
            mHostMtx.setTrans(mPosition);
        }

        /* 0x8C */ TPos3f mHostMtx;
    };

    /// @brief Creates a new `AstroDomeBlueStar`.
    /// @param pName A pointer to the null-terminated name of the object.
    AstroDomeBlueStar(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x18 */ virtual void draw() const;
    /* 0x24 */ virtual void appear();
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void waitAppear();
    void forceKill();
    bool tryStartBind(const LiveActor*);
    bool isActiveBind() const;
    bool isValidBindStart() const;
    void calcZoomInPos(TVec3f*) const;

    void exeAppear();
    void exeWait();
    void exePointing();
    void exeTryStartBind();
    void exeBindTraction();
    void exeBindHold();
    void exeBindEnd();
    void exeGalaxySelect();
    void exeGalaxyConfirmStart();
    void exeGalaxyConfirmCancel();
    void exeGalaxyConfirm();

    /* 0x08C */ GCaptureRibbon* mCaptureRibbon;
    /* 0x090 */ TPos3f mHostMtx;
    /* 0x0C0 */ TPos3f mBindStartMtx;
    /* 0x0F0 */ ActorCameraInfo* mCameraInfo;
    /* 0x0F4 */ TVec3f mZoomPos;
    /* 0x100 */ CaptureActor* mCaptureActor;
};
