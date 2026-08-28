#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class DinoPackunDemoPosition : public LiveActor {
public:
    /// @brief Creates a new `DinoPackunDemoPosition`.
    /// @param pName A pointer to the null-terminated name of the object.
    DinoPackunDemoPosition(const char* pName);

    /* 0x08 */ virtual ~DinoPackunDemoPosition();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();

    void startOpeningDemo();
    void endDemo();
    void exeOpeningDemo();
};
