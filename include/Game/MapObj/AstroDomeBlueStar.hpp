#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/EffectUtil.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class GCaptureRibbon;
class CaptureActor;
class ActorCameraInfo;

class AstroDomeBlueStar : public LiveActor {
public:
    class CaptureActor : public LiveActor {
    public:
        CaptureActor();

        virtual ~CaptureActor();
        virtual void init(const JMapInfoIter&);
        virtual void kill();

        void updateTransTraction(const TVec3f&, const TVec3f&, s32);

        inline void forceKill() {
            MR::forceDeleteEffectAll(this);
            LiveActor::kill();
        };

        inline void setPosAll(TVec3f& mPos) {
            mPosition.set(mPos);
            _8C.setTrans(mPosition);
        }

        TPos3f _8C;
    };

    AstroDomeBlueStar(const char*);

    virtual ~AstroDomeBlueStar();
    virtual void init(const JMapInfoIter&);
    virtual void draw() const;
    virtual void appear();
    virtual void kill();
    virtual void control();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

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
    /* 0x090 */ TPos3f _90;
    /* 0x0C0 */ TPos3f _C0;
    /* 0x0F0 */ ActorCameraInfo* _F0;
    /* 0x0F4 */ TVec3f mZoomPos;
    /* 0x100*/ CaptureActor* mCaptureActor;
};
