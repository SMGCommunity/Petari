#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/Util/Array.hpp"

class AnimScaleController;
class AnimScaleParam;
class DharmaSamboParts;
class FixedPosition;
class SpinHitController;

class DharmaSambo : public LiveActor {
public:
    DharmaSambo(const char*);

    virtual ~DharmaSambo();
    virtual void init(const JMapInfoIter&) override;
    virtual void initAfterPlacement() override;
    virtual void appear() override;
    virtual void kill() override;
    virtual void control() override;
    virtual void calcAndSetBaseMtx() override;

    void exeWaitUnderGround();
    void exeAppear();
    void exePursue();
    void endPursue();
    void exeAttack();
    void exeSpinHitted();
    void exeStarPointerBind();
    void endStarPointerBind();
    void tearDownStarPointerBind();
    void exeEscapeRun();
    void exeEscapeLand();
    void exeTrampleDeath();
    void exeHide();
    void exeHideAppear();
    void exeKilled();
    DharmaSamboParts* getHeadParts();
    bool isActionEndHead();
    void startActionHead(const char*);
    void startActionAllParts(const char*);
    void startBtkAllParts(const char*);
    bool decCountAndTryToHide(bool);
    bool isStarPointerPointingParts();

    /* 0x8C */ const Nerve* mStarPointerBindEndNerve;
    /* 0x90 */ TVec3f mInitialPos;
    /* 0x9C */ s32 mHideTimer;
    /* 0xA0 */ s32 mPartsCount;
    /* 0xA4 */ MR::Vector< MR::AssignableArray< DharmaSamboParts* > > mParts;
    /* 0xB0 */ MR::Vector< MR::AssignableArray< FixedPosition* > > mFixedPositions;
    /* 0xBC */ HitSensor* mHitSensor;
    /* 0xC0 */ AnimScaleController* mAnimScaleController;
    /* 0xC4 */ AnimScaleParam* mAnimScaleParam;
};

class DharmaSamboParts : public ModelObj {
public:
    DharmaSamboParts(DharmaSambo*, const char*, const char*, MtxPtr, int, int, int);

    virtual ~DharmaSamboParts();
    virtual void init(const JMapInfoIter&) override;
    virtual void appear() override;
    virtual void kill() override;
    virtual void control() override;
    virtual void calcAndSetBaseMtx() override;
    virtual void attackSensor(HitSensor*, HitSensor*) override;
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*) override;

    void exeDoNothing();
    void exeBlow();
    void exeStarPointerBind();
    void endStarPointerBind();
    void setHead(bool);

    /* 0x90 */ DharmaSambo* mHost;
    /* 0x94 */ s32 mOriginalIndex;
    /* 0x98 */ s32 mIndex;
    /* 0x9C */ SpinHitController* mSpinHitController;
    /* 0xA0 */ f32 mHitDistance;
    /* 0xA4 */ bool mIsHead;
};
