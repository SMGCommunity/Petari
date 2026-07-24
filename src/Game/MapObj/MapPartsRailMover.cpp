#include "Game/MapObj/MapPartsRailMover.hpp"
#include "Game/LiveActor/Nerve.hpp"
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
