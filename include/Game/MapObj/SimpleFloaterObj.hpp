#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/MapPartsFloatingForce.hpp"

class SimpleFloaterObj : public LiveActor {
public:
    SimpleFloaterObj(const char *);
    virtual ~SimpleFloaterObj();

    virtual void init(const JMapInfoIter &);

    void exeWait();
    bool requiredInline() {
        return PSVECMag(&_90->_50) < 0.30000001f;
    }

    const char * _8C;
    MapPartsFloatingForce* _90;
    TVec3f _94;
};
