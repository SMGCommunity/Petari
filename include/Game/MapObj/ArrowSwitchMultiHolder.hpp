#pragma once

#include "Game/LiveActor/LiveActorGroup.hpp"

class ArrowSwitchMulti;
class ArrowSwitchTarget;
class JMapIdInfo;

class ArrowSwitchMultiHolder : public DeriveActorGroup< ArrowSwitchMulti > {
public:
    ArrowSwitchMultiHolder();

    ArrowSwitchMulti* findSwitch(const JMapIdInfo*);
};

namespace MR {
    void createArrowSwitchMultiHolder();
    void registerArrowSwitchMulti(ArrowSwitchMulti*);
    void registerArrowSwitchTarget(ArrowSwitchTarget*);
};  // namespace MR