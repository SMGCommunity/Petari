#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class AstroMapBoard : public MapObjActor {
public:
    AstroMapBoard(const char*);

    /* 0x08 */ virtual ~AstroMapBoard();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x78 */ virtual void connectToScene(const MapObjActorInitInfo&);

    void exeWait();
};
