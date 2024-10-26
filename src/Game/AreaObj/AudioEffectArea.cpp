#include "Game/AreaObj/AudioEffectArea.hpp"
#include "Game/Util.hpp"

AudioEffectArea::AudioEffectArea(int a1, const char *pName) : AreaObj(a1, pName) {
    _3C = 0;
    _40 = 0;
}

void AudioEffectArea::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);
    MR::connectToSceneAreaObj(this);

    _40 = mObjArg1;
    _3C = mObjArg0;

    if (mObjArg1 < 0) {
        _40 = 0x40;
    }
}

void AudioEffectArea::movement() {
    bool val;

    switch(_3C) {
        case 3:
        case 4:
            val = false;

            if (isInVolume(MR::getCamPos()) || isInVolume(*MR::getPlayerPos())) {
                val = true;
            }

            break;
        default:
            val = isInVolume(*MR::getPlayerPos());
            break;
    }

    if (val) {
        MR::setAudioEffectType(_3C, _40);
    }
    else {
        MR::setAudioEffectType(0, 0);
    }
}

const char* AudioEffectArea::getManagerName() const {
    return "AudioEffectArea";
}