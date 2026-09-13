#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/Array.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class JMapLinkInfo;
class LiveActor;

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

    /* 0x00 */ TMtx34f _0;
    /* 0x30 */ const TPos3f* _30;
    /* 0x34 */ LiveActor* mActor;
    /* 0x38 */ const JMapLinkInfo* mLinkInfo;
    /* 0x3C */ BaseMatrixFollowValidater* mValidater;
};

class BaseMatrixFollower;

class BaseMatrixFollowTargetHolder : public NameObj {
public:
    BaseMatrixFollowTargetHolder(const char*, s32, s32);

    virtual ~BaseMatrixFollowTargetHolder();

    virtual void initAfterPlacement();
    virtual void movement();

    void addFollower(BaseMatrixFollower* pFollower);
    void setFollowTargetInfo(LiveActor* pActor, const JMapInfoIter& rIter, const TPos3f* pBaseMtx, BaseMatrixFollowValidater* pValidater) NO_INLINE;
    BaseMatrixFollowTarget* findFollowTarget(const JMapLinkInfo* pLinkInfo);
    BaseMatrixFollowTarget* findFollowTarget(const BaseMatrixFollower* pFollower);

    /* 0x0C */ MR::Vector< MR::AssignableArray< BaseMatrixFollowTarget* > > mTargets;
    /* 0x18 */ MR::Vector< MR::AssignableArray< BaseMatrixFollower* > > mFollowers;
};

class BaseMatrixFollower {
public:
    BaseMatrixFollower(NameObj*, const JMapInfoIter&);

    virtual void setGravityFollowHost(const NameObj*) {
    }

    virtual void update();

    NameObj* getFollowTargetActor() const;
    void calcFollowMatrix(TPos3f*) const;
    bool isEnableFollow() const;
    bool isValid() const;

    /* 0x04 */ JMapLinkInfo* mLinkInfo;
    /* 0x08 */ NameObj* mFollowerObj;
    /* 0x0C */ BaseMatrixFollowTarget* mFollowTarget;
    /* 0x10 */ s32 mFollowID;
};

namespace MR {
    bool isValidFollowID(const JMapInfoIter&);
    void addBaseMatrixFollower(BaseMatrixFollower*);
    void addBaseMatrixFollowTarget(LiveActor*, const JMapInfoIter&, const TPos3f*, BaseMatrixFollowValidater*);
};  // namespace MR
