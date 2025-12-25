#pragma once

#include "Game/AreaObj/AreaObj.hpp"

struct BgmMuteSet {
    const char* mGalaxyName;
    u32 _4;
    u32 _8;
};

class BgmProhibitArea : public AreaObj {
public:
    BgmProhibitArea(int, const char*);

    virtual ~BgmProhibitArea();
    virtual void init(const JMapInfoIter&);
    virtual void movement();
    virtual const char* getManagerName() const;

    u8 _3C;
    u8 _3D;
    TVec3f _40;
};