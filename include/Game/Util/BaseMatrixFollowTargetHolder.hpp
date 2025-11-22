#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"
#include <revolution.h>

class JMapLinkInfo;

class BaseMatrixFollowValidater {
public:
    virtual bool isValid(s32) const = 0;
};

class BaseMatrixFollowTarget {
public:
    BaseMatrixFollowTarget(const JMapLinkInfo*);

    void set(LiveActor*, const TPos3f&, const TPos3f*, BaseMatrixFollowValidater*);
    const MtxPtr getHostBaseMtx() const;
    bool isValid(s32) const;

    TMtx34f _0;
    const TPos3f* _30;
    LiveActor* mActor;                      // 0x34
    const JMapLinkInfo* mLinkInfo;          // 0x38
    BaseMatrixFollowValidater* mValidater;  // 0x3C
};

class BaseMatrixFollower {
public:
    BaseMatrixFollower(NameObj*, const JMapInfoIter&);

    virtual void setGravityFollowHost(const NameObj*) {}

    virtual void update();

    NameObj* getFollowTargetActor() const;
    void calcFollowMatrix(TPos3f*) const;
    bool isEnableFollow() const;
    bool isValid() const;

    JMapLinkInfo* mLinkInfo;                // 0x4
    NameObj* mFollowerObj;                  // 0x8
    BaseMatrixFollowTarget* mFollowTarget;  // 0xC
    s32 mFollowID;                          // 0x10
};

class BaseMatrixFollowTargetHolder : public NameObj {
public:
    BaseMatrixFollowTargetHolder(const char*, s32, s32);

    virtual ~BaseMatrixFollowTargetHolder();

    virtual void initAfterPlacement();
    virtual void movement();

    MR::Vector< MR::AssignableArray< BaseMatrixFollower* > > mFollowers;    // 0xC
    MR::Vector< MR::AssignableArray< BaseMatrixFollowTarget* > > mTargets;  // 0x18
};

namespace MR {
    bool isValidFollowID(const JMapInfoIter&);
    void addBaseMatrixFollower(BaseMatrixFollower*);
    void addBaseMatrixFollowTarget(LiveActor*, const JMapInfoIter&, const TPos3f*, BaseMatrixFollowValidater*);
};  // namespace MR
