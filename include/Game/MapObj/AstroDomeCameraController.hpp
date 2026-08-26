#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AstroDomeCameraController : public LiveActor {
public:
    class Position {
    public:
        Position() {
            mStart.zero();
            mEnd.zero();
            mTarget.zero();
        }

        void setEnd(const TVec3f& rEnd) {
            mStart.set(mTarget);
            mEnd.set(rEnd);
        }

        void reset(const TVec3f& rVec) {
            mStart.set(rVec);
            mEnd.set(rVec);
            mTarget.set(rVec);
        }

        /* 0x00 */ TVec3f mStart;
        /* 0x0C */ TVec3f mEnd;
        /* 0x18 */ TVec3f mTarget;
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

    /* 0x08C */ TVec3f mStartPos;
    /* 0x098 */ Position mZoomPos;
    /* 0x0BC */ Position mTargetPos;
    /* 0x0E0 */ Position mUpPos;
    /* 0x104 */ f32 mEaseRate;
    /* 0x108 */ TVec3f mZeroPos;
};
