#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

enum MeteorStrikeType {
    /* 0x1 */ MeteorStrikeType_Environment = 1,
    /* 0x2 */ MeteorStrikeType_Cannon = 2
};

class ModelObj;

class MeteorStrike : public LiveActor {
public:
    MeteorStrike(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void kill();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void appear(s32);
    bool getMovedPos(TVec3f*, s32) const;
    static f32 getSpeed(const JMapInfoIter&);
    void initMapToolInfo(const JMapInfoIter&);
    void initModel();
    void calcBreakPosture(const TVec3f&, TPos3f*);
    void emitEffectColumn(const TPos3f&);
    void startRumble();
    bool isInScreen() const;

    void exeMove();
    void exeBreak();

    /* 0x8C */ s32 mType;
    /* 0x90 */ f32 mStepSize;  // Obj_arg0
    /* 0x94 */ TVec3f mRailDir;
    /* 0xA0 */ ModelObj* mBrokenModel;
    /* 0xA4 */ s32 mStepsLeft;
    /* 0xA8 */ s32 mTotalSteps;
    /* 0xAC */ TMtx34f mLavaColumnPos;
};
