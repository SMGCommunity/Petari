#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AstroDomeCameraController : public LiveActor {
public:

    class Position {
    public:
        Position();

        void reset(const TVec3f &);

        TVec3f _0;
        TVec3f _C;
        TVec3f _18;
    };

    AstroDomeCameraController(const char *);

    virtual ~AstroDomeCameraController();
    virtual void init(const JMapInfoIter &);
    virtual void appear();
    virtual void control();
    virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);

    void calcZoomOutPos(TVec3f *) const;
    void calcZoomInPos(TVec3f *, const TVec3f &) const;
    void calcZoomInTarget(TVec3f *, const TVec3f &) const;
    void calcZoomInUp(TVec3f *) const;

    void exeGalaxySelectStart();
    void exeGalaxySelect();
    void exeGalaxyConfirmStart();
    void exeGalaxyConfirm();
    void exeGalaxyConfirmCancel();

    TVec3f _8C;
    Position _98;
    Position _BC;
    Position _E0;
    f32 _104;
    TVec3f _108;
};