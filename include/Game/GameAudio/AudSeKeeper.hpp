#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JKernel/JKRDisposer.hpp"

class AudSoundObject;
class J3DJoint;

class AudSeKeeperBox {
public:
    AudSeKeeperBox();

    ~AudSeKeeperBox();
    void update();
    void add(J3DJoint*, u16);
    void updateJointPos();

    AudSoundObject* _0;  // 0x00
    J3DJoint* _4;        // 0x04
    TVec3f _8;           // 0x08
    u16 _14;             // 0x0C
};

class AudSeKeeper : public JKRDisposer {
public:
    AudSeKeeper(LiveActor*, int);

    ~AudSeKeeper();
    void update();
    void makeBox(const char*, const char*);
    void playLevel(const char*, const char*);
    AudSeKeeperBox* getBoxEqualName(const char*);

    // TODO
};