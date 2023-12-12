#pragma once

#include "Game/AreaObj/AreaObj.h"
#include "Game/Util.h"

struct BgmMuteSet {
    const char* mGalaxyName;
    u32 _4;
    u32 _8;
};

namespace {
    static BgmMuteSet sBgmMuteSet[2] = {
        { "OceanPhantomCaveGalaxy", 7, 1 },
        { "SoundMapCodeTest", 7, 1 }
    };

    BgmMuteSet* findDataElement(const char *);
};

class BgmProhibitArea : public AreaObj {
public:
    BgmProhibitArea(int, const char *);

    virtual ~BgmProhibitArea();
    virtual void init(const JMapInfoIter &);
    virtual void movement();
    virtual const char* getManagerName() const;

    u8 _3C;
    u8 _3D;
    TVec3f _40;
};