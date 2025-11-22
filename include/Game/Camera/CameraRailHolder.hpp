#pragma once

#include "Game/NameObj/NameObj.hpp"

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
        RailSet& operator=(const RailSet&);

        u32 _0;
        u32 _4;
    };

    CameraRailHolder(const char*);
    ~CameraRailHolder();

    // getRider(s32, s32) const;
    // find(s32, s32) const;

    ZoneRail* mRails;  // 0xC
};