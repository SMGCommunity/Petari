#pragma once

#include <JSystem/JGeometry.hpp>

class LiveActor;
class WalkerStateParam;

class WalkerStateFunction {
public:
    static bool isInSightPlayer(const LiveActor*, const TVec3f&, const WalkerStateParam*);

    static void calcPassiveMovement(LiveActor*, const WalkerStateParam*);
};
