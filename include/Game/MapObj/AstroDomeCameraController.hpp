#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AstroDomeCameraController : public LiveActor {
public:
    class Position {
    public:
        Position() {
            _0.zero();
            _C.zero();
            _18.zero();
        }

        void reset(const TVec3f& rVec) {
            _0.set(rVec);
            _C.set(rVec);
            _18.set(rVec);
        }

        /* 0x00 */ TVec3f _0;
        /* 0x0C */ TVec3f _C;
        /* 0x18 */ TVec3f _18;
    };

    AstroDomeCameraController(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void kill();
    virtual void control();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void calcZoomOutPos(TVec3f*) const;
    void calcZoomInPos(TVec3f*, const TVec3f&) const;
    void calcZoomInTarget(TVec3f*, const TVec3f&) const;
    void calcZoomInUp(TVec3f*) const;

    void exeGalaxySelectStart();
    void exeGalaxySelect();
    void exeGalaxyConfirmStart();
    void exeGalaxyConfirm();
    void exeGalaxyConfirmCancel();

    /* 0x08C */ TVec3f _8C;
    /* 0x098 */ Position _98;
    /* 0x0BC */ Position _BC;
    /* 0x0E0 */ Position _E0;
    /* 0x104 */ f32 _104;
    /* 0x108 */ TVec3f _108;
};
