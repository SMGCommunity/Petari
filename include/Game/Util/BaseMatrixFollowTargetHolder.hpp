#pragma once

#include "Game/NameObj/NameObj.h"
#include "Game/LiveActor/LiveActor.h"
#include "Game/Util.h"
#include "JSystem/JGeometry.h"

class BaseMatrixFollowValidater {
public:
    virtual bool isValid(s32) const = 0;
};

class BaseMatrixFollowTarget {
public:
    BaseMatrixFollowTarget(const JMapLinkInfo *);

    void set(LiveActor *, const TPos3f &, const TPos3f *, BaseMatrixFollowValidater *);
    const MtxPtr getHostBaseMtx() const;
    bool isValid(s32) const;

    TMtx34f _0;
    const TPos3f* _30;
    LiveActor* mActor;                      // _34
    const JMapLinkInfo* mLinkInfo;          // _38
    BaseMatrixFollowValidater* mValidater;  // _3C
};

class BaseMatrixFollower {
public:
    BaseMatrixFollower(NameObj *, const JMapInfoIter &);
    
    virtual void setGravityFollowHost(const NameObj *) {

    }

    virtual void update();

    NameObj* getFollowTargetActor() const;
    void calcFollowMatrix(TPos3f *) const;
    bool isEnableFollow() const;
    bool isValid() const;

    JMapLinkInfo* mLinkInfo;                // _4
    NameObj* mFollowerObj;                  // _8
    BaseMatrixFollowTarget* mFollowTarget;  // _C
    s32 mFollowID;                          // _10
};

class BaseMatrixFollowTargetHolder : public NameObj {
public:
    BaseMatrixFollowTargetHolder(const char *, s32, s32);

    virtual ~BaseMatrixFollowTargetHolder();

    virtual void initAfterPlacement();
    virtual void movement();

    MR::Vector<MR::AssignableArray<BaseMatrixFollower*> > mFollowers;       // _C
    MR::Vector<MR::AssignableArray<BaseMatrixFollowTarget*> > mTargets;     // _18
};

namespace MR {
    bool isValidFollowID(const JMapInfoIter &);
    void addBaseMatrixFollower(BaseMatrixFollower *);
    void addBaseMatrixFollowTarget(LiveActor *, const JMapInfoIter &, const TPos3f *, BaseMatrixFollowValidater *);
};
