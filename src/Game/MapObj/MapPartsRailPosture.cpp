#include "Game/MapObj/MapPartsRailPosture.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

namespace NrvMapPartsRailPosture {
    NEW_NERVE(HostTypeDoNothing, MapPartsRailPosture, DoNothing);
    NEW_NERVE(HostTypeMove, MapPartsRailPosture, Move);
};  // namespace NrvMapPartsRailPosture

MapPartsRailPosture::~MapPartsRailPosture() {
}

MapPartsRailPosture::MapPartsRailPosture(LiveActor* pHost) : MapPartsFunction(pHost, "姿勢制御") {
    mMovePosture = 0;
    _18.identity();
}

void MapPartsRailPosture::init(const JMapInfoIter& rIter) {
    MR::getMapPartsArgMovePosture(&mMovePosture, mHost);

    if (mMovePosture == 0) {
        initNerve(GET_NERVE(MapPartsRailPosture, HostTypeDoNothing));
    } else {
        initNerve(GET_NERVE(MapPartsRailPosture, HostTypeMove));
    }
}

void MapPartsRailPosture::start() {
}

void MapPartsRailPosture::end() {
    _18.identity();
    setNerve(GET_NERVE(MapPartsRailPosture, HostTypeDoNothing));
}

bool MapPartsRailPosture::isWorking() const {
    return isNerve(GET_NERVE(MapPartsRailPosture, HostTypeMove));
}

void MapPartsRailPosture::exeMove() {
    s32 arg0 = -1;
    MR::getRailArg0NoInit(mHost, &arg0);

    TVec3f shadowVector;

    if (mMovePosture == 3) {
        MR::calcDropShadowVector(mHost, &shadowVector, nullptr, 0);
    } else {
        MR::calcGravityVector(mHost, &shadowVector, nullptr, 0);
    }

    TVec3f v11 = MR::getRailDirection(mHost);

    if (!MR::isSameDirection(shadowVector, v11)) {
        if (!MR::isRailGoingToEnd(mHost)) {
            v11.negate();
        }

        TVec3f v10(1.0f, 0.0f, 0.0f);
        TVec3f v9(0.0f, 1.0f, 0.0f);
        TVec3f v8(0.0f, 0.0f, 1.0f);

        if (mMovePosture == 1 || isPostureTypeRailDirRailUseShadowGravity) {
            v9.set(-shadowVector);
            v10.cross(v9, v11);
            MR::normalize(&v10);
            v8.cross(v10, v9);
            MR::normalize(&v8);
        } else if (mMovePosture == 2) {
            v9.set(-shadowVector);
            v8.set(v11);
            v10.cross(v9, v8);
            MR::normalize(&v10);
            v9.cross(v8, v10);
            MR::normalize(&v9);
        }

        TVec3f v7;
        _18.getXDir(v7);
        MR::normalize(&v7);

        if (MR::isSameDirection(v7, v8)) {
            MR::makeMtxFrontNoSupport(&_18, v8);
        } else {
            MR::makeMtxFrontUp(&_18, v8, v9);
        }
    }
}

void MapPartsRailPosture::exeDoNothing() {
}

bool MapPartsRailPosture::isPostureTypeRailDirRailUseShadowGravity() const {
    return mMovePosture == 3;
}
