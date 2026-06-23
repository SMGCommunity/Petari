#include "Game/AreaObj/AudioEffectArea.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

AudioEffectArea::AudioEffectArea(int formType, const char* pName) : AreaObj(formType, pName), _3C(), _40() {
}

void AudioEffectArea::init(const JMapInfoIter& rIter) {
    AreaObj::init(rIter);
    MR::connectToSceneAreaObj(this);

    _3C = mObjArg0;
    _40 = mObjArg1;

    if (mObjArg1 < 0) {
        _40 = 64;
    }
}

void AudioEffectArea::movement() {
    bool val;

    switch (_3C) {
    case 3:
    case 4:
        val = isInVolume(MR::getCamPos()) || isInVolume(*MR::getPlayerPos());
        break;
    default:
        val = isInVolume(*MR::getPlayerPos());
        break;
    }

    if (val) {
        MR::setAudioEffectType(_3C, _40);
    } else {
        MR::setAudioEffectType(0, 0);
    }
}

AudioEffectArea::~AudioEffectArea() {
}
