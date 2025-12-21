#include "Game/MapObj/Tsukidashikun.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjActor.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"

namespace NrvTsukidashikun {
    NEW_NERVE(TsukidashikunNrvRelax, Tsukidashikun, Relax);
    NEW_NERVE(TsukidashikunNrvWaitBack, Tsukidashikun, Wait);
    NEW_NERVE(TsukidashikunNrvWaitForward, Tsukidashikun, Wait);
    NEW_NERVE(TsukidashikunNrvSignBack, Tsukidashikun, Sign);
    NEW_NERVE(TsukidashikunNrvSignForward, Tsukidashikun, Sign);
    NEW_NERVE(TsukidashikunNrvMoveBack, Tsukidashikun, Move);
    NEW_NERVE(TsukidashikunNrvMoveForward, Tsukidashikun, Move);
};  // namespace NrvTsukidashikun

Tsukidashikun::Tsukidashikun(const char* pName) : MapObjActor(pName), _C4(10.0f), mTimer(120) {}

void Tsukidashikun::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    info.setupHioNode("ツキダシドン");
    info.setupDefaultPos();
    info.setupConnectToScene();
    info.setupEffect(0);
    info.setupSound(4);
    info.setupGroupClipping(8);
    info.setupNerve(&NrvTsukidashikun::TsukidashikunNrvRelax::sInstance);
    initialize(rIter, info);
    MR::initLightCtrl(this);
    MR::getJMapInfoArg0NoInit(rIter, &_C4);
    MR::getJMapInfoArg1NoInit(rIter, &mTimer);
    MR::moveCoordToNearestPos(this, mPosition);
}

void Tsukidashikun::exeWait() {
    if (MR::isFirstStep(this)) {
        if (isNerve(&NrvTsukidashikun::TsukidashikunNrvWaitBack::sInstance)) {
            MR::startBva(this, "BWait");
        } else {
            MR::startBva(this, "FWait");
        }
    }
    if (MR::isGreaterEqualStep(this, mTimer)) {
        if (isNerve(&NrvTsukidashikun::TsukidashikunNrvWaitBack::sInstance)) {
            setNerve(&NrvTsukidashikun::TsukidashikunNrvSignForward::sInstance);
        } else {
            setNerve(&NrvTsukidashikun::TsukidashikunNrvSignBack::sInstance);
        }
    }
}

void Tsukidashikun::exeSign() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "BWait", nullptr);
        if (isNerve(&NrvTsukidashikun::TsukidashikunNrvSignForward::sInstance)) {
            MR::startBva(this, "FSign");
        } else {
            MR::startBva(this, "BSign");
        }
    }

    TVec3f pos;
    MR::calcPerpendicFootToLineInside(&pos, *MR::getPlayerPos(), MR::getRailPointPosStart(this), MR::getRailPointPosEnd(this));
    if (MR::isNearPlayer(pos, 700.0f)) {
        MR::tryRumblePadWeak(this, 0);
    }
    MR::startLevelSound(this, "SE_OJ_LV_TSUKIDASHI_VIB", -1, -1, -1);
    if (MR::isStep(this, 60)) {
        MR::setBckFrameAndStop(this, 0.0f);
        if (isNerve(&NrvTsukidashikun::TsukidashikunNrvSignForward::sInstance)) {
            setNerve(&NrvTsukidashikun::TsukidashikunNrvMoveForward::sInstance);
        } else {
            setNerve(&NrvTsukidashikun::TsukidashikunNrvMoveBack::sInstance);
        }
    }
}

void Tsukidashikun::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_TSUKIDASHI_START", -1, -1);
    }
    MR::moveCoordAndFollowTrans(this, _C4);
    MR::startLevelSound(this, "SE_OJ_LV_TSUKIDASHI_MOVE", -1, -1, -1);
    if (MR::isRailReachedGoal(this)) {
        MR::startLevelSound(this, "SE_OJ_TSUKIDASHI_STOP", -1, -1, -1);
        MR::reverseRailDirection(this);
        if (isNerve(&NrvTsukidashikun::TsukidashikunNrvMoveForward::sInstance)) {
            setNerve(&NrvTsukidashikun::TsukidashikunNrvWaitForward::sInstance);
        } else {
            setNerve(&NrvTsukidashikun::TsukidashikunNrvWaitBack::sInstance);
        }
    }
}

void Tsukidashikun::exeRelax() {
    if (MR::isFirstStep(this)) {
        MR::startBva(this, "BWait");
    }
}

void Tsukidashikun::connectToScene(const MapObjActorInitInfo& rInfo) {
    MR::connectToSceneCollisionMapObjStrongLight(this);
}

void Tsukidashikun::initCaseUseSwitchB(const MapObjActorInitInfo& rInfo) {
    MR::listenStageSwitchOnB(this, MR::Functor_Inline(this, &Tsukidashikun::startMove));
}

void Tsukidashikun::initCaseNoUseSwitchB(const MapObjActorInitInfo& rInfo) {
    setNerve(&NrvTsukidashikun::TsukidashikunNrvWaitBack::sInstance);
}

void Tsukidashikun::startMove() {
    setNerve(&NrvTsukidashikun::TsukidashikunNrvWaitBack::sInstance);
}
