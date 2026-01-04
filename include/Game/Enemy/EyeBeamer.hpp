#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"

class VolumeModelDrawer;
class ModelObj;
class MapPartsRailMover;

class EyeBeamer : public LiveActor {
public:
    EyeBeamer(const char*);

    virtual ~EyeBeamer();
    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void draw() const;
    virtual void calcAnim();
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);

    void initStartNerve(const JMapInfoIter& rVec);
    void initModel();
    void initRailMoveFunction(const JMapInfoIter& rVec);
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

    MapPartsRailMover* mRailMover;         // 0x8C
    ModelObj* mBeamBloom;                  // 0x90
    ModelObj* mBeamMdl;                    // 0x94
    VolumeModelDrawer* mBeamVolumeDrawer;  // 0x98
    TMtx34f _9C;
    TQuat4f _CC;
    TQuat4f _DC;
    TVec3f _EC;
    TVec3f _F8;
    TVec3f _104;
    TPos3f mWaterSurfaceMtx;  // 0x110
    TVec3f _140;
    u32 _14C;
    u32 _150;
    u32 _154;
    u32 _158;
    f32 _15C;
    f32 _160;
    bool mIsInMercatorCube;  // 0x164
};
