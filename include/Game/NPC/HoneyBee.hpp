#pragma once

#include "Game/NPC/NPCActor.hpp"

class NameObjArchiveListCollector;

class HoneyBee : public NPCActor {
public:
    HoneyBee(const char*);

    virtual ~HoneyBee();
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void calcAndSetBaseMtx();

    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);
    void exeWait();
    void exeReaction();
    void exeFly();
    void exeDropLecture();
    void exeJumpLecture();
    void exeFlyLectureA();
    void exeFlyLectureB();

    s32 _15C;
    TMtx34f _160;
    TVec3f _190;
    s32 _19C;
    f32 _1A0;
};
