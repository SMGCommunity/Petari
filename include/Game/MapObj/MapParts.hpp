#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class MapParts : public LiveActor {
public:
    MapParts(const char*);

    virtual inline ~MapParts();

    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual s32 getSensorNumMax() const;
    virtual void connectToScene();

    static s32 getMoveStartSignalTime();
    void initModelAndCollision(const JMapInfoIter&);

    TVec3f _8C;
};
