#pragma once

class AudFader {
public:
    AudFader();
    void set(float desiredVolume, long fadeTime);
    void update();

    float mCurrentVolume;
    float mFinalVolume;
    float mStepVolume;
};
