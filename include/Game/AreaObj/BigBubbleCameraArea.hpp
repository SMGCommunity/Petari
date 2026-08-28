#pragma once

#include "Game/AreaObj/CubeCamera.hpp"
#include "Game/AreaObj/SwitchArea.hpp"

class BigBubbleCameraArea : public CubeCameraArea {
public:
    BigBubbleCameraArea(int, const char*);

    /* 0x08 */ virtual ~BigBubbleCameraArea();

    /* 0x24 */ virtual bool isInVolume(const TVec3f&) const;

    /* 0x28 */ virtual const char* getManagerName() const {
        return "CubeCamera";
    }
};

class BigBubbleSwitchArea : public SwitchArea {
public:
    BigBubbleSwitchArea(int, const char*);

    /* 0x08 */ virtual ~BigBubbleSwitchArea();

    /* 0x24 */ virtual bool isInVolume(const TVec3f&) const;

    /* 0x28 */ virtual const char* getManagerName() const {
        return "SwitchArea";
    }
};
