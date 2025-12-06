#pragma once

#include "Game/NPC/Tico.hpp"

class TicoStarRing : public Tico {
public:
    TicoStarRing(const char*);

    virtual ~TicoStarRing();
    virtual void init(const JMapInfoIter&);
    virtual void kill();

    void initialize(LiveActor*, LiveActor*, const char*);
    void exeWait();

    const char* mGalaxy;  // 0x190
    LiveActor* _194;
    LiveActor* mTicoGalaxy;  // 0x198
};
