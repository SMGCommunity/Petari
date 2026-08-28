#pragma once

#include "Game/NPC/TicoFat.hpp"

class TicoStarRing;

class TicoGalaxy : public TicoFat {
public:
    TicoGalaxy(const char*);

    /* 0x08 */ virtual ~TicoGalaxy();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x78 */ virtual void shootStarPiece();
    /* 0x80 */ virtual void startEat();
    /* 0x84 */ virtual bool enableAppear();
    /* 0x88 */ virtual void initStarPieceSaveData(const JMapInfoIter&);
    /* 0x8C */ virtual void addStarPieceSaveData(s32);
    /* 0x90 */ virtual void disappear(bool);
    /* 0x94 */ virtual void appearInformation() const;

    const char* mGalaxyName;  // 0x208
    TicoStarRing* mStarRing;  // 0x20C
};
