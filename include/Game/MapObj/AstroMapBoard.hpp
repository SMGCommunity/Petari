#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class AstroMapBoard : public MapObjActor {
public:
    AstroMapBoard(const char*);

    virtual ~AstroMapBoard();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void connectToScene(const MapObjActorInitInfo&);

    void exeWait();
};
