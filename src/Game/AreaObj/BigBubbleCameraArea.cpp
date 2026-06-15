#include "Game/AreaObj/BigBubbleCameraArea.hpp"
#include "Game/MapObj/BigBubbleHolder.hpp"

BigBubbleSwitchArea::BigBubbleSwitchArea(int formType, const char* pName) : SwitchArea(formType, pName) {
}

bool BigBubbleSwitchArea::isInVolume(const TVec3f& rPos) const {
    if (MR::isPlayerBindedBigBubble()) {
        return AreaObj::isInVolume(rPos);
    }

    return false;
}

BigBubbleCameraArea::BigBubbleCameraArea(int formType, const char* pName) : CubeCameraArea(formType, pName) {
}

bool BigBubbleCameraArea::isInVolume(const TVec3f& rVec) const {
    if (MR::isPlayerBindedBigBubble()) {
        return CubeCameraArea::isInVolume(rVec);
    }

    return false;
}
