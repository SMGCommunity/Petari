#include "Game/MapObj/TimerMoveWall.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjActor.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <JSystem/JGeometry/TVec.hpp>

void TimerMoveWall_FORCE_MATCH_SDATA2() {
    (void)0.0f;
}

namespace {
    static const s32 sDefaultTimer = 300;
    static const f32 sDefaultMoveLength = 1000.0f;
};  // namespace

namespace NrvTimerMoveWall {
    NEW_NERVE(TimerMoveWallNrvWait, TimerMoveWall, Wait);
    NEW_NERVE(TimerMoveWallNrvMoveFront, TimerMoveWall, Move);
    NEW_NERVE(TimerMoveWallNrvMoveBack, TimerMoveWall, Move);
};  // namespace NrvTimerMoveWall

TimerMoveWall::TimerMoveWall(const char* pName) : MapObjActor(pName), mTimer(::sDefaultTimer), mMoveLenght(::sDefaultMoveLength) {
}

void TimerMoveWall::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    MapObjActorUtil util;
    util.setupInitInfoSimpleMapObj(&info);
    info.setupSound(4);
    info.setupNerve(GET_NERVE(TimerMoveWall, TimerMoveWallNrvWait));
    initialize(rIter, info);
    MR::getJMapInfoArg0NoInit(rIter, &mTimer);
    MR::getJMapInfoArg1NoInit(rIter, &mMoveLenght);
}

inline void TimerMoveWall::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
    }
}

void TimerMoveWall::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(this);
        TVec3f frontVec;
        MR::calcFrontVec(&frontVec, this);

        if (isNerve(GET_NERVE(TimerMoveWall, TimerMoveWallNrvMoveBack))) {
            frontVec.negate();
        }

        mVelocity.scale(mMoveLenght / mTimer, frontVec);
        MR::startSound(this, "SE_OJ_TRIBBL_T_MVWALL_ST");
    }

    MR::startLevelSound(this, "SE_OJ_LV_TRIBBL_T_MVWALL_MV");

    if (MR::isStep(this, mTimer)) {
        mVelocity.set2(0.0f);

        if (isNerve(GET_NERVE(TimerMoveWall, TimerMoveWallNrvMoveFront))) {
            setNerve(GET_NERVE(TimerMoveWall, TimerMoveWallNrvMoveBack));
        } else {
            MR::startSound(this, "SE_OJ_TRIBBL_T_MVWALL_ED");
            setNerve(GET_NERVE(TimerMoveWall, TimerMoveWallNrvWait));
        }
    }
}

void TimerMoveWall::initCaseUseSwitchB(const MapObjActorInitInfo& rInfo) {
    MR::listenStageSwitchOnB(this, MR::Functor(this, &TimerMoveWall::startMove));
}

void TimerMoveWall::startMove() {
    setNerve(GET_NERVE(TimerMoveWall, TimerMoveWallNrvMoveFront));
}
