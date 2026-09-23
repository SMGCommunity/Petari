#include "Game/AreaObj/SoundEmitterSphere.hpp"
#include "Game/AudioLib/AudAnmSoundObject.hpp"
#include "Game/AudioLib/AudMicWrap.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    const char* cSoundNameTable[] = {"SE_AT_LV_MAGMA_AMBIENT",      "SE_OJ_LV_FLOATING_SAND",    "SE_AT_LV_WIND_DESERT",
                                     "SE_AT_LV_MAGMA_WIND_VOLCANO", "SE_AT_LV_KOOPA_FLOAT_SAND", "SE_AT_LV_WIND_RABBIT_MAZE"};
}  // namespace

SoundEmitterSphere::~SoundEmitterSphere() {
}

SoundEmitterSphere::SoundEmitterSphere(int formType, const char* pName)
    : AreaObj(formType, pName), mSoundObj(), _40(0.0f, 0.0f, 0.0f), mSoundName(), mSoundIndex() {
}

void SoundEmitterSphere::init(const JMapInfoIter& rIter) {
    AreaObj::init(rIter);

    mSoundName = ::cSoundNameTable[mObjArg0];
    mSoundIndex = mObjArg0;
    mSoundObj = new AudAnmSoundObject(&_40, 4, MR::getCurrentHeap());

    MR::connectToSceneAreaObj(this);
    MR::registerDemoSimpleCastAll(this);
}

void SoundEmitterSphere::movement() {
    switch (mSoundIndex) {
    case 1:
    case 2:
    case 4:
    case 5:
        _40.set(MR::getCameraWatchPos());
        break;
    case 0:
    case 3:
    default:
        _40.set(AudMicWrap::getMicPos());
        break;
    }

    if (!isInVolume(_40)) {
        TVec3f center;
        MR::calcSpherePos(&center, this);
        f32 radius = MR::getSphereRadius(this);
        f32 rate = radius / (_40 - center).length();
        _40.x = center.x + rate * (_40.x - center.x);
        _40.y = center.y + rate * (_40.y - center.y);
        _40.z = center.z + rate * (_40.z - center.z);
    }

    mSoundObj->process();
    MR::startSoundObjectLevel(mSoundObj, mSoundName);
}
