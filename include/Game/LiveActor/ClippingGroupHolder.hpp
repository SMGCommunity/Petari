#pragma once

#include "Game/NameObj/NameObj.hpp"

class ClippingActorInfo;
class JMapIdInfo;

class ClippingInfoGroup : public NameObj {
public:
    ClippingInfoGroup(const char*, int);

    /* 0x08 */ virtual ~ClippingInfoGroup();

    void setGroupNo(const JMapInfoIter&);
    void registerInfo(ClippingActorInfo*);
    bool isClippedNowAll() const;
    void startClippedAll();
    void endClippedAll();

    int _C;
    s32 _10;
    ClippingActorInfo** _14;
    JMapIdInfo* _18;
    u8 _1C;
};

class ClippingGroupHolder : public NameObj {
public:
    ClippingGroupHolder();

    /* 0x08 */ virtual ~ClippingGroupHolder();
    /* 0x14 */ virtual void movement();

    void createAndAdd(ClippingActorInfo*, const JMapInfoIter&, int);
    ClippingInfoGroup* createGroup(ClippingActorInfo*, const JMapInfoIter&, int);

    ClippingInfoGroup* findGroup(const JMapInfoIter&);

    s32 mNumGroups;                   // 0xC
    ClippingInfoGroup** mInfoGroups;  // 0x10
};