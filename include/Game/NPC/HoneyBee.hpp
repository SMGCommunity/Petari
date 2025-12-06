#pragma once

#include "Game/NPC/NPCActor.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/NPCUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

class HoneyBee : public NPCActor {
public:
    HoneyBee(const char*);

    virtual ~HoneyBee();
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void calcAndSetBaseMtx();

    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);
    inline void exeWait();
    inline void exeReaction();
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
