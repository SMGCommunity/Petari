#include "Game/AreaObj/BigBubbleCameraArea.h"

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