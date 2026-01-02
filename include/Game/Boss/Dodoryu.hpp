#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

struct DodoryuChaseParam;
class DodoryuStateBase;

class Dodoryu : public LiveActor {
public:
    Dodoryu(const char*);

    void setHillAppearNumMax();
    void shiftMoveStateNull();
    void invalidateStarPieceSensor();
    void validateStarPieceSensor();
    void stopHillForce();
    void stopHill();
    void startHill();
    void startLeadHillBck(const char*);
    void killLeadHill();
    void shiftMoveStateChase(f32, f32, f32, f32);
    void setMtx(const TPos3f&);
    void snapToGround();
    void tryRumblePad();
    bool keepOffFromClosedArea(TVec3f*);
    void shiftMoveStateJumpOut(f32, f32);
    void shiftMoveStateEscape(f32, s32, s32, f32, f32);
    bool isHeadNeedle(HitSensor*, HitSensor*) const;
    void reactJumpOutCommon() const;
    void nextState();
    void startSpinOutCamera();
    void endSpinOutCamera();
    void startPlayerCenterCamera();
    void endPlayerCenterCamera();

    /* 0x8C */ TPos3f mBaseMtx;
};

struct DodoryuAnimSet {
    const char* mBck;
    const char* mBtk;
    const char* mBrk;
    const char* mBva;
    const char* mLeadHillBck;
};
