#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class ActorCameraInfo;
class BenefitItemObj;

class SimpleBreakableObj : public MapObjActor {
public:
    SimpleBreakableObj(const char*);

    virtual ~SimpleBreakableObj() NO_INLINE;
    virtual void init(const JMapInfoIter&) override;
    virtual void control() override;
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*) override;
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*) override;
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*) override;
    virtual void initCaseUseSwitchA(const MapObjActorInitInfo&) override;
    virtual void initCaseNoUseSwitchA(const MapObjActorInitInfo&) override {
    }

    virtual void initCaseUseSwitchB(const MapObjActorInitInfo&) override;
    virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&) override {
    }

    void setStateBreak();
    void startBreak();
    void endBreak();
    bool isStateBreaking() const;
    void appearItem();
    void exeWait() {
    }

    void exeTryStartDemo();
    void exeDemo();

    /* 0xC4 */ ActorCameraInfo* mCameraInfo;
    /* 0xC8 */ s32 mBreakOffsetFrame;
    s32 _CC;
    /* 0xD0 */ s32 mSwitchDeadDelay;
    /* 0xD4 */ u16 mBreakFlags;
    /* 0xD8 */ s32 mItemType;
    /* 0xDC */ s32 mItemNum;
    /* 0xE0 */ BenefitItemObj* mKinokoOneUp;
    /* 0xE4 */ bool mPlayRiddleSound;
    /* 0xE5 */ bool mBreakEnded;
    bool _E6;
};

class SimpleBreakableIndirectObj : public SimpleBreakableObj {
public:
    SimpleBreakableIndirectObj(const char* pName) : SimpleBreakableObj(pName) {
    }

    virtual ~SimpleBreakableIndirectObj() {
    }

    virtual void connectToScene(const MapObjActorInitInfo&) override;
};
