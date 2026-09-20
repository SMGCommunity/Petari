#include "Game/MapObj/LavaHomeSeesaw.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/MaterialCtrl.hpp"
#include "Game/MapObj/LavaHomeSeesawRotator.hpp"
#include "Game/MapObj/MapPartsFunction.hpp"
#include "Game/Util.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "math_types.hpp"
#include "revolution/types.h"

LavaHomeSeesaw::LavaHomeSeesaw(const char* pName) : LiveActor(pName), mSeesawRotator(), mMtxSetter(), _94(gZeroVec), mYOffset() {
}

void LavaHomeSeesaw::initAfterPlacement() {
    TVec3f gravity(mGravity);
    gravity *= 1000.0f;
    if (!MR::getFirstPolyOnLineToMap(&_94, nullptr, mPosition, gravity)) {
        MR::calcMapGroundUpper(&_94, this);
    }

    mYOffset = mPosition.distance(_94);
    if ((mPosition - _94).dot(mGravity) < 0.0f) {
        mYOffset *= -1.0f;
    }

    TVec3f uselessVec(0.0f, mYOffset, 0.0f);
    mMtxSetter->updateMtxUseBaseMtxWithLocalOffset(TVec3f(0.0f, mYOffset, 0.0f));
    MR::offCalcGravity(this);
}

void LavaHomeSeesaw::control() {
    if (mSeesawRotator != nullptr && mSeesawRotator->isWorking()) {
        mSeesawRotator->movement();
    }
}

void LavaHomeSeesaw::calcAndSetBaseMtx() {
    if (mSeesawRotator == nullptr || !mSeesawRotator->isWorking()) {
        LiveActor::calcAndSetBaseMtx();
    } else {
        TPos3f mtx;
        mtx.identity();
        mtx.set(mSeesawRotator->getRotateMtx());
        mtx.setTrans(mPosition);
        MR::setBaseTRMtx(this, mtx);
    }
}

void LavaHomeSeesaw::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    const char* name;
    MR::getObjectName(&name, rIter);
    initModelManagerWithAnm(name, nullptr, true);
    mMtxSetter = MR::initDLMakerProjmapEffectMtxSetter(this);
    MR::newDifferedDLBuffer(this);
    initSound(4, false);
    initHitSensor(1);
    MR::initCollisionParts(this, name, MR::addBodyMessageSensorMapObj(this), nullptr);
    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
    MR::connectToSceneCollisionMapObj(this);
    mSeesawRotator = new LavaHomeSeesawRotator(this, "SE_OJ_LV_LAVA_SEESAW_MOVE", 0.1f);
    mSeesawRotator->init(rIter);
    mSeesawRotator->start();
    makeActorAppeared();
}

void MapPartsFunction::start() {
}

bool MapPartsFunction::isWorking() const {
    return true;
}
