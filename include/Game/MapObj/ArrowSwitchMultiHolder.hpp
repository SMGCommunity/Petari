#pragma once

#include "Game/MapObj/ArrowSwitchMulti.h"
#include "Game/LiveActor/LiveActorGroup.h"

class ArrowSwitchMultiHolder : public DeriveActorGroup<ArrowSwitchMulti> {
public:
    ArrowSwitchMultiHolder();

    ArrowSwitchMulti* findSwitch(const JMapIdInfo *);
};

namespace MR {
    void createArrowSwitchMultiHolder();
    void registerArrowSwitchMulti(ArrowSwitchMulti *);
    void registerArrowSwitchTarget(ArrowSwitchTarget *);
};