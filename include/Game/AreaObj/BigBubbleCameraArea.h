#pragma once

#include "Game/AreaObj/CubeCamera.h"
#include "Game/AreaObj/SwitchArea.h"

class BigBubbleCameraArea : public CubeCameraArea {
public:
    BigBubbleCameraArea(int, const char *);
    virtual ~BigBubbleCameraArea();

    virtual bool isInVolume(const TVec3f &) const;
    virtual const char* getManagerName() const;
};