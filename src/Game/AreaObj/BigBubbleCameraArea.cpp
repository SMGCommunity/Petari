#include "Game/AreaObj/BigBubbleCameraArea.hpp"
#include "Game/MapObj/BigBubbleHolder.hpp"

BigBubbleSwitchArea::BigBubbleSwitchArea(int type, const char* pName) : SwitchArea(type, pName) {}

bool BigBubbleSwitchArea::isInVolume(const TVec3f& rPos) const {
    if (MR::isPlayerBindedBigBubble()) {
        return AreaObj::isInVolume(rPos);
    }

    return false;
}

BigBubbleCameraArea::BigBubbleCameraArea(int a1, const char* pName) : CubeCameraArea(a1, pName) {}

bool BigBubbleCameraArea::isInVolume(const TVec3f& rVec) const {
    if (MR::isPlayerBindedBigBubble()) {
        return CubeCameraArea::isInVolume(rVec);
    }

    return false;
}
