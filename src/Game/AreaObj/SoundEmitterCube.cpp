#include "Game/AreaObj/SoundEmitterCube.hpp"
#include "Game/AudioLib/AudAnmSoundObject.hpp"
#include "Game/Util.hpp"

namespace {
    const char* cSoundNameTable[] = {"SE_AT_LV_GRAVE", "SE_AT_LV_KOOPA_WIND", "SE_AT_LV_WIND_DESERT", "SE_AT_LV_WIND_ICE_MT_TOP"};
};  // namespace

SoundEmitterCube::~SoundEmitterCube() {
}

SoundEmitterCube::SoundEmitterCube(int formType, const char* pName) : AreaObj(formType, pName) {
    mSoundObj = nullptr;
    _40.x = 0.0f;
    _40.y = 0.0f;
    _40.z = 0.0f;
    mSoundName = nullptr;
}

void SoundEmitterCube::init(const JMapInfoIter& rIter) {
    AreaObj::init(rIter);
    mSoundName = cSoundNameTable[mObjArg0];
    mSoundObj = new AudAnmSoundObject(&_40, 4, MR::getCurrentHeap());

    MR::connectToSceneAreaObj(this);
    MR::registerDemoSimpleCastAll(this);
}
