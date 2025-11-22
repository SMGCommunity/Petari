#pragma once

#include "Game/AreaObj/CubeCamera.hpp"
#include "Game/AreaObj/SwitchArea.hpp"

class BigBubbleSwitchArea : public SwitchArea {
public:
    BigBubbleSwitchArea(int, const char*);
    virtual ~BigBubbleSwitchArea();

    virtual bool        isInVolume(const TVec3f&) const;
    virtual const char* getManagerName() const;
};

class BigBubbleCameraArea : public CubeCameraArea {
public:
    BigBubbleCameraArea(int, const char*);

    virtual ~BigBubbleCameraArea();

    virtual bool        isInVolume(const TVec3f&) const;
    virtual const char* getManagerName() const;
};