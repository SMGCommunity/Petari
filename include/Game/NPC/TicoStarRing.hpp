#pragma once

#include "Game/NPC/Tico.hpp"

class TicoStarRing : public Tico {
public:
    /// @brief Creates a new `TicoStarRing`.
    /// @param pName A pointer to the null-terminated name of the object.
    TicoStarRing(const char* pName);

    virtual void init(const JMapInfoIter&);
    virtual void kill();

    void initialize(LiveActor*, LiveActor*, const char*);

    void exeWait();

    /* 0x190 */ const char* mGalaxyName;
    /* 0x194 */ LiveActor* _194;
    /* 0x198 */ LiveActor* mTicoGalaxy;
};
