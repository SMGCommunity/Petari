#pragma once

#include "Game/Screen/LayoutActor.hpp"

class IconComet : public LayoutActor {
public:
    IconComet();

    /* 0x0C */ virtual void init(const JMapInfoIter&);

    bool appearIfLanding(const char*);
    void appearByCometNameId(int);
    void exeWait();
};