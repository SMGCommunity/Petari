#pragma once

#include "Game/NameObj/NameObj.hpp"

class RailRider;

class CameraRailHolder : public NameObj {
public:
    struct RailSet {
    public:
        /* 0x0 */ s32 mRailId;
        /* 0x4 */ RailRider* mRailRider;
    };

    class ZoneRail {
    public:
        ZoneRail() : mNumRails(), mRails() {
        }

        /* 0x0 */ u32 mNumRails;
        /* 0x4 */ RailSet* mRails;
    };

    CameraRailHolder(const char*);

    RailRider* getRider(s32 zoneId, s32 railId) const;
    s32 find(s32 zoneId, s32 railId) const;

    /* 0xC */ ZoneRail* mRails;
};
