#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class MapParts : public LiveActor {
public:
    MapParts(const char*);

    /* 0x08 */ virtual ~MapParts() {};

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual void appear();
    virtual s32 getSensorNumMax() const;
    /* 0x78 */ virtual void connectToScene();

    static s32 getMoveStartSignalTime();
    void initModelAndCollision(const JMapInfoIter&);

    TVec3f _8C;
};
