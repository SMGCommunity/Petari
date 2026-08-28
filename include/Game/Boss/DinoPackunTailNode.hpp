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

    /* 0x08 */ virtual ~DinoPackunTailNode();
    /* 0x78 */ virtual const TVec3f* getNodeDirection() const;
    /* 0x7C */ virtual void requestLockPosition();
    /* 0x80 */ virtual void requestUnLockPosition();
    /* 0x84 */ virtual void addNodeVelocity(const TVec3f&);
    /* 0x88 */ virtual JointController* createJointControllerOwn(LiveActor*, const char*) = 0;

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

    DinoPackun* mParent;    // 0x8C
    TVec3f mNodeDirection;  // 0x90
    TVec3f _9C;
    TVec3f _A8;
    LiveActor* _B4;
    LiveActor* _B8;
    MtxPtr _BC;
    JointControlDelegator< DinoPackunTailNode >* _C0;
    JointControlDelegator< DinoPackunTailNode >* _C4;
    f32 mLinkLength;     // 0xC8
    f32 mKeepBendPower;  // 0xCC
    u8 _D0;
};
