#pragma once

#include "Game/NameObj/NameObj.h"
#include "Game/Util.h"

class ClippingActorInfo;

class ClippingInfoGroup : public NameObj {
public:
    ClippingInfoGroup(const char *, int);

    virtual ~ClippingInfoGroup();

    void setGroupNo(const JMapInfoIter &);
    void registerInfo(ClippingActorInfo *);
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

    virtual void movement();

    s32 mNumGroups;                         // _C
    ClippingInfoGroup** mInfoGroups;         // _10
};