#pragma once

#include "Game/LiveActor/PartsModel.hpp"
#include "Game/NPC/NPCActor.hpp"

class JetTurtle;

class RemovableTurtle {
public:
    RemovableTurtle(LiveActor*, bool);

    bool isPullRange() const;
    bool tryRemove();
    bool tryAttach();

    /* 0x00 */ PartsModel* mShellModel;
    /* 0x04 */ JetTurtle* mJetTurtle;
};

class Penguin : public NPCActor {
public:
    enum Behavior {
        Behavior_Default = -1,
        Behavior_Sit = 0,
        Behavior_Swim = 1,
        Behavior_SwimSurface = 2,
        Behavior_Dive = 3,
        Behavior_SwimTurtle = 4,
        Behavior_Stand = 5,
        Behavior_Dash = 6
    };

    Penguin(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeReaction();
    void exeWait();
    void exeDive();
    void exeFlow();
    bool tryReleaseTurtle();

    /* 0x15C */ s32 mBehavior;
    /* 0x160 */ s32 mStepToDive;
    /* 0x164 */ RemovableTurtle* mTurtle;
};

class TurtlePartsModel : public PartsModel {
public:
    TurtlePartsModel(LiveActor* pActor, const char* pName, const char* pModelName, MtxPtr mtx, int drawBufferType, bool a6)
        : PartsModel(pActor, pName, pModelName, mtx, drawBufferType, a6) {
    }

    virtual void appear() {
        LiveActor::appear();
        MR::startBtk(this, "KouraShine");
        MR::emitEffect(this, "HandyGlow");
    }

    virtual void init(const JMapInfoIter& rIter) {
        initEffectKeeper(8, "koura", false);
        initSound(8, false);
        MR::invalidateClipping(this);
        makeActorAppeared();
    }
};
