#include "Game/MapObj/MapPartsRailMover.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapParts.hpp"
#include "Game/MapObj/MapPartsFunction.hpp"
#include "Game/MapObj/MapPartsRailPointPassChecker.hpp"
#include "Game/Util.hpp"

namespace NrvMapPartsRailMover {
    NEW_NERVE(HostTypeWait, MapPartsRailMover, Wait);
    NEW_NERVE(HostTypeMove, MapPartsRailMover, Move);
    NEW_NERVE(HostTypeMoveStart, MapPartsRailMover, MoveStart);
    NEW_NERVE(HostTypeStopAtPointBeforeRotate, MapPartsRailMover, StopAtPoint);
    NEW_NERVE(HostTypeStopAtPointAfterRotate, MapPartsRailMover, StopAtPoint);
    NEW_NERVE(HostTypeStopAtEndBeforeRotate, MapPartsRailMover, StopAtEnd);
    NEW_NERVE(HostTypeStopAtEndAfterRotate, MapPartsRailMover, StopAtEnd);
    NEW_NERVE(HostTypeStopAtEndWithPlayerOn, MapPartsRailMover, StopAtEndWithPlayerOn);
    NEW_NERVE(HostTypeWaitForRestartByPlayerOn, MapPartsRailMover, WaitForRestartByPlayerOn);
    NEW_NERVE(HostTypeRotateAtPoint, MapPartsRailMover, RotateAtPoint);
    NEW_NERVE(HostTypeRotateAtEndPoint, MapPartsRailMover, RotateAtEndPoint);
    NEW_NERVE(HostTypeVanish, MapPartsRailMover, Vanish);
};  // namespace NrvMapPartsRailMover

MapPartsRailMover::MapPartsRailMover(LiveActor* pActor)
    : MapPartsFunction(pActor, "レイル移動"), mRailPointPassChecker(), mMoveConditionType(), mMoveStopType(1), mSignMotionType(), _28(gZeroVec) {
    _34 = 0.0f;
    mStopTime = 0;
    _3C = 0.0f;
    _40 = 0;
    _44 = 0.0f;
    _48 = 0.0f;
}

void MapPartsRailMover::init(const JMapInfoIter& rIter) {
    MR::getMapPartsArgMoveConditionType(&mMoveConditionType, rIter);
    MR::getMapPartsArgMoveStopType(&mMoveStopType, mHost);
    MR::getMapPartsArgSignMotionType(&mSignMotionType, rIter);

    s32 initPostType = 0;
    MR::getMapPartsArgRailInitPosType(&initPostType, mHost);

    if (MR::isMapPartsRailInitPosTypeRailPos(initPostType)) {
        MR::moveCoordAndTransToNearestRailPos(mHost);
    } else if (MR::isMapPartsRailInitPosTypeRailPoint(initPostType)) {
        MR::moveCoordAndTransToNearestRailPoint(mHost);
    } else if (MR::isMapPartsRailInitPosTypePoint0(initPostType)) {
        MR::moveCoordAndTransToRailStartPoint(mHost);
    }

    _28.set< f32 >(mHost->mPosition);
    _34 = MR::getRailCoord(mHost);
    mRailPointPassChecker = new MapPartsRailPointPassChecker(mHost);
    mRailPointPassChecker->init(rIter);
    initNerve(GET_NERVE(MapPartsRailMover, HostTypeWait));
}

void MapPartsRailMover::movement() {
    bool isMove = false;

    if (isNerve(GET_NERVE(MapPartsRailMover, HostTypeMove)) || isNerve(GET_NERVE(MapPartsRailMover, HostTypeMoveStart))) {
        isMove = true;
    }

    if (isMove) {
        tryPassPoint();
    } else {
        tryRestartAtEnd();
    }

    mRailPointPassChecker->movement();
    MapPartsFunction::movement();
}

