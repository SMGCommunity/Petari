#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class MirrorReflectionModel;
class ProjmapEffectMtxSetter;
class SpaceInner;

class Sky : public LiveActor {
public:
    Sky(const char*);

    /* 0x08 */ virtual ~Sky();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x1C */ virtual void calcAnim();
    /* 0x48 */ virtual void control();
    /* 0x78 */ virtual void initModel(const char*);

    void appearSpaceInner();
    void disappearSpaceInner();

    void exeWait();
    void exeChange();

    SpaceInner* mSpaceInner;                  // 0x8C
    MirrorReflectionModel* mReflectionModel;  // 0x90
};

class ProjectionMapSky : public Sky {
public:
    ProjectionMapSky(const char*);

    /* 0x08 */ virtual ~ProjectionMapSky();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x78 */ virtual void initModel(const char*);

    ProjmapEffectMtxSetter* mMtxSetter;  // 0x94
};
