#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class DinoPackunDemoPosition : public LiveActor {
public:
    DinoPackunDemoPosition(const char*);

    virtual ~DinoPackunDemoPosition();
    virtual void init(const JMapInfoIter&);
    virtual void control();

    void startOpeningDemo();
    void endDemo();
    void exeOpeningDemo();
};
