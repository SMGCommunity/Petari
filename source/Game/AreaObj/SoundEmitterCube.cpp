#include "Game/AreaObj/SoundEmitterCube.h"
#include "Game/AudioLib/AudAnmSoundObject.h"

namespace {
    const char *gSoundNameTable[4] = {
        "SE_AT_LV_GRAVE",
        "SE_AT_LV_KOOPA_WIND",
        "SE_AT_LV_WIND_DESERT",
        "SE_AT_LV_WIND_ICE_MT_TOP"
    };
}

SoundEmitterCube::SoundEmitterCube(int type, const char *pName) : AreaObj(type, pName) {
    mSoundObj = nullptr;
	_40.x = 0.0f;
	_40.y = 0.0f;
	_40.z = 0.0f;
    mSoundName = nullptr;
}

SoundEmitterCube::~SoundEmitterCube() {

}

void SoundEmitterCube::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);
    mSoundName = gSoundNameTable[mObjArg0];
    mSoundObj = new AudAnmSoundObject(&_40, 4, MR::getCurrentHeap());

    MR::connectToSceneAreaObj(this);
    MR::registerDemoSimpleCastAll(this);
}
