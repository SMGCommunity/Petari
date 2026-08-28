#pragma once

#include "Game/AreaObj/ImageEffectArea.hpp"

class ScreenBlurArea : public ImageEffectArea {
public:
    ScreenBlurArea(int, const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);

    /* 0x28 */ virtual const char* getManagerName() const {
        return "ImageEffectArea";
    }

    /* 0x2C */ virtual bool isSyncWithPlayer() const {
        return mIsPlayerSync;
    }

    /* 0x40 */ bool mIsPlayerSync;
};
