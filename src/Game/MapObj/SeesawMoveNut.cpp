#include "Game/MapObj/SeesawMoveNut.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util.hpp"

namespace NrvSeesawMoveNut {
    NEW_NERVE(SeesawMoveNutNrvHalfway, SeesawMoveNut, Halfway);
    NEW_NERVE(SeesawMoveNutNrvEdge, SeesawMoveNut, Edge);
}  // namespace NrvSeesawMoveNut

void SeesawMoveNut_FORCE_MATCH_SDATA2() {
    (void)0.0f;
    (void)0.5f;
}

SeesawMoveNut::SeesawMoveNut(const char* pName) : MapObjActor(pName), mSpeed(100.0f), mPrevAngularSpeed() {
}

void SeesawMoveNut::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo initInfo;

    initInfo.setupHioNode("地形オブジェ");
    initInfo.setupDefaultPos();
    initInfo.setupConnectToScene();
    initInfo.setupSeesaw1AxisRotator(nullptr, 0.0f);
    initInfo.setupEffect(nullptr);
    initInfo.setupSound(6);

    initialize(rIter, initInfo);
    MR::setBodySensorType(this, ATYPE_MAP_OBJ_PRESS);
    MR::getJMapInfoArg0NoInit(rIter, &mSpeed);

    MR::calcNearestRailPos(&mPosition, this, mPosition);

    if (isAtEdge()) {
        initNerve(GET_NERVE(SeesawMoveNut, SeesawMoveNutNrvEdge));
        MapObjActorUtil::endRotator(this);
    } else {
        initNerve(GET_NERVE(SeesawMoveNut, SeesawMoveNutNrvHalfway));
        MapObjActorUtil::startRotator(this);
    }
}

void SeesawMoveNut::exeHalfway() {
    f32 angularSpeed = MapObjActorUtil::getSeesaw1AxisAngularSpeed(this);

    if (angularSpeed > 0.0f) {
        MR::setRailDirectionToEnd(this);
    } else if (angularSpeed < 0.0f) {
        MR::setRailDirectionToStart(this);
    }

    MR::moveCoordAndFollowTrans(this, MR::abs(angularSpeed * mSpeed * 0.01f));

    autoBackHome();

    if (mPrevAngularSpeed == 0.0f && angularSpeed != 0.0f) {
        MR::startSound(this, "SE_OJ_SEESAW_NUT_START");
    }

    mPrevAngularSpeed = angularSpeed;

    if (angularSpeed == 0.0f) {
        return;
    }

    if (MR::abs(angularSpeed) >= 0.001f) {
        MR::startLevelSound(this, "SE_OJ_LV_SEESAW_NUT_ROT_L");
    }

    if (MR::abs(angularSpeed) >= 0.1f) {
        MR::startLevelSound(this, "SE_OJ_LV_SEESAW_NUT_ROT_M");
    }

    if (MR::abs(angularSpeed) >= 0.2f) {
        MR::startLevelSound(this, "SE_OJ_LV_SEESAW_NUT_ROT_H");
    }

    if (isAtEdge()) {
        MapObjActorUtil::endRotator(this);
        MR::reverseRailDirection(this);
        MR::tryRumblePadStrong(this, WPAD_CHAN0);
        MR::shakeCameraNormal();
        MR::startSound(this, "SE_OJ_SEESAW_NUT_STOP");
        setNerve(GET_NERVE(SeesawMoveNut, SeesawMoveNutNrvEdge));
    }
}

void SeesawMoveNut::exeEdge() {
    if (isEnableStart()) {
        MapObjActorUtil::startRotator(this);
        _CC = 0;
        setNerve(GET_NERVE(SeesawMoveNut, SeesawMoveNutNrvHalfway));
    } else {
        autoBackHome();
    }
}

void SeesawMoveNut::autoBackHome() {
    if (_CC >= 180) {
        if (isNerve(GET_NERVE(SeesawMoveNut, SeesawMoveNutNrvEdge)) && MR::getRailCoord(this) == MR::getRailTotalLength(this)) {
            MapObjActorUtil::startRotator(this);
            setNerve(GET_NERVE(SeesawMoveNut, SeesawMoveNutNrvHalfway));
            return;
        }

        if (MR::abs(MapObjActorUtil::getSeesaw1AxisAngularSpeed(this)) < 0.5f) {
            MapObjActorUtil::forceRotateSeesaw1Axis(this, -0.02f);
        }
    }

    if (MR::isOnPlayer(this)) {
        _CC = 0;
    } else {
        _CC++;
    }
}

bool SeesawMoveNut::isEnableStart() const {
    if (!MR::isOnPlayer(this)) {
        return false;
    }

    TVec3f frontVec;
    MR::calcFrontVec(&frontVec, this);

    TVec3f dirToPlayerNoFront;
    dirToPlayerNoFront.sub(*MR::getPlayerPos(), mPosition);
    dirToPlayerNoFront.orthogonalize(frontVec);

    MR::normalize(&dirToPlayerNoFront);

    return dirToPlayerNoFront.cross(mGravity).dot(MR::getRailDirection(this)) > 0.0f;
}

bool SeesawMoveNut::isAtEdge() const {
    return MR::getRailCoord(this) == 0.0f || MR::getRailCoord(this) == MR::getRailTotalLength(this);
}
