#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class BgmProhibitArea : public AreaObj {
public:
    BgmProhibitArea(int, const char*);

    virtual ~BgmProhibitArea();
    virtual void init(const JMapInfoIter&);
    virtual void movement();

    virtual const char* getManagerName() const {
        return "BgmProhibitArea";
    }

    /* 0x3C */ bool _3C;
    /* 0x3D */ bool _3D;
    /* 0x40 */ TVec3f _40;
};
