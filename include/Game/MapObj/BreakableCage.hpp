#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class ActorCameraInfo;
class DummyDisplayModel;
class ModelObj;

class BreakableCage : public LiveActor {
public:
    BreakableCage(const char*);

    enum CageType { CAGE_INVALID = 0, CAGE_NORMAL = 1, CAGE_LARGE = 2, CAGE_FIX = 3, CAGE_TRASH = 4 };

    /* 0x08 */ virtual ~BreakableCage();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual void appear();
    /* 0x2C */ virtual void kill();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    const ActorCameraInfo* getCamInfo() const {
        return mCameraInfo;
    }

    void initMapToolInfo(const JMapInfoIter&);
    void initModel(const char*, const JMapInfoIter&);
    void initBaseMtxForCage();
    bool isTypeCage() const;
    bool isAppearPowerStar() const;
    bool tryBreak();

    void exeWait();
    void exeWaitStartDemoBreak();
    void exeBreak();

    TPos3f mMtx;                         // 0x8C
    ModelObj* mBreakModel;               // 0xBC
    CageType mCageType;                  // 0xC0
    f32 mRotationSpeed;                  // 0xC4
    DummyDisplayModel* mItemModel;       // 0xC8
    bool mIgnoreGravity;                 // 0xCC
    bool mDelayDeadActivate;             // 0xCD
    const ActorCameraInfo* mCameraInfo;  // 0xD0
};
