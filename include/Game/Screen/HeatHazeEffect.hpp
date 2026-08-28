#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class HeatHazeEffect : public LiveActor {
    friend class HeatHazeDirector;

public:
    /// @brief Creates a new `HeatHazeEffect`.
    /// @param pName A pointer to the null-terminated name of the object.
    HeatHazeEffect(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x24 */ virtual void appear();
    /* 0x48 */ virtual void control();

private:
    /* 0x8C */ f32 _8C;
};

class HeatHazeDirector : public NameObj {
public:
    /// @brief Creates a new `HeatHazeDirector`.
    /// @param pName A pointer to the null-terminated name of the object.
    HeatHazeDirector(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x14 */ virtual void movement();

private:
    /* 0x0C */ HeatHazeEffect* mEffect;
};
