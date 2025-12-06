#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"

class GCaptureRibbon;

class AstroDomeBlueStar : public LiveActor {
public:
    class CaptureActor : public LiveActor {
    public:
        CaptureActor();

        virtual ~CaptureActor();
        virtual void init(const JMapInfoIter&);
        virtual void kill();

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

    GCaptureRibbon* mCaptureRibbon;  // 0x8C
    TPos3f _90;
    TPos3f _C0;
    u32 _F0;
    TVec3f mZoomPos;              // 0xF4
    CaptureActor* mCaptureActor;  // 0x100
};
