#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class DinoPackunDemoPosition : public LiveActor {
public:
    /// @brief Creates a new `DinoPackunDemoPosition`.
    /// @param pName A pointer to the null-terminated name of the object.
    DinoPackunDemoPosition(const char* pName);

    virtual ~DinoPackunDemoPosition();
    virtual void init(const JMapInfoIter&);
    virtual void control();

    void startOpeningDemo();
    void endDemo();
    void exeOpeningDemo();
};
