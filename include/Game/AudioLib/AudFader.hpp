#pragma once

class AudFader {
public:
    AudFader();

    void set(float desiredVolume, long fadeTime);
    void update();

    /* 0x00 */ float mCurrentVolume;
    /* 0x04 */ float mFinalVolume;
    /* 0x08 */ float mStepVolume;    ///< Volume intensity increase/decrease per AudFader::update() call until mCurrentVolume reaches mFinalVolume value
};
