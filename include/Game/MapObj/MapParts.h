#pragma once

#include "Game/LiveActor/LiveActor.h"

class MapParts : public LiveActor {
public:
    MapParts(const char *);

    virtual ~MapParts();
    virtual void init(const JMapInfoIter &);
    virtual void appear();
    virtual s32 getSensorNumMax() const;
    virtual void connectToScene();

    s32 getMoveStartSignalTime();
    void initModelAndCollision(const JMapInfoIter &);

    TVec3f _8C;
};