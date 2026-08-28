#pragma once

#include "Game/LiveActor/PartsModel.hpp"
#include "Game/NPC/NPCActor.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"

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

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

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

    /* 0x24 */ virtual void appear() {
        LiveActor::appear();
        MR::startBtk(this, "KouraShine");
        MR::emitEffect(this, "HandyGlow");
    }

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter) {
        initEffectKeeper(8, "koura", false);
        initSound(8, false);
        MR::invalidateClipping(this);
        makeActorAppeared();
    }
};
