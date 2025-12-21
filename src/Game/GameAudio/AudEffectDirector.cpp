#include "Game/GameAudio/AudEffectDirector.hpp"
#include "Game/AudioLib/AudWrap.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

struct AudEffectData {
    /* 0x00 */ f32 fxSend;
    /* 0x04 */ s32 cutoff;
    /* 0x08 */ f32 volume;
    /* 0x0C */ u32 _C;
};

namespace {
    const AudEffectData cAudEffectDataList[5] = {
        {0.0f, 0x7F, 1.0f, 0x5A}, {1.0f, 0x7F, 1.0f, 0x5A}, {0.0f, 0x0A, 1.0f, 0x5A}, {1.0f, 0x7F, 0.0f, 0x5A}, {0.8f, 0x7F, 1.0f, 0x5A},
    };
}

AudEffectDirector::AudEffectDirector() : NameObj("オーディオエフェクトディレクター") {
    _C = 0;
    _10 = 0;
    _14 = 0;
}

AudEffectDirector::~AudEffectDirector() {}

void AudEffectDirector::init(const JMapInfoIter& rIter) {
    MR::connectToScene(this, 24, -1, -1, -1);
}

void AudEffectDirector::movement() {
    if (_10 != _C) {
        setAudioEffectParam(_C);
        _10 = _C;
    }

    playEffectSound(_C);
    _14 = 0;
}

void AudEffectDirector::setEffectType(s32 a1, s32 a2) {
    if (a2 < 0)
        a2 = 0;

    if (a2 > 0x7f)
        a2 = 0x7f;

    if (a2 >= _14) {
        _C = a1;
        _14 = a2;
    }
}

void AudEffectDirector::setAudioEffectParam(s32 a1) {
    AudEffector* pAudEffector = AudWrap::getSystem()->getAudEffector();

    pAudEffector->setFxSend(::cAudEffectDataList[a1].fxSend);
    pAudEffector->setCutoff(::cAudEffectDataList[a1].cutoff);

    if (!MR::isStageStateScenarioOpeningCamera())
        MR::moveVolumeStageBGM(::cAudEffectDataList[a1].volume, ::cAudEffectDataList[a1]._C);

    switch (a1) {
    case 1:
        return pAudEffector->setSpecialFx(1);
    case 3:
        return pAudEffector->setSpecialFx(2);
    default:
        return pAudEffector->setSpecialFx(0);
    }
}

void AudEffectDirector::playEffectSound(s32 a1) {
    switch (a1) {
    case 3:
        return MR::startAtmosphereLevelSE("SE_AT_LV_WATERFALL_CAVE", -1, -1);
    case 4:
        return MR::startAtmosphereLevelSE("SE_AT_LV_PHNCV_TERESA_ROOM", -1, -1);
    }
}
