#pragma once

#include "Game/Screen/LayoutActor.h"

class IconComet : public LayoutActor {
public:
    IconComet();

    virtual ~IconComet();
    virtual void init(const JMapInfoIter &);

    bool appearIfLanding(const char *);
    void appearByCometNameId(int);
};

namespace NrvIconComet {
    NERVE_DECL_NULL(IconCometNrvWait);
};