#pragma once

#include "Game/Boss/DinoPackunTailNode.hpp"
#include "Game/Util/JointController.hpp"

class DinoPackunTailRoot : public DinoPackunTailNode {
public:
    DinoPackunTailRoot(const char*, DinoPackun*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x84 */ virtual void addNodeVelocity(const TVec3f&);
    /* 0x88 */ virtual JointController* createJointControllerOwn(LiveActor*, const char*);

    bool calcJointDirection(TPos3f*, const JointControllerInfo&);
};