void MapPartsRailMover::start() {
    moveToInitPos();
    mRailPointPassChecker->start();

    if (MR::hasMapPartsMoveStartSignMotion(mSignMotionType)) {
        setNerve(GET_NERVE(MapPartsRailMover, HostTypeMoveStart));
    } else {
        setNerve(GET_NERVE(MapPartsRailMover, HostTypeMove));
    }
}

void MapPartsRailMover::end() {
    mRailPointPassChecker->end();
    setNerve(GET_NERVE(MapPartsRailMover, HostTypeWait));
}

bool MapPartsRailMover::isWorking() const {
    bool isWork = false;

    if (isNerve(GET_NERVE(MapPartsRailMover, HostTypeMove)) || isNerve(GET_NERVE(MapPartsRailMover, HostTypeMoveStart))) {
        isWork = true;
    }

    return isWork;
}

bool MapPartsRailMover::receiveMsg(u32 msg) {
    if (msg == ACTMES_MAPPARTS_ON_PLAYER && isNerve(GET_NERVE(MapPartsRailMover, HostTypeWaitForRestartByPlayerOn))) {
        restartAtEnd();
        return true;
    }

    return false;
}

void MapPartsRailMover::moveToInitPos() {
    if (!MR::isNearZero(_34 - MR::getRailCoord(mHost))) {
        MR::setRailCoord(mHost, _34);
        _28.set< f32 >(MR::getRailPos(mHost));
        if (!MR::isRailGoingToEnd(mHost)) {
            MR::reverseRailDirection(mHost);
        }
    }
}

void MapPartsRailMover::startWithSignalMotion() {
    setNerve(GET_NERVE(MapPartsRailMover, HostTypeMoveStart));
}

void MapPartsRailMover::cancelSignalMotion() {
    MR::setRailCoord(mHost, _48);
    _28.set< f32 >(MR::getRailPos(mHost));
    setNerve(GET_NERVE(MapPartsRailMover, HostTypeWait));
}

bool MapPartsRailMover::tryResetPositionRepeat() {
    if (mMoveStopType != 2) {
        return false;
    }

    if (!isNerve(GET_NERVE(MapPartsRailMover, HostTypeMove))) {
        return false;
    }

    if (getStep() != 1) {
        return false;
    }

    MR::resetPosition(mHost);
    return true;
}

void MapPartsRailMover::resetToInitPos() {
    moveToInitPos();
    MR::resetPosition(mHost, _28);
    setNerve(GET_NERVE(MapPartsRailMover, HostTypeWait));
}

void MapPartsRailMover::passPoint() {
    mStopTime = 0;
    MR::getMapPartsArgStopTime(&mStopTime, mHost);

    if (mStopTime > 0) {
        setNerve(GET_NERVE(MapPartsRailMover, HostTypeStopAtPointBeforeRotate));

    } else {
        if (!sendMsgToHost(ACTMES_MAPPARTS_START_ROTATE_BETWEEN_POINTS)) {
            if (sendMsgToHost(ACTMES_MAPPARTS_START_ROTATE_AT_POINT)) {
                setNerve(GET_NERVE(MapPartsRailMover, HostTypeRotateAtPoint));
            } else {
                setNerve(GET_NERVE(MapPartsRailMover, HostTypeMove));
            }
        }
    }
}

void MapPartsRailMover::reachedEnd() {
    if (mMoveStopType == 1) {
        MR::reverseRailDirection(mHost);
    }

    if (MR::isMoveStartTypePlayerOnStopEnd(mMoveConditionType)) {
        reachedEndPlayerOn();
    } else {
        setStateStopAtEndBeforeRotate();
    }
}

void MapPartsRailMover::reachedEndPlayerOn() {
    if (MR::isOnPlayer(MR::getBodySensor(mHost))) {
        setNerve(GET_NERVE(MapPartsRailMover, HostTypeStopAtEndWithPlayerOn));
    } else {
        setNerve(GET_NERVE(MapPartsRailMover, HostTypeWaitForRestartByPlayerOn));
    }
}

