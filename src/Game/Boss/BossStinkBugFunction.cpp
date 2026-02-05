#include "Game/Boss/BossStinkBugFunction.hpp"
#include "Game/Boss/BossStinkBug.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMath.hpp"

void BossStinkBugFunction::addHeightOffset(TVec3f* pOut, const BossStinkBug* pStinkBug, f32 scale) {
    pOut->addInLine(pStinkBug->mGravity.multInLine(-scale));
}

void BossStinkBugFunction::addLocalHeightOffset(TVec3f* pOut, BossStinkBug* pStinkBug, f32 scale) {
    if (!MR::isNearZero(scale)) {
        pStinkBug->updatePose();
        TVec3f upVec;
        pStinkBug->calcUpVec(&upVec);
        JMAVECScaleAdd(upVec, pOut, pOut, scale);
    }
}

void BossStinkBugFunction::calcDiffCurrentRailPosition(TVec3f* pOut, BossStinkBug* pStinkBug, f32 scale1, f32 scale2) {
    TVec3f railPosition;
    railPosition.set(MR::getRailPos(pStinkBug));
    addHeightOffset(&railPosition, pStinkBug, scale1);
    addLocalHeightOffset(&railPosition, pStinkBug, scale2);
    pOut->set(pStinkBug->mPosition - railPosition);
}

void BossStinkBugFunction::moveRail(BossStinkBug* pStinkBug, f32 f1, f32 f2, f32 f3) {
    MR::moveCoord(pStinkBug, f1);
    TVec3f railPos(MR::getRailPos(pStinkBug));
    addHeightOffset(&railPos, pStinkBug, f2);
    MR::turnDirectionDegree(pStinkBug, &pStinkBug->_EC, MR::getRailDirection(pStinkBug), 10.0f);
    addLocalHeightOffset(&railPos, pStinkBug, f3);
    pStinkBug->mPosition.set(railPos);
}

void BossStinkBugFunction::turnRailGround(BossStinkBug* pStinkBug, const TVec3f& rVec, f32 f1, bool a1) {
    if (MR::isRailGoingToEnd(pStinkBug) == a1) {
        MR::reverseRailDirection(pStinkBug);
    }
    MR::rotateVecDegree(&pStinkBug->_EC, rVec, pStinkBug->mGravity, MR::getEaseInOutValue(f1, 0.0f, 1.0f, 1.0f) * 180.0f * -(a1 ? 1.0f : -1.0f));
}

void BossStinkBugFunction::turnRail(BossStinkBug* pStinkBug, const TVec3f& rVec1, f32 f1, f32 f2, f32 f3, f32 f4, f32 f5, bool a1) {
    f32 moveCoordParameter;

    if (f2 <= 0.5f) {
        moveCoordParameter = f3 * MR::getEaseInOutValue(1.0f - MR::normalize(f2, 0.0f, 0.5f), 0.0f, 1.0f, 1.0f);
    } else {
        if (MR::isRailGoingToEnd(pStinkBug) == a1) {
            MR::reverseRailDirection(pStinkBug);
        }
        moveCoordParameter = f3 * MR::getEaseInOutValue(MR::normalize(f2, 0.5f, 1.0f), 0.0f, 1.0f, 1.0f);
    }

    MR::moveCoord(pStinkBug, moveCoordParameter);
    TVec3f railPos(MR::getRailPos(pStinkBug));
    addHeightOffset(&railPos, pStinkBug, f4);

    f32 flt1 = (a1 ? 1.0f : -1.0f);
    f32 temp = 4.0f * f2 * (f2 - 1.0f);
    temp *= temp;
    pStinkBug->mRotation.z = MR::repeat(f1 * (1 - temp) + (90.0f * flt1) * temp, -180.0f, 360.0f);

    MR::rotateVecDegree(&pStinkBug->_EC, rVec1, pStinkBug->mGravity, MR::getEaseInOutValue(f2, 0.0f, 1.0f, 1.0f) * 180.0f * -flt1);
    addLocalHeightOffset(&railPos, pStinkBug, f5);
    pStinkBug->mPosition.set(railPos);
}

void BossStinkBugFunction::regainToRail(BossStinkBug* pStinkBug, const TVec3f& rVec, f32 f1, f32 f2, f32 f3, f32 f4) {
    pStinkBug->mRotation.z *= 0.9f;
    MR::moveCoord(pStinkBug, f2);
    TVec3f railPos(MR::getRailPos(pStinkBug));
    addHeightOffset(&railPos, pStinkBug, f3);
    addLocalHeightOffset(&railPos, pStinkBug, f4);
    TVec3f dif = railPos - pStinkBug->mPosition;
    if (MR::isNearZero(dif)) {
        MR::turnDirectionDegree(pStinkBug, &pStinkBug->_EC, dif, 2.0f);
    } else {
        MR::turnDirectionDegree(pStinkBug, &pStinkBug->_EC, MR::getRailDirection(pStinkBug), 10.0f);
    }
    JMAVECScaleAdd(rVec, railPos, pStinkBug->mPosition, 1.0f - f1);
}

void BossStinkBugFunction::setFallVelocity(BossStinkBug* pStinkBug, const TVec3f& rVec, f32 f1, f32 f2, f32 f3) {
    f32 scaleAddFactor;

    if (f1 < f2) {
        f32 temp = 1.0f / f2 * (f1 - f2);
        temp *= temp;
        temp *= temp;
        scaleAddFactor = 1.0f - temp;
    } else {
        scaleAddFactor = 1.0f - MR::getEaseInOutValue(1.0f / (1.0f - f2) * (f1 - f2), 0.0f, 1.0f, 1.0f);
    }

    TVec3f temp;
    temp.set(pStinkBug->mGravity);

    TVec3f scaleAdd;

    JMAVECScaleAdd(temp, rVec, &scaleAdd, scaleAddFactor * f3);

    pStinkBug->mVelocity.set(scaleAdd - pStinkBug->mPosition);
}

bool BossStinkBugFunction::throwBomb(BossStinkBug* pStinkBug) {
    pStinkBug->throwBomb(20.0f, 10.0f);
}

void BossStinkBugFunction::invalidateAttack(BossStinkBug* pStinkBug) {
    MR::invalidateHitSensor(pStinkBug, "attack_left");
    MR::invalidateHitSensor(pStinkBug, "attack_right");
}

bool BossStinkBugFunction::isHipDropableSensor(const BossStinkBug* pStinkBug, const HitSensor* pSensor) {
    return pStinkBug->isSensorBody(pSensor);
}

bool BossStinkBugFunction::isExistPlayerBack(const BossStinkBug* pStinkBug, f32 f1) {
    return MR::calcDifferenceRailCoord(pStinkBug, MR::calcNearestRailCoord(pStinkBug, *MR::getPlayerPos())) < -f1;
}

void BossStinkBugFunction::validateAttack(BossStinkBug* pStinkBug) {
    MR::validateHitSensor(pStinkBug, "attack_left");
    MR::validateHitSensor(pStinkBug, "attack_right");
}
