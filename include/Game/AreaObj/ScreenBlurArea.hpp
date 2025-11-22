#pragma once

#include "Game/AreaObj/ImageEffectArea.hpp"

class ScreenBlurArea : public ImageEffectArea {
public:
    ScreenBlurArea(int, const char*);
    virtual ~ScreenBlurArea();

    virtual void init(const JMapInfoIter&);

    virtual const char* getManagerName() const;

    virtual bool isSyncWithPlayer() const;

    bool mIsPlayerSync;
    u8 _41[3];
};
