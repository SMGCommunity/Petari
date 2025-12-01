#include "Game/MapObj/Tsukidashikun.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/Spine.hpp"
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

Tsukidashikun::Tsukidashikun(const char* pName) : MapObjActor(pName) {
    _C4 = 10.0;
    mTimer = 120;
}

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
    MR::getJMapInfoArg0NoInit(rIter, &mTimer);
    MR::moveCoordToNearestPos(this, mPosition);
}

void Tsukidashikun::exeWait() {
    if (MR::isFirstStep(this)) {
        if (isNerve(&NrvTsukidashikun::TsukidashikunNrvWaitBack::sInstance)) {
            MR::startBva(this, "BWait");
        }
        else {
            MR::startBva(this, "BSkip");
        }
    }
    if (MR::isGreaterEqualStep(this, mTimer)) {
        if (isNerve(&NrvTsukidashikun::TsukidashikunNrvWaitBack::sInstance)) {
            setNerve(&NrvTsukidashikun::TsukidashikunNrvSignForward::sInstance);
        }
        else {
            setNerve(&NrvTsukidashikun::TsukidashikunNrvSignBack::sInstance);
        }
    }    
}

void Tsukidashikun::exeSign() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "BSign", nullptr);
        if (isNerve(&NrvTsukidashikun::TsukidashikunNrvSignForward::sInstance)) {
            MR::startBva(this, "BSign");
        }
        else {
            MR::startBva(this, "BWait");
        }
    }
    TVec3f railEnd = MR::getRailPointPosEnd(this);
    TVec3f railStart = MR::getRailPointPosStart(this);
    TVec3f* playerPos = MR::getPlayerPos();
    f32 f1 = MR::calcPerpendicFootToLineInside(playerPos, railEnd, railStart, mPosition);
    if (MR::isNearPlayer(mPosition, f1)) {
        MR::tryRumblePadWeak(this, 0);
    }
    MR::startLevelSound(this, "SE_OJ_LV_TSUKIDASHI_VIB", -1, -1, -1);
    if (MR::isStep(this, 0)) {
        MR::setBckFrameAndStop(this, 0.0f);
        if (isNerve(&NrvTsukidashikun::TsukidashikunNrvSignForward::sInstance)) {
            setNerve(&NrvTsukidashikun::TsukidashikunNrvMoveForward::sInstance);
        }
        else {
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

namespace NrvTsukidashikun {
    INIT_NERVE(TsukidashikunNrvRelax);

    void TsukidashikunNrvRelax::execute(Spine* pSpine) const {
        Tsukidashikun* pActor = (Tsukidashikun*)pSpine->mExecutor;
        if (MR::isFirstStep(pActor)) {
            MR::startBva(pActor, "BSign");
        }
    }
}