bool MapPartsRailMover::isReachedEnd() const {
    bool ret = false;

    if (isNerve(GET_NERVE(MapPartsRailMover, HostTypeMove)) || isNerve(GET_NERVE(MapPartsRailMover, HostTypeMoveStart))) {
        if (mRailPointPassChecker->isReachedEnd()) {
            ret = true;
        }
    }

    return ret;
}

bool MapPartsRailMover::isDone() const {
    if (mMoveStopType == 0 || mMoveStopType == 3) {
        return mRailPointPassChecker->isReachedEnd();
    }

    return false;
}

void MapPartsRailMover::setStateStopAtEndBeforeRotate() {
    _3C = 0.0f;
    _44 = 0.0f;
    MR::setRailCoordSpeed(mHost, 0.0f);
    mStopTime = 0;
    MR::getMapPartsArgStopTime(&mStopTime, mHost);

    if (mStopTime <= 0) {
        if (sendMsgToHost(ACTMES_MAPPARTS_START_ROTATE_AT_POINT)) {
            setNerve(GET_NERVE(MapPartsRailMover, HostTypeRotateAtEndPoint));
        } else {
            restartAtEnd();
        }
    } else {
        setNerve(GET_NERVE(MapPartsRailMover, HostTypeStopAtEndBeforeRotate));
    }
}

// MapPartsRailMover::calcTimeToNextRailPoint

void MapPartsRailMover::endRotateAtPoint() {
    if (isNerve(GET_NERVE(MapPartsRailMover, HostTypeRotateAtPoint))) {
        setNerve(GET_NERVE(MapPartsRailMover, HostTypeStopAtPointAfterRotate));
    } else if (isNerve(GET_NERVE(MapPartsRailMover, HostTypeRotateAtEndPoint))) {
        setNerve(GET_NERVE(MapPartsRailMover, HostTypeStopAtEndAfterRotate));
    }
}

void MapPartsRailMover::calcMoveSpeed(f32* pMoveSpeed) const {
    bool isNotNear = MR::isNearZero(_44) == false;

    if (isNotNear) {
        *pMoveSpeed = MR::getRailCoordSpeed(mHost);
    } else {
        s32 calcType = 0;
        MR::getMapPartsArgSpeedCalcType(&calcType, mHost);

        if (MR::isMapPartsRailSpeedCalcTypeTime(calcType)) {
            calcMoveSpeedTime(pMoveSpeed);
        } else {
            calcMoveSpeedDirect(pMoveSpeed);
        }
    }
}

void MapPartsRailMover::calcMoveSpeedDirect(f32* pSpeed) const {
    f32 speed = -1.0f;
    MR::getMapPartsArgMoveSpeed(&speed, mHost);

    if (speed < 0.0f) {
        return;
    }

    *pSpeed = speed;
}

void MapPartsRailMover::calcMoveSpeedTime(f32* pTime) const {
    s32 moveTime = -1;
    MR::getMapPartsArgMoveTimeToNextPoint(&moveTime, mHost);

    if (moveTime >= 0) {
        s32 curPnt = MR::getCurrentRailPointNo(mHost);

        if (!MR::isRailGoingToEnd(mHost)) {
            curPnt--;
        }

        *pTime = MR::getRailPartLength(mHost, curPnt) / moveTime;
    }
}

void MapPartsRailMover::restartAtEnd() {
    if (mMoveStopType == 0) {
        setNerve(GET_NERVE(MapPartsRailMover, HostTypeWait));
    } else if (mMoveStopType == 1) {
        if (MR::hasMapPartsMoveStartSignMotion(mSignMotionType)) {
            setNerve(GET_NERVE(MapPartsRailMover, HostTypeMoveStart));
        } else {
            setNerve(GET_NERVE(MapPartsRailMover, HostTypeMove));
        }
    } else if (mMoveStopType == 2) {
        MR::moveCoordToStartPos(mHost);
        _28.set< f32 >(MR::getRailPos(mHost));
        setNerve(GET_NERVE(MapPartsRailMover, HostTypeMove));
    } else if (mMoveStopType == 3) {
        setNerve(GET_NERVE(MapPartsRailMover, HostTypeVanish));
    }
}

void MapPartsRailMover::exeMove() {
    if (isFirstStep()) {
        updateAccel();
        calcMoveSpeed(&_3C);
        sendMsgToHost(ACTMES_MAPPARTS_START_ROTATE_BETWEEN_POINTS);
    }

    bool isNotNear = !MR::isNearZero(_44);

    if (isNotNear && getStep() < _40) {
        _3C += _44;
    }

    MR::moveCoord(mHost, _3C);
    _28.set< f32 >(MR::getRailPos(mHost));
}

void MapPartsRailMover::exeMoveStart() {
    if (isFirstStep()) {
        _48 = MR::getRailCoord(mHost);

        if (MR::isExistCollisionParts(mHost)) {
            MR::offUpdateCollisionParts(mHost);
        }
    }

    f32 v3;

    if ((getStep() / 3) % 2 == 0) {
        v3 = 1.0f;
    } else {
        v3 = -1.0f;
    }

    if (MR::isRailGoingToEnd(mHost)) {
        v3 *= -1.0f;
    }

    MR::setRailCoord(mHost, _48 + (7.0f * (getStep() % 3)));
    _28.set< f32 >(MR::getRailPos(mHost));
    if (isStep(MapParts::getMoveStartSignalTime())) {
        MR::setRailCoord(mHost, _48);

        if (MR::isExistCollisionParts(mHost)) {
            MR::onUpdateCollisionParts(mHost);
        }

        _28.set< f32 >(MR::getRailPos(mHost));
        setNerve(GET_NERVE(MapPartsRailMover, HostTypeMove));
    }
}

void MapPartsRailMover::exeStopAtPoint() {
    if (isFirstStep()) {
        _3C = 0.0f;
        _44 = 0.0f;
        MR::setRailCoordSpeed(mHost, 0.0f);
    }

    if (isStep(mStopTime)) {
        if (isNerve(GET_NERVE(MapPartsRailMover, HostTypeStopAtPointBeforeRotate))) {
            if (sendMsgToHost(ACTMES_MAPPARTS_START_ROTATE_AT_POINT)) {
                setNerve(GET_NERVE(MapPartsRailMover, HostTypeRotateAtPoint));
            } else {
                setNerve(GET_NERVE(MapPartsRailMover, HostTypeStopAtPointAfterRotate));
            }
        } else {
            setNerve(GET_NERVE(MapPartsRailMover, HostTypeMove));
        }
    }
}

void MapPartsRailMover::exeStopAtEnd() {
    if (!tryRestartAtEnd() && isStep(mStopTime)) {
        if (isNerve(GET_NERVE(MapPartsRailMover, HostTypeStopAtEndBeforeRotate))) {
            if (sendMsgToHost(ACTMES_MAPPARTS_START_ROTATE_AT_POINT)) {
                setNerve(GET_NERVE(MapPartsRailMover, HostTypeRotateAtEndPoint));
            } else {
                setNerve(GET_NERVE(MapPartsRailMover, HostTypeStopAtEndAfterRotate));
            }

        } else {
            restartAtEnd();
        }
    }
}

MapPartsRailMover::~MapPartsRailMover() {
    return;
}

void MapPartsRailMover::exeVanish() {
    if (MapPartsFunction::isFirstStep()) {
        MapPartsFunction::sendMsgToHost(ACTMES_MAPPARTS_DISAPPEAR_WITH_BLINK);
    }
}

void MapPartsRailMover::exeStopAtEndWithPlayerOn() {
    if (!MR::isOnPlayer(MR::getBodySensor(mHost))) {
        setNerve(GET_NERVE(MapPartsRailMover, HostTypeWaitForRestartByPlayerOn));
    }
}

void MapPartsRailMover::exeRotateAtEndPoint() {
    return;
}

void MapPartsRailMover::exeRotateAtPoint() {
    return;
}

void MapPartsRailMover::exeWaitForRestartByPlayerOn() {
    return;
}

void MapPartsRailMover::exeWait() {
    return;
}
