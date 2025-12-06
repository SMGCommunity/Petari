#pragma once

#include "Game/AreaObj/ImageEffectArea.hpp"

class DepthOfFieldArea : public ImageEffectArea {
public:
    DepthOfFieldArea(int, const char*);
    virtual ~DepthOfFieldArea();

    virtual void init(const JMapInfoIter&);
    virtual const char* getManagerName() const;

    virtual bool isSyncWithPlayer() const;

    f32 getIntensity() const;
    s32 getBlurMaxDist() const;
    s32 getBlurMinDist() const;

    bool mIsPlayerSync;  // 0x40
    u8 _41[3];
};