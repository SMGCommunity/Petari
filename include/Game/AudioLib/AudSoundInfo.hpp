#pragma once

#include "JSystem/JAudio2/JAUStdSoundInfo.hpp"

// TODO: JAUStdSoundInfo or just JAUSoundInfo?
class AudSoundInfo : public JAUSoundInfo {
public:
    u16 getSoundSw(JAISoundID) const;
    bool isLimitNumPlaying(JAISoundID) const;
    bool isLimitCountPlaying(JAISoundID) const;
    u16 getChordResIdBgm(JAISoundID) const;
    bool isRhythmBgm(JAISoundID) const;
    bool isFanfareSe(JAISoundID) const;
    bool isUseFxMix(JAISoundID) const;
    bool isUseLpf(JAISoundID) const;
};
