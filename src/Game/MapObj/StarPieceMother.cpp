#include "Game/MapObj/StarPieceMother.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/StarPiece.hpp"
#include "Game/Util.hpp"

namespace {
    // const f32 hMoveSpeed = _;
    const f32 hAddRandomVectorValue = 0.5f;
    const f32 hAddRotateY = 10.0f;
};  // namespace

namespace NrvStarPieceMother {
    NEW_NERVE(HostTypeNrvMoveOnRail, StarPieceMother, MoveOnRail);
    NEW_NERVE(HostTypeNrvWait, StarPieceMother, Wait);
};  // namespace NrvStarPieceMother

StarPieceMother::StarPieceMother(const char* pName) : LiveActor(pName), mStarPieceArray(), _90(), _94(6), _98(), _9C(), _A0() {
}

void StarPieceMother::init(const JMapInfoIter& rIter) {
    s32 isRailNotConnected = false;

    if (!MR::isValidInfo(rIter) || !MR::isConnectedWithRail(rIter)) {
        isRailNotConnected = true;

        MR::initDefaultPos(this, rIter);

        if (MR::useStageSwitchReadAppear(this, rIter)) {
            MR::syncStageSwitchAppear(this);
        }
    } else {
        initRailRider(rIter);
        MR::moveCoordAndTransToRailStartPoint(this);
        MR::needStageSwitchReadAppear(this, rIter);
        MR::syncStageSwitchAppear(this);
    }

    MR::useStageSwitchWriteDead(this, rIter);

    if (MR::useStageSwitchReadA(this, rIter)) {
        MR::listenStageSwitchOnOffA(this, MR::Functor(this, &StarPieceMother::onSwitchA), MR::Functor(this, &StarPieceMother::offSwitchA));
    }

    initModelManagerWithAnm("StarPieceMother", nullptr, false);
    MR::connectToSceneNoSilhouettedMapObj(this);
    MR::onCalcGravity(this);
    MR::initShadowVolumeSphere(this, 60.0f);
    initSound(6, false);
    MR::invalidateClipping(this);
    MR::tryRegisterDemoCast(this, rIter);

    s32 arg = -1;
    MR::getJMapInfoArg0NoInit(rIter, &arg);
    if (arg != -1) {
        _94 = arg;
    }

    mStarPieceArray = new StarPiece*[_94];
    _90 = new f32[_94];

    for (u32 idx = 0; idx < _94; idx++) {
        mStarPieceArray[idx] = new StarPiece("スターピースマザーピース");
        mStarPieceArray[idx]->initAndSetFloatingFromGroup(rIter);
        mStarPieceArray[idx]->makeActorDead();

        if (MR::isValidSwitchA(this)) {
            mStarPieceArray[idx]->mFlags._1 = false;
        }
    }

    if (isRailNotConnected) {
        initNerve(&NrvStarPieceMother::HostTypeNrvWait::sInstance);
        initHitSensor(1);
        MR::addHitSensorMapObj(this, "body", 8, 100.0f, TVec3f(0.0f, 0.0f, 0.0f));
        makeActorAppeared();
    } else {
        initNerve(&NrvStarPieceMother::HostTypeNrvMoveOnRail::sInstance);

        arg = -1;
        MR::getJMapInfoArg1NoInit(rIter, &arg);
        f32 railPos = (arg == -1) ? 0.0f : static_cast< f32 >(arg);
        placementPieceOnRail(railPos);

        arg = -1;
        MR::getJMapInfoArg2NoInit(rIter, &arg);
        if (arg != -1 && arg != 0) {
            _A0 = static_cast< f32 >(arg);
        }

        makeActorDead();
    }

    MR::startBpk(this, "StarPieceMother");
}

void StarPieceMother::kill() {
    MR::stopSound(this, "SE_OJ_STAR_PIECE_MO_ST");
    MR::startSound(this, "SE_OJ_STAR_PIECE_MO_ED");
    LiveActor::kill();

    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }
}

void StarPieceMother::control() {
    mRotation.y += ::hAddRotateY;
}

void StarPieceMother::exeMoveOnRail() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_STAR_PIECE_MO_ST");
    }

    MR::startLevelSound(this, "SE_OJ_LV_STAR_PIECE_MO_MV");
    MR::moveCoordAndFollowTrans(this, _A0);

    f32 railCoord = MR::getRailCoord(this);  // Necessary to match
    if (_90[_98] <= railCoord) {
        mStarPieceArray[_98]->appear();
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");
        _98++;
    }

    if (MR::isRailReachedGoal(this)) {
        kill();
    }
}

void StarPieceMother::placementPieceOnRail(f32 distance) {
    TVec3f railPos;
    MR::calcRailPosAtCoord(&railPos, this, distance);
    mStarPieceArray[0]->mPosition.set(railPos);
    _90[0] = distance;

    f32 step;
    f32 difference = MR::getRailTotalLength(this) - distance;
    if (1 < _94) {
        step = 1.0f / (_94 - 1.0f) * difference;
    }

    for (int idx = 1; idx < _94; idx++) {
        distance += step;
        f32 value = distance >= difference ? difference : distance;
        MR::calcRailPosAtCoord(&railPos, this, value);
        mStarPieceArray[idx]->mPosition.set(railPos);
        _90[idx] = value;
    }
}

void StarPieceMother::onSwitchA() {
    for (u32 idx = 0; idx < _94; idx++) {
        mStarPieceArray[idx]->mFlags._1 = true;
    }
}

void StarPieceMother::offSwitchA() {
    for (u32 idx = 0; idx < _94; idx++) {
        mStarPieceArray[idx]->mFlags._1 = false;
    }
}

void StarPieceMother::attackSensor(HitSensor* pSensor1, HitSensor* pSensor2) {
    if (MR::isSensorPlayer(pSensor2) && (!MR::isValidSwitchA(this) || MR::isOnSwitchA(this))) {
        emitStarPieces();
        kill();
    }
}

void StarPieceMother::emitStarPieces() {
    for (u32 idx = 0; idx < _94; idx++) {
        TVec3f position;
        position.set(*MR::getPlayerCenterPos());
        position.sub(mPosition);
        MR::normalizeOrZero(&position);
        MR::addRandomVector(&position, position, ::hAddRandomVectorValue);
        MR::normalizeOrZero(&position);

        if (MR::isNearZero(position)) {
            position.set(0.0f, 0.0f, 1.0f);
        }

        mStarPieceArray[idx]->makeActorAppeared();
        mStarPieceArray[idx]->goToPlayer(position);
    }
}

void StarPieceMother::exeWait() {
}
