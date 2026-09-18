#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class HeatHazeEffect : public LiveActor {
    friend class HeatHazeDirector;

public:
    /// @brief Creates a new `HeatHazeEffect`.
    /// @param pName A pointer to the null-terminated name of the object.
    HeatHazeEffect(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void control();

public:
    /* 0x8C */ f32 _8C;
};

class HeatHazeDirector : public NameObj {
public:
    /// @brief Creates a new `HeatHazeDirector`.
    /// @param pName A pointer to the null-terminated name of the object.
    HeatHazeDirector(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void movement();

public:
    /* 0x0C */ HeatHazeEffect* mEffect;
};
