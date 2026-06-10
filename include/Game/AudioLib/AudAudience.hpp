#pragma once

#include <JSystem/JAudio2/JAIAudience.hpp>
#include <JSystem/JAudio2/JAUAudience.hpp>

class AudAudience_withSetting : public JAIAudience {
public:
    /// @brief Creates a new `AudAudience_withSetting`.
    AudAudience_withSetting();

    void updateSetting();

    /* 0x04 */ JAUAudience3DSetting mSetting;
    /* 0x3C */ f32 _3C;
    /* 0x40 */ f32 _40;
    /* 0x44 */ f32 _44;
    /* 0x48 */ f32 _48;
    /* 0x4C */ f32 _4C;
    /* 0x50 */ bool mFarCamera;
};
