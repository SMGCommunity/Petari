#pragma once

#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/MapObj/ArrowSwitchMulti.hpp"

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