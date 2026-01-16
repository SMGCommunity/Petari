#include "Game/MapObj/LavaStrangeRock.hpp"
#include "Game/LiveActor/LodCtrl.hpp"

namespace NrvLavaStrangeRock {
    NEW_NERVE(LavaStrangeRockNrvWait, LavaStrangeRock, Wait);
}

LavaStrangeRock::LavaStrangeRock(const char* pName) : LiveActor(pName), rockType(3), _90(0), mLodCtrlPlanet(nullptr) {}

void LavaStrangeRock::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    s32 v1 = -1;
    MR::getJMapInfoArg0NoInit(rIter, &v1);

    if (((unsigned)v1 + 1) <= 1) {
        _90 = 0;
    } else if (v1 == 1) {
        _90 = 1;
    } else if (v1 == 2) {
        _90 = 2;
    }
    const char* pDest = nullptr;
    MR::getObjectName(&pDest, rIter);
    if ((MR::isEqualString(pDest, "LavaStrangeRockTable"))) {
        rockType = 2;
        setName("溶岩奇岩台形");  // lavastrangerock in japanese
    } else if ((MR::isEqualString(pDest, "LavaStrangeRockL"))) {
        rockType = 1;
        setName("溶岩奇岩大");
    } else {
        rockType = 0;
    }
    initModelAndClipping(rIter);
    if (rockType == 1) {
        mLodCtrlPlanet = MR::createLodCtrlPlanet(this, rIter, -1, -1);
    }
    MR::connectToSceneMapObj(this);
    initEffect();
    initSound(4, false);
    initNerve(&NrvLavaStrangeRock::LavaStrangeRockNrvWait::sInstance);
    makeActorAppeared();
}

void LavaStrangeRock::initModelAndClipping(const JMapInfoIter& iter) {
    MR::setGroupClipping(this, iter, 16);
    s32 v2 = 0;
    MR::getJMapInfoArg3NoInit(iter, &v2);
    switch (rockType) {
    case 0:
        initModelManagerWithAnm("LavaStrangeRock", nullptr, false);
        MR::setClippingTypeSphere(this, (mScale.y * 870.0f));
        break;
    case 1:
        initModelManagerWithAnm("LavaStrangeRockL", nullptr, false);
        MR::setClippingTypeSphere(this, (mScale.y * 1300.0f));
        break;
    case 2:
        initModelManagerWithAnm("LavaStrangeRockTable", nullptr, false);
        MR::setClippingTypeSphere(this, (mScale.y * 730.0f));
        break;
    default:
        break;
    }
}

void LavaStrangeRock::initEffect() {
    switch (rockType) {
    case 0:
        initEffectKeeper(1, "LavaStrangeRock", false);
        break;
    case 1:
        initEffectKeeper(1, "LavaStrangeRockL", false);
        break;
    case 2:
        initEffectKeeper(1, "LavaStrangeRockTable", false);
        break;
    default:
        break;
    }
}

void LavaStrangeRock::control() {
    if (rockType == 1) {
        mLodCtrlPlanet->update();
    }
}

void LavaStrangeRock::exeWait() {
    TVec3f pCenterPos = *MR::getPlayerCenterPos();
    JMathInlineVEC::PSVECSubtract(&pCenterPos, MR::getPlayerVelocity(), &pCenterPos);
    TVec3f upVec;
    MR::calcUpVec(&upVec, this);
    TVec3f upVec2(upVec);
    f32 v1;
    switch (rockType) {
    case 0:
        upVec2 *= 870.0f;
        v1 = 150.0f * mScale.y;
        break;
    case 1:
        upVec2 *= 1300.0f;
        v1 = 200.0f * mScale.y;
        break;
    case 2:
        upVec2 *= 730.0f;
        v1 = 285.0f * mScale.y;
        break;
    default:
        break;
    }
    TVec3f checkHitSegSphereRes;
    if (_90 == 1) {
        if (!(MR::isPlayerInRush()))
            return;
        TVec3f upPlusPosition = mPosition.operator+(upVec2);
        TVec3f* pCenterPos2 = MR::getPlayerCenterPos();
        if ((MR::checkHitSegmentSphere(*pCenterPos2, mPosition, upPlusPosition, v1, 0)) != 1)
            return;
        MR::emitEffect(this, "Break");
        switch ((unsigned)rockType) {  // the lack of breaks here is intentional
        case 0:
            MR::startSound(this, "SE_OJ_STRANGEROCK_BREAK_S", -1, -1);
        case 1:
            MR::startSound(this, "SE_OJ_STRANGEROCK_BREAK_L", -1, -1);
        case 2:
            MR::startSound(this, "SE_OJ_STRANGEROCK_BREAK_M", -1, -1);
        default:
            break;
        }
        kill();
        return;
    }
    if (_90 != 2)
        return;
    TVec3f upPlusPosition2 = mPosition.operator+(upVec2);
    // TVec3f checkHitSegSphereOut;
    if (!(MR::checkHitSegmentSphere(*MR::getPlayerCenterPos(), mPosition, upPlusPosition2, v1, &checkHitSegSphereRes)))
        return;
    TVec3f checkHitSegSphereRes2(checkHitSegSphereRes);
    checkHitSegSphereRes2 *= 50.0f;
    MR::pushPlayer(checkHitSegSphereRes2);
}
