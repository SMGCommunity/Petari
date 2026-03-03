#include "Game/Enemy/WalkerStateParam.hpp"

WalkerStateParam::WalkerStateParam()
    : mGravityAccel(1.0f), mAirFriction(0.99f), mGroundFriction(0.93f), mPlayerNearDistance(1000.0f), mPlayerSightFanDegreeH(80.0f),
      mPlayerSightFanDegreeV(40.0f) {
}
