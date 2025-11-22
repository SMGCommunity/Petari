#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class InvisiblePolygonObj : public LiveActor {
public:
    InvisiblePolygonObj(const char*);

    virtual void init(const JMapInfoIter&);
    virtual MtxPtr getBaseMtx() const;

    void initBaseMtx();
    void initCollision(const JMapInfoIter&);

    TMtx34f mMatrix;
};
