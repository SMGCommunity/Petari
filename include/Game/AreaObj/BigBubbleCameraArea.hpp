#pragma once

#include "Game/AreaObj/CubeCamera.hpp"
#include "Game/AreaObj/SwitchArea.hpp"

class BigBubbleCameraArea : public CubeCameraArea {
public:
    BigBubbleCameraArea(int, const char*);

    virtual bool isInVolume(const TVec3f&) const;
    virtual const char* getManagerName() const { return "CubeCamera"; }
};

class BigBubbleSwitchArea : public SwitchArea {
public:
    BigBubbleSwitchArea(int, const char*);

    virtual bool isInVolume(const TVec3f&) const;
    virtual const char* getManagerName() const { return "SwitchArea"; }
};
