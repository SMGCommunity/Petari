#include "Game/MapObj/LavaStrangeRock.hpp"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

namespace {
    static const f32 sHitRange = 870.0f;
    static const f32 sHitRangeL = 1300.0f;
    static const f32 sHitRangeTable = 730.0f;
    static const f32 sRockSLength = 150.0f;
    static const f32 sRockLLength = 200.0f;
    static const f32 sRockTableLength = 285.0f;
    static const f32 sPushVelocity = 50.0f;
};  // namespace

namespace NrvLavaStrangeRock {
    NEW_NERVE(LavaStrangeRockNrvWait, LavaStrangeRock, Wait);
};  // namespace NrvLavaStrangeRock

LavaStrangeRock::~LavaStrangeRock() {
}

LavaStrangeRock::LavaStrangeRock(const char* pName) : LiveActor(pName), mRockType(Type_3), _90(), mLodCtrlPlanet() {
}

void LavaStrangeRock::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);

    s32 arg0 = -1;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);

    if (arg0 + 1 <= 1U) {
        _90 = 0;
    } else if (arg0 == 1) {
        _90 = 1;
    } else if (arg0 == 2) {
        _90 = 2;
    }

    const char* objName = nullptr;
    MR::getObjectName(&objName, rIter);

    if (MR::isEqualString(objName, "LavaStrangeRockTable")) {
        mRockType = Type_2;
        setName("溶岩奇岩台形");
    } else if (MR::isEqualString(objName, "LavaStrangeRockL")) {
        mRockType = Type_1;
        setName("溶岩奇岩大");
    } else {
        mRockType = Type_0;
    }

    initModelAndClipping(rIter);

    if (mRockType == 1) {
        mLodCtrlPlanet = MR::createLodCtrlPlanet(this, rIter, -1, -1);
    }

    MR::connectToSceneMapObj(this);
    initEffect();
    initSound(4, false);
    initNerve(&NrvLavaStrangeRock::LavaStrangeRockNrvWait::sInstance);
    makeActorAppeared();
}

void LavaStrangeRock::initModelAndClipping(const JMapInfoIter& rIter) {
    MR::setGroupClipping(this, rIter, 16);

    s32 arg3 = 0;
    MR::getJMapInfoArg3NoInit(rIter, &arg3);

    switch (mRockType) {
    case Type_0:
        initModelManagerWithAnm("LavaStrangeRock", nullptr, false);
        MR::setClippingTypeSphere(this, ::sHitRange * mScale.y);
        break;
    case Type_1:
        initModelManagerWithAnm("LavaStrangeRockL", nullptr, false);
        MR::setClippingTypeSphere(this, ::sHitRangeL * mScale.y);
        break;
    case Type_2:
        initModelManagerWithAnm("LavaStrangeRockTable", nullptr, false);
        MR::setClippingTypeSphere(this, ::sHitRangeTable * mScale.y);
        break;
    }
}

void LavaStrangeRock::initEffect() {
    switch (mRockType) {
    case Type_0:
        initEffectKeeper(1, "LavaStrangeRock", false);
        break;
    case Type_1:
        initEffectKeeper(1, "LavaStrangeRockL", false);
        break;
    case Type_2:
        initEffectKeeper(1, "LavaStrangeRockTable", false);
        break;
    }
}

void LavaStrangeRock::control() {
    if (mRockType == Type_1) {
        mLodCtrlPlanet->update();
    }
}

void LavaStrangeRock::exeWait() {
    TVec3f centerPos = *MR::getPlayerCenterPos();
    centerPos -= *MR::getPlayerVelocity();

    TVec3f upVec;
    MR::calcUpVec(&upVec, this);

    TVec3f hitOffset = upVec;
    f32 radius;

    switch (mRockType) {
    case Type_0:
        hitOffset *= ::sHitRange;
        radius = ::sRockSLength * mScale.y;
        break;
    case Type_1:
        hitOffset *= ::sHitRangeL;
        radius = ::sRockLLength * mScale.y;
        break;
    case Type_2:
        hitOffset *= ::sHitRangeTable;
        radius = ::sRockTableLength * mScale.y;
        break;
    }

    if (_90 == 1) {
        if (!MR::isPlayerInRush()) {
            return;
        }

        if (MR::checkHitSegmentSphere(*MR::getPlayerCenterPos(), mPosition, mPosition + hitOffset, radius, nullptr) != true) {
            return;
        }

        MR::emitEffect(this, "Break");

        switch (mRockType) {  // the lack of breaks here is intentional
        case Type_0:
            MR::startSound(this, "SE_OJ_STRANGEROCK_BREAK_S");
        case Type_1:
            MR::startSound(this, "SE_OJ_STRANGEROCK_BREAK_L");
        case Type_2:
            MR::startSound(this, "SE_OJ_STRANGEROCK_BREAK_M");
        }

        kill();
    } else if (_90 == 2) {
        TVec3f hitCheckDir;

        if (MR::checkHitSegmentSphere(*MR::getPlayerCenterPos(), mPosition, mPosition + hitOffset, radius, &hitCheckDir)) {
            MR::pushPlayer(hitCheckDir * ::sPushVelocity);
        }
    }
}
