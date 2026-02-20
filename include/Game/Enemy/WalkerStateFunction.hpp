#pragma once

#include <JSystem/JGeometry.hpp>

class LiveActor;
class WalkerStateParam;

namespace WalkerStateFunction {
    bool isInSightPlayer(const LiveActor* pActor, const TVec3f& rDirection, const WalkerStateParam* pStateParam);

    void calcPassiveMovement(LiveActor* pActor, const WalkerStateParam* pStateParam);
};  // namespace WalkerStateFunction
