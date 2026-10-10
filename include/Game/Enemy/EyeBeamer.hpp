#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class MapPartsRailMover;
class ModelObj;
class VolumeModelDrawer;

class EyeBeamer : public LiveActor {
public:
    EyeBeamer(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void draw() const;
    virtual void calcAnim();
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);

    void initStartNerve(const JMapInfoIter& rIter);
    void initModel();
    void initRailMoveFunction(const JMapInfoIter& rIter);
    void updatePoseAndTrans();
    void updateWaterSurfaceMtx();
    void requestStartPatrol();
    bool tryGotoPatrol();
    bool tryPatrol();
    void exeDemoStartWait();
    void exeDemoWait();
    void exeDemoTurn();
    void exeDemoGotoPatrol();
    void exeWait();
    void exeTurn();
    void exeGotoPatrol();
    void exePatrol();
    bool isInBeamRange(const TVec3f& rVec) const;
    bool isOnBeam() const;

    /* 0x08C */ MapPartsRailMover* mRailMover;
    /* 0x090 */ ModelObj* mBeamBloom;
    /* 0x094 */ ModelObj* mBeamMdl;
    /* 0x098 */ VolumeModelDrawer* mBeamVolumeDrawer;
    /* 0x09C */ TPos3f _9C;
    /* 0x0CC */ TQuat4f _CC;
    /* 0x0DC */ TQuat4f _DC;
    /* 0x0EC */ TVec3f _EC;
    /* 0x0F8 */ TVec3f _F8;
    /* 0x104 */ TVec3f _104;
    /* 0x110 */ TPos3f mWaterSurfaceMtx;
    /* 0x140 */ TVec3f _140;
    /* 0x14C */ u32 _14C;
    /* 0x150 */ u32 _150;
    /* 0x154 */ u32 _154;
    /* 0x158 */ u32 _158;
    /* 0x15C */ f32 _15C;
    /* 0x160 */ f32 _160;
    /* 0x164 */ bool mIsInMercatorCube;
};
