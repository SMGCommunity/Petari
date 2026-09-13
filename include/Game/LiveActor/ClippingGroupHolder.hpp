#pragma once

#include "Game/NameObj/NameObj.hpp"

class ClippingActorInfo;
class JMapIdInfo;

class ClippingInfoGroup : public NameObj {
public:
    ClippingInfoGroup(const char*, int);

    virtual ~ClippingInfoGroup();

    void setGroupNo(const JMapInfoIter&);
    void registerInfo(ClippingActorInfo*);
    bool isClippedNowAll() const;
    void startClippedAll();
    void endClippedAll();

    /* 0x0C */ int _C;
    /* 0x10 */ s32 _10;
    /* 0x14 */ ClippingActorInfo** _14;
    /* 0x18 */ JMapIdInfo* _18;
    /* 0x1C */ u8 _1C;
};

class ClippingGroupHolder : public NameObj {
public:
    ClippingGroupHolder();

    virtual ~ClippingGroupHolder();
    virtual void movement();

    void createAndAdd(ClippingActorInfo*, const JMapInfoIter&, int);
    ClippingInfoGroup* createGroup(ClippingActorInfo*, const JMapInfoIter&, int);

    ClippingInfoGroup* findGroup(const JMapInfoIter&);
    ClippingInfoGroup* getGroup(int index) const {
        return mInfoGroups[index];
    }

    /* 0x0C */ s32 mNumGroups;
    /* 0x10 */ ClippingInfoGroup** mInfoGroups;
};
