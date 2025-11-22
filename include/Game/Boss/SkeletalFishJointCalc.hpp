#pragma once

#include "Game/Boss/SkeletalFishRailControl.hpp"
#include "Game/Util/JointController.hpp"

class SkeletalFishJointCalc {
public:
    static void calcJoint(TPos3f*, const TPos3f*, SkeletalFishRailControl*, const JointControllerInfo&);
};