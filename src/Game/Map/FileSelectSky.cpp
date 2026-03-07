#include "Game\Map\FileSelectSky.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/MaterialCtrl.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"
#include "revolution/types.h"

namespace NrvFileSelectSky {
    NEW_NERVE(FileSelectSkyNrvWait, FileSelectSky, Wait);
};  

FileSelectSky::FileSelectSky(const char* pName) : LiveActor(pName) {
    _8C = 0.0f;
    _90 = 0.0f;
    mProjmapEffectMtxSetter = nullptr;
}

FileSelectSky::~FileSelectSky() {}

void FileSelectSky::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm("CometNearOrbitSky", nullptr, false);
    mScale.x = 0.8f;
    mScale.y = 0.8f;
    mScale.z = 0.8f;
    mProjmapEffectMtxSetter = MR::initDLMakerProjmapEffectMtxSetter(this);
    MR::connectToSceneSky(this);
    initEffectKeeper(0, nullptr, false);
    MR::invalidateClipping(this);
    initNerve(&NrvFileSelectSky::FileSelectSkyNrvWait::sInstance);
    makeActorDead();
}

void FileSelectSky::calcAnim() {
    LiveActor::calcAnim();
    mProjmapEffectMtxSetter->updateMtxUseBaseMtx();
}

void FileSelectSky::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _94);
}

bool FileSelectSky::receiveOtherMsg(u32,HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}

void FileSelectSky::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "CometNearOrbitSky", nullptr);
        MR::startBtk(this, "CometNearOrbitSky");
    }

    if (MR::isFirstStep(this)) {
        _8C = 0.0f;
        _90 = 0.0f;
    }

    TPos3f a1, a2;
    a2.makeRotate(TVec3f(0.0f, 1.0f, 0.0f), _90);
    a1.makeRotate(TVec3f(1.0f, 0.0f, 0.0f), _8C);
    _94.concat(a2, a1);
    _94.invert(_94);
    f32 steps = (3.1415927f * getNerveStep()) / 3000.0f;
    if (steps < 0.0f) {
        steps = -steps;
    }
    f32 value = _90 + 0.001f;
    f32 temp = 1.0f - JMath::sSinCosTable.cosShort(8 * steps);
    _90 = value;
    _8C = (3.0f * ((temp * 0.5f) * 3.1415927f)) * 0.25f;
}
