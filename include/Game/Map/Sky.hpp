#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/MaterialCtrl.hpp"

class MirrorReflectionModel;
class SpaceInner;

class Sky : public LiveActor {
public:
    Sky(const char *);

    virtual ~Sky();
    virtual void init(const JMapInfoIter &);
    virtual void calcAnim();
    virtual void control();
    virtual void initModel(const char *);

    void disappearSpaceInner();
    void appearSpaceInner();

    SpaceInner* mSpaceInner;                    // 0x8C
    MirrorReflectionModel* mReflectionModel;    // 0x90
};

class ProjectionMapSky : public Sky {
public:
    ProjectionMapSky(const char *);

    virtual ~ProjectionMapSky();
    virtual void calcAndSetBaseMtx();
    virtual void initModel(const char *);

    ProjmapEffectMtxSetter* mMtxSetter;     // 0x94
};

namespace NrvSky {
    const char* cChangeAnimName = "Change";

    NERVE(HostTypeWait);
    NERVE(HostTypeChange);
};