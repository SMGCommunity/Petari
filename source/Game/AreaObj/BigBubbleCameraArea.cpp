#include "Game/AreaObj/BigBubbleCameraArea.h"

SwitchArea::~SwitchArea() {

}

BigBubbleSwitchArea::BigBubbleSwitchArea(int type, const char *pName) : SwitchArea(type, pName) {

}

BigBubbleSwitchArea::~BigBubbleSwitchArea() {

}

bool BigBubbleSwitchArea::isInVolume(const TVec3f &rPos) const {
    if (MR::isPlayerBindedBigBubble()) {
        return AreaObj::isInVolume(rPos);
    }

    return false;
} 

const char *BigBubbleSwitchArea::getManagerName() const {
    return "SwitchArea";
}

BigBubbleCameraArea::BigBubbleCameraArea(int a1, const char *pName) : CubeCameraArea(a1, pName) {

}

BigBubbleCameraArea::~BigBubbleCameraArea() {

}

bool BigBubbleCameraArea::isInVolume(const TVec3f &rVec) const {
    if (MR::isPlayerBindedBigBubble()) {
        return CubeCameraArea::isInVolume(rVec);
    }

    return false;
}

const char* BigBubbleCameraArea::getManagerName() const {
    return "CubeCamera";
}