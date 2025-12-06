#pragma once

#include "Game/NPC/TicoFat.hpp"

class TicoStarRing;

class TicoGalaxy : public TicoFat {
public:
    TicoGalaxy(const char*);

    virtual ~TicoGalaxy();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void shootStarPiece();
    virtual void startEat();
    virtual bool enableAppear();
    virtual void initStarPieceSaveData(const JMapInfoIter&);
    virtual void addStarPieceSaveData(s32);
    virtual void disappear(bool);
    virtual void appearInformation() const;

    const char* mGalaxyName;  // 0x208
    TicoStarRing* mStarRing;  // 0x20C
};
