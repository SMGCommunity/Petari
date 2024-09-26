#pragma once

#include "Game/Screen/LayoutActor.hpp"

class ScenarioTitle : public LayoutActor {
public:
    ScenarioTitle();

    virtual ~ScenarioTitle();
    virtual void init(const JMapInfoIter&);

    void start();
    void end();
    void exeAppear();
    void exeEnd();

    inline void exeWait() {}
};