#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioState.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "revolution/types.h"

void Mario::checkOnimasu(const HitSensor* pSensor) {
    if (strstr(pSensor->mHost->mName, "オニマス") == nullptr) {
        return;
    }

    if (_5FC == nullptr) {
        _5FC = pSensor;
        _60C = 1;
    } else if (_5FC != pSensor && (pSensor->mPosition - mPosition).length() < (_5FC->mPosition - mPosition).length()) {
        _5FC = pSensor;
        _60C = 1;
    }
}

bool Mario::isDossun(const Triangle* pTriangle) const {
    if (!pTriangle->isValid()) {
        return false;
    }

    if (strstr(pTriangle->mSensor->mHost->getName(), "ドッスン") != nullptr) {
        return true;
    }

    return false;
}

bool Mario::isStageCameraRotate2D() const {
    return MR::isEqualStageName("HellProminenceGalaxy") && MR::getCurrentScenarioNo() == 3;
}

bool Mario::isNoWalkFallOnDossun() const {
    if (!MR::isEqualStageName("CannonFleetGalaxy")) {
        return false;
    }

    if (isDossun(mGroundPolygon) && isDossun(_45C)) {
        return true;
    }

    return false;
}

bool Mario::isNotReflectGlassGround() const {
    if (!MR::isEqualStageName("AstroGalaxy")) {
        return false;
    }

    TVec3f vec(1627.0f, 783.0f, -2152.0f);
    TVec3f v10 = mPosition - vec;

    f32 v4 = MR::diffAngleAbs(getCamDirZ(), v10);
    f32 v5 = v4;
    if (v4 >= 1.5707964f) {
        v5 = (PI - v4);
    }

    f32 v7 = (400.0f + (300.0f * (MR::sin(v5) * MR::sin(v5))));
    MR::vecKillElement(v10, getAirGravityVec(), &v10);
    f32 length = v10.length();

    if (length >= v7) {
        if (length <= 700.0f) {
            return true;
        }
    }

    return false;
}

bool Mario::isUseAnotherMovingPolygon() const {
    if (MR::isEqualStageName("BattleShipGalaxy")) {
        return true;
    }

    if (MR::isEqualStageName("SandClockGalaxy")) {
        return true;
    }

    return MR::isEqualStageName("TriLegLv1Galaxy");
}

bool Mario::isUseFooSpecialGravity(const TVec3f& a1, TVec3f* a2) const {
    if (!isStatusActive(MarioStatus_Foo)) {
        return false;
    }

    if (MR::isEqualStageName("HeavensDoorGalaxy") && MR::getCurrentScenarioNo() == 2) {
        TVec3f vec(14760.0f, -10676.2f, 6770.0f);
        TVec3f res = vec - a1;
        a2->set(res);
        MR::normalizeOrZero(a2);
        return true;
    }

    return false;
}

// Mario::updateOnimasu

bool Mario::isHeadPushEnableArea() const {
    if (!mMovementStates._17) {
        return false;
    }

    if (MR::isEqualStageName("SandClockGalaxy")) {
        if (MR::isInRange(mPosition.x, -7020.0f, -6920.0f)) {
            return true;
        }
    }

    return false;
}

bool Mario::isOnimasuBinderPressSkip() const {
    if (isStatusActive(MarioStatus_SideStep)) {
        if (mFrontWallTriangle->mSensor != nullptr) {
            if (strstr(mFrontWallTriangle->mSensor->mHost->mName, "オニマス")) {
                return true;
            }
        }
    }

    return false;
}
