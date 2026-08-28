#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class BgmProhibitArea : public AreaObj {
public:
    BgmProhibitArea(int, const char*);

    /* 0x08 */ virtual ~BgmProhibitArea();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x14 */ virtual void movement();

    /* 0x28 */ virtual const char* getManagerName() const {
        return "BgmProhibitArea";
    }

    /* 0x3C */ bool _3C;
    /* 0x3D */ bool _3D;
    /* 0x40 */ TVec3f _40;
};
