#pragma once

#include "Game/NameObj/NameObj.h"

class CameraRailHolder : public NameObj {
public:
    class ZoneRail {
    public:
        ZoneRail();

        u32 _0;
        u32 _4;
    };

    class RailSet {
    public:

        RailSet &operator=(const RailSet &);

        u32 _0;
        u32 _4;
    };

    CameraRailHolder(const char *);
    ~CameraRailHolder();

    // getRider(long, long) const;
    // find(long, long) const;

    ZoneRail *mRails;   // _C
};