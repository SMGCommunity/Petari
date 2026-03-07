#pragma once

#include "Game/Boss/DinoPackunTailNode.hpp"
#include "Game/Util/JointController.hpp"

class DinoPackunTailRoot : public DinoPackunTailNode {
public:
    DinoPackunTailRoot(const char*, DinoPackun*);

    virtual ~DinoPackunTailRoot();
    virtual void init(const JMapInfoIter&);
    virtual void addNodeVelocity(const TVec3f&);
    virtual JointController* createJointControllerOwn(LiveActor*, const char*);

    bool calcJointDirection(TPos3f*, const JointControllerInfo&);
};
