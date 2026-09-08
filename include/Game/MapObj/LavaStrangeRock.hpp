#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class LodCtrl;

class LavaStrangeRock : public LiveActor {
public:
    enum Type {
        /* 0x00 */ Type_0,
        /* 0x01 */ Type_1,
        /* 0x02 */ Type_2,
        /* 0x03 */ Type_3,
    };

    /// @brief Creates a new `LavaStrangeRock`.
    /// @param pName A pointer to the null-terminated name of the object.
    LavaStrangeRock(const char* pName);

    virtual ~LavaStrangeRock();
    virtual void init(const JMapInfoIter&);
    virtual void control();

    void initModelAndClipping(const JMapInfoIter&);
    void initEffect();
    void exeWait();

    /* 0x8C */ s32 mRockType;
    /* 0x90 */ s32 _90;
    /* 0x94 */ LodCtrl* mLodCtrlPlanet;
};
