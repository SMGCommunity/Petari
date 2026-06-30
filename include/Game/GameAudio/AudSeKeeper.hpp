#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JKernel/JKRDisposer.hpp>

class AudSoundObject;
class J3DJoint;

class AudSeKeeperBox {
public:
    AudSeKeeperBox();

    ~AudSeKeeperBox();
    void update();
    void add(J3DJoint*, u16);
    void updateJointPos();

    /* 0x00 */ AudSoundObject* mSoundObj;
    /* 0x04 */ J3DJoint* mJoint;
    /* 0x08 */ TVec3f mPos;
    /* 0x14 */ u16 mHash;
};

class AudSeKeeper : public JKRDisposer {
public:
    AudSeKeeper(LiveActor*, int);

    ~AudSeKeeper();
    void update();
    void makeBox(const char*, const char*);
    bool playLevel(const char*, const char*);
    int getBoxEqualName(const char*);

    /* 0x18 */ LiveActor* mActor;
    /* 0x1C */ AudSeKeeperBox* mBoxes;
    /* 0x20 */ int mMaxBoxes;
    /* 0x24 */ int mNumBoxes;
};
