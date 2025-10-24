#include "Game/NPC/KoopaJr.hpp"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/NPCUtil.hpp"

namespace NrvKoopaJr {
    NEW_NERVE(HostTypeWait, KoopaJr, Wait);
    NEW_NERVE(HostTypeReaction, KoopaJr, Reaction);
    NEW_NERVE(HostTypeReactionEnd, KoopaJr, ReactionEnd);
    NEW_NERVE(HostTypeShipBattleAppear, KoopaJr, ShipBattleAppear);
    NEW_NERVE(HostTypeShipBattlePowerUp, KoopaJr, ShipBattlePowerUp);
    NEW_NERVE(HostTypeShipBattleEscape, KoopaJr, ShipBattleEscape);
    NEW_NERVE(HostTypeShipBattleShipDamage, KoopaJr, ShipBattleShipDamage);
    NEW_NERVE(HostTypeShipBattleDemoTalkStart, KoopaJr, ShipBattleDemoTalkStart);
    NEW_NERVE(HostTypeShipBattleDemoTalkWait, KoopaJr, ShipBattleDemoTalkWait);
};

KoopaJr::KoopaJr(const char *pName) : NPCActor(pName) {
    _15C = 0;
    _15D = 0;
}

void KoopaJr::init(const JMapInfoIter &rIter) {
    NPCActor::init(rIter);
    initModelManagerWithAnm("KoopaJr", nullptr, false);
    NPCActorCaps caps("KoopaJr");
    caps._D = 1;
    caps.mConnectsToScene = 1;
    caps._35 = 1;
    caps._36 = 1;
    caps.mUseLightCtrl = 1;
    caps.mUseSounds = 1;
    caps._30 = 4;
    caps.mUseHitSensors = 1;
    caps.mSensorSize = 175.0f;
    caps.mSensorOffset.y = 175.0f;
    caps.mUsesNerves = 1;
    caps.mWaitNerve = &NrvKoopaJr::HostTypeWait::sInstance;
    caps._E = 1;
    caps._5C = 1;
    caps._60 = 100.0f;
    caps.mUseEffect = 1;
    mParam.setSingleAction("Wait");
    mParam._0 = 0;
    mParam._1 = 0;
    _13C = "Damage";
    initialize(rIter, caps);
    MR::declareStarPiece(this, 20);
    if (MR::tryRegisterDemoCast(this, rIter)) {
        MR::tryRegisterDemoActionFunctor(this, MR::Functor_Inline(this, &KoopaJr::startShipBattleTalk), "クッパJr会話");
        if (mLodCtrl != nullptr) {
            if (mLodCtrl->_10) {
                MR::tryRegisterDemoCast(mLodCtrl->_10, rIter);
            }

            if (mLodCtrl->_14) {
                MR::tryRegisterDemoCast(mLodCtrl->_14, rIter);
            }
        }
    }

    if (mLodCtrl != nullptr) {
        mLodCtrl->setDistanceToMiddleAndLow(5000.0f, 10000.0f);
    }
    
    MR::onCalcShadow(this, nullptr);
    MR::setShadowDropLength(this, nullptr, 200.0f);
    makeActorAppeared();
    MR::startBva(this, "Apron");
    if (mLodCtrl != nullptr) {
        if (mLodCtrl->_10 != nullptr) {
            MR::startBva(mLodCtrl->_10, "Apron");
        }

        if (mLodCtrl->_14 != nullptr) {
            MR::startBva(mLodCtrl->_14, "Apron");
        }
    }
}

void KoopaJr::kill() {
    if (mLodCtrl != nullptr) {
        mLodCtrl->invalidate();
    }

    LiveActor::kill();
}

void KoopaJr::setStateShipBattleAppear() {
    setNerve(&NrvKoopaJr::HostTypeShipBattleAppear::sInstance);
}

void KoopaJr::setStateShipBattlePowerUp() {
    setNerve(&NrvKoopaJr::HostTypeShipBattlePowerUp::sInstance);
}

void KoopaJr::setStateShipBattleEscape() {
    setNerve(&NrvKoopaJr::HostTypeShipBattleEscape::sInstance);
}

void KoopaJr::setStateShipBattleShipDamage() {
    setNerve(&NrvKoopaJr::HostTypeShipBattleShipDamage::sInstance);
}

void KoopaJr::endShipBattleTalk() {
    setNerve(mWaitNerve);
}

void KoopaJr::control() {
    NPCActor::control();
    turnToPlayer();
}

void KoopaJr::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _A0);
}

void KoopaJr::startShipBattleTalk() {
    if (!_15D) {
        _15D = 1;
        MR::tryTalkTimeKeepDemo(mMsgCtrl);
        setNerve(&NrvKoopaJr::HostTypeShipBattleDemoTalkStart::sInstance);
    }
}

void KoopaJr::exeWait() {
    if (MR::tryStartReaction(this)) {
        if (!_E6) {
            MR::startSound(this, "SE_OJ_STAR_PIECE_HIT_ENM_F", -1, -1);
        }

        setNerve(&NrvKoopaJr::HostTypeReaction::sInstance);
    }
    else {
        MR::tryStartTurnAction(this);
    }
}

void KoopaJr::exeReaction() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Damage", nullptr);
        MR::startSound(this, "SE_BV_KOOPAJR_DAMAGE_S", -1, -1);
        if (!_15C) {
            if (MR::appearStarPiece(this, mPosition, 20, 10.0f, 40.0f, false)) {
                MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
            }
        }
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvKoopaJr::HostTypeReactionEnd::sInstance);
    }
}

void KoopaJr::exeReactionEnd() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Recovery", nullptr);
    }

    if (MR::isBckStopped(this)) {
        setNerve(mWaitNerve);
    }
}

void KoopaJr::exeShipBattleAppear() {
    if (MR::isFirstStep(this)) {
        if (MR::isEqualStageName("TriLegLv1Galaxy")) {
            MR::startBck(this, "RoboAppearDemo", nullptr);
        }
        else {
            MR::startBck(this, "AppearDemo", nullptr);
        }
    }

    if (MR::isBckStopped(this)) {
        setNerve(mWaitNerve);
    }
}

void KoopaJr::exeShipBattlePowerUp() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "AngryDemo", nullptr);
    }

    if (MR::isBckStopped(this)) {
        mParam.setSingleAction("WaitAngry");
        setNerve(mWaitNerve);
    }
}

void KoopaJr::exeShipBattleEscape() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "RunAwayDemo", nullptr);
    }

    if (MR::isBckStopped(this)) {
        setNerve(mWaitNerve);
    }
}

void KoopaJr::exeShipBattleShipDamage() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Stumble", nullptr);
    }

    if (MR::isBckStopped(this)) {
        setNerve(mWaitNerve);
    }
}

void KoopaJr::exeShipBattleDemoTalkStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Talk", nullptr);
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvKoopaJr::HostTypeShipBattleDemoTalkWait::sInstance);
    }
}

void KoopaJr::exeShipBattleDemoTalkWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "TalkWait", nullptr);
    }
}

KoopaJr::~KoopaJr() {
    
}
