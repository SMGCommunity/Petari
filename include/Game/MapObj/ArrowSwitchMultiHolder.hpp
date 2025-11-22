#pragma once

#include "Game/MapObj/ArrowSwitchMulti.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"

class ArrowSwitchMultiHolder : public DeriveActorGroup< ArrowSwitchMulti > {
public:
    ArrowSwitchMultiHolder();

    ArrowSwitchMulti* findSwitch(const JMapIdInfo*);
};

namespace MR {
    void createArrowSwitchMultiHolder();
    void registerArrowSwitchMulti(ArrowSwitchMulti*);
    void registerArrowSwitchTarget(ArrowSwitchTarget*);
}; // namespace MR