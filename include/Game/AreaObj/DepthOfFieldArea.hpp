#pragma once

#include "Game/AreaObj/ImageEffectArea.hpp"

class DepthOfFieldArea : public ImageEffectArea {
public:
    DepthOfFieldArea(int, const char*);

    virtual void init(const JMapInfoIter&);

    virtual const char* getManagerName() const {
        return "ImageEffectArea";
    }

    virtual bool isSyncWithPlayer() const {
        return mIsPlayerSync;
    }

    f32 getIntensity() const;
    s32 getBlurMaxDist() const;
    s32 getBlurMinDist() const;

    /* 0x40 */ bool mIsPlayerSync;
};
