#pragma once

#include "Game/AreaObj/ImageEffectArea.hpp"

class ScreenBlurArea : public ImageEffectArea {
public:
    ScreenBlurArea(int, const char*);

    virtual ~ScreenBlurArea();
    virtual void init(const JMapInfoIter&);

    virtual const char* getManagerName() const {
        return "ImageEffectArea";
    }

    virtual bool isSyncWithPlayer() const {
        return mIsPlayerSync;
    }

    /* 0x40 */ bool mIsPlayerSync;
};
