#include "Game/MapObj/Tsukidashikun.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

void Tsukidashikun_FORCE_MATCH_SDATA2() {
    (void)0.0f;
}

namespace {
    static const f32 sDefaultMoveSpeed = 10.0f;
    static const s32 sDefaultWaitTime = 120;
    static const s32 sStepForSign = 60;
    static const f32 sRumblePadLength = 700.0f;
};  // namespace

namespace NrvTsukidashikun {
    NEW_NERVE(TsukidashikunNrvRelax, Tsukidashikun, Relax);
    NEW_NERVE(TsukidashikunNrvWaitBack, Tsukidashikun, Wait);
    NEW_NERVE(TsukidashikunNrvWaitForward, Tsukidashikun, Wait);
    NEW_NERVE(TsukidashikunNrvSignBack, Tsukidashikun, Sign);
    NEW_NERVE(TsukidashikunNrvSignForward, Tsukidashikun, Sign);
    NEW_NERVE(TsukidashikunNrvMoveBack, Tsukidashikun, Move);
    NEW_NERVE(TsukidashikunNrvMoveForward, Tsukidashikun, Move);
};  // namespace NrvTsukidashikun

Tsukidashikun::Tsukidashikun(const char* pName) : MapObjActor(pName), mMoveSpeed(::sDefaultMoveSpeed), mWaitTimer(::sDefaultWaitTime) {
}

void Tsukidashikun::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    info.setupHioNode("地形オブジェ");
    info.setupDefaultPos();
    info.setupConnectToScene();
    info.setupEffect(0);
    info.setupSound(4);
    info.setupGroupClipping(8);
    info.setupNerve(GET_NERVE(Tsukidashikun, TsukidashikunNrvRelax));
    initialize(rIter, info);
    MR::initLightCtrl(this);
    MR::getJMapInfoArg0NoInit(rIter, &mMoveSpeed);
    MR::getJMapInfoArg1NoInit(rIter, &mWaitTimer);
    MR::moveCoordToNearestPos(this, mPosition);
}

void Tsukidashikun::exeRelax() {
    if (MR::isFirstStep(this)) {
        MR::startBva(this, "BSign");
    }
}

void Tsukidashikun::exeWait() {
    if (MR::isFirstStep(this)) {
        if (isNerve(GET_NERVE(Tsukidashikun, TsukidashikunNrvWaitBack))) {
            MR::startBva(this, "BWait");
        } else {
            MR::startBva(this, "FWait");
        }
    }

    if (MR::isGreaterEqualStep(this, mWaitTimer)) {
        if (isNerve(GET_NERVE(Tsukidashikun, TsukidashikunNrvWaitBack))) {
            setNerve(GET_NERVE(Tsukidashikun, TsukidashikunNrvSignForward));
        } else {
            setNerve(GET_NERVE(Tsukidashikun, TsukidashikunNrvSignBack));
        }
    }
}

void Tsukidashikun::exeSign() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Sign");

        if (isNerve(GET_NERVE(Tsukidashikun, TsukidashikunNrvSignForward))) {
            MR::startBva(this, "FSign");
        } else {
            MR::startBva(this, "BSign");
        }
    }

    TVec3f pos;
    MR::calcPerpendicFootToLineInside(&pos, *MR::getPlayerPos(), MR::getRailPointPosStart(this), MR::getRailPointPosEnd(this));

    if (MR::isNearPlayer(pos, ::sRumblePadLength)) {
        MR::tryRumblePadWeak(this, WPAD_CHAN0);
    }

    MR::startLevelSound(this, "SE_OJ_LV_TSUKIDASHI_VIB");

    if (MR::isStep(this, ::sStepForSign)) {
        MR::setBckFrameAndStop(this, 0.0f);

        if (isNerve(GET_NERVE(Tsukidashikun, TsukidashikunNrvSignForward))) {
            setNerve(GET_NERVE(Tsukidashikun, TsukidashikunNrvMoveForward));
        } else {
            setNerve(GET_NERVE(Tsukidashikun, TsukidashikunNrvMoveBack));
        }
    }
}

void Tsukidashikun::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_TSUKIDASHI_START");
    }

    MR::moveCoordAndFollowTrans(this, mMoveSpeed);
    MR::startLevelSound(this, "SE_OJ_LV_TSUKIDASHI_MOVE");

    if (MR::isRailReachedGoal(this)) {
        MR::startLevelSound(this, "SE_OJ_TSUKIDASHI_STOP");
        MR::reverseRailDirection(this);

        if (isNerve(GET_NERVE(Tsukidashikun, TsukidashikunNrvMoveForward))) {
            setNerve(GET_NERVE(Tsukidashikun, TsukidashikunNrvWaitForward));
        } else {
            setNerve(GET_NERVE(Tsukidashikun, TsukidashikunNrvWaitBack));
        }
    }
}

void Tsukidashikun::connectToScene(const MapObjActorInitInfo& rInfo) {
    MR::connectToSceneCollisionMapObjStrongLight(this);
}

void Tsukidashikun::initCaseUseSwitchB(const MapObjActorInitInfo& rInfo) {
    MR::listenStageSwitchOnB(this, MR::Functor(this, &Tsukidashikun::startMove));
}

void Tsukidashikun::initCaseNoUseSwitchB(const MapObjActorInitInfo& rInfo) {
    setNerve(GET_NERVE(Tsukidashikun, TsukidashikunNrvWaitBack));
}

void Tsukidashikun::startMove() {
    setNerve(GET_NERVE(Tsukidashikun, TsukidashikunNrvWaitBack));
}
