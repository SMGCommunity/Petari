#include "Game/AreaObj/SoundEmitterSphere.hpp"
#include "Game/AudioLib/AudAnmSoundObject.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace {
    const char* cSoundNameTable[] = {"SE_AT_LV_MAGMA_AMBIENT",      "SE_OJ_LV_FLOATING_SAND",    "SE_AT_LV_WIND_DESERT",
                                     "SE_AT_LV_MAGMA_WIND_VOLCANO", "SE_AT_LV_KOOPA_FLOAT_SAND", "SE_AT_LV_WIND_RABBIT_MAZE"};
};  // namespace

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

// SoundEmitterSphere::movement
