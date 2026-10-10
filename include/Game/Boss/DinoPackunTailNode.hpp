#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class DinoPackun;
template < typename T >
class JointControlDelegator;
class JointController;
class JointControllerInfo;

class DinoPackunTailNode : public LiveActor {
public:
    DinoPackunTailNode(const char*, DinoPackun*);

    virtual const TVec3f* getNodeDirection() const;
    virtual void requestLockPosition();
    virtual void requestUnLockPosition();
    virtual void addNodeVelocity(const TVec3f&);
    virtual JointController* createJointControllerOwn(LiveActor*, const char*) = 0;

    void createJointController(LiveActor*, const char*);
    void resetJoint();
    f32 getLinkLength() const;
    f32 getKeepBendPower() const;
    bool preCalcJoint(TPos3f*, const JointControllerInfo&);
    bool turnJointLocalXDir(TPos3f*, const JointControllerInfo&);
    bool calcJointScale(TPos3f*, const JointControllerInfo&);
    void registerPreCalcJointCallBack();
    void registerJointCallBack();
    void lockPosition();
    void unLockPosition();
    void addNodeVelocityHost(const TVec3f&);

    /* 0x8C */ DinoPackun* mParent;
    /* 0x90 */ TVec3f mNodeDirection;
    /* 0x9C */ TVec3f _9C;
    /* 0xA8 */ TVec3f _A8;
    /* 0xB4 */ LiveActor* _B4;
    /* 0xB8 */ LiveActor* _B8;
    /* 0xBC */ MtxPtr _BC;
    /* 0xC0 */ JointControlDelegator< DinoPackunTailNode >* _C0;
    /* 0xC4 */ JointControlDelegator< DinoPackunTailNode >* _C4;
    /* 0xC8 */ f32 mLinkLength;
    /* 0xCC */ f32 mKeepBendPower;
    /* 0xD0 */ u8 _D0;
};
