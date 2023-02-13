#include "Game/AreaObj/SoundEmitterSphere.h"
#include "Game/AudioLib/AudAnmSoundObject.h"

namespace {
    const char *gSoundNameTable[6] = {
        "SE_AT_LV_MAGMA_AMBIENT",
        "SE_OJ_LV_FLOATING_SAND",
        "SE_AT_LV_WIND_DESERT",
        "SE_AT_LV_MAGMA_WIND_VOLCANO",
        "SE_AT_LV_KOOPA_FLOAT_SAND",
        "SE_AT_LV_WIND_RABBIT_MAZE"
    };
}

SoundEmitterSphere::SoundEmitterSphere(int type, const char *pName) : AreaObj(type, pName) {
    mSoundObj = nullptr;
    _40.x = 0.0f;
    _40.y = 0.0f;
    _40.z = 0.0f;
    mSoundName = nullptr;
    mSoundIndex = 0;
}

SoundEmitterSphere::~SoundEmitterSphere() {

}

void SoundEmitterSphere::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);
    mSoundName = gSoundNameTable[mObjArg0];
    mSoundIndex = mObjArg0;
    mSoundObj = new AudAnmSoundObject(&_40, 4, MR::getCurrentHeap());

    MR::connectToSceneAreaObj(this);
    MR::registerDemoSimpleCastAll(this);
}

void SoundEmitterSphere::movement() {

}
