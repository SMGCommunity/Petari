#include "Game/LiveActor/MaterialCtrl.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/FileSelectSky.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace {
    const f32 cAngleIncY = 0.001f;
    const f32 cCycleX = 3000.0f;
};  // namespace

namespace NrvFileSelectSky {
    NEW_NERVE(FileSelectSkyNrvWait, FileSelectSky, Wait);
};  // namespace NrvFileSelectSky

FileSelectSky::~FileSelectSky() {
}

FileSelectSky::FileSelectSky(const char* pName) : LiveActor(pName), mAngleX(), mAngleY(), mProjmapEffectMtxSetter() {
}

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
    MR::setBaseTRMtx(this, mBaseMtx);
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
        mAngleX = 0.0f;
        mAngleY = 0.0f;
    }

    TPos3f rotateX, rotateY;
    rotateY.makeRotate(TVec3f(0.0f, 1.0f, 0.0f), mAngleY);
    rotateX.makeRotate(TVec3f(1.0f, 0.0f, 0.0f), mAngleX);
    mBaseMtx.concat(rotateY, rotateX);
    mBaseMtx.invert(mBaseMtx);
    f32 step = (getNerveStep() * PI) / ::cCycleX;

    if (step < 0.0f) {
        step = -step;
    }

    mAngleX = (1.0f - JMACosShort(step * 8)) * 3.0f / 2.0f * PI / 4.0f;
    mAngleY += ::cAngleIncY;
}
