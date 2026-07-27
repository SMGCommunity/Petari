#include "Game/MapObj/MapPartsRailMover.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapParts.hpp"
#include "Game/MapObj/MapPartsFunction.hpp"
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
