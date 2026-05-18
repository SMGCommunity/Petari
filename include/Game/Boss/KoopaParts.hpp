#pragma once

#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

class Koopa;
class JMapInfoIter;
class KoopaFireStairs;
class KoopaBattleMapStair;
class LiveActorGroup;
class LiveActor;
class KoopaBattleMapPlanet;
class KoopaPlanetShadow;
class KoopaRockBreak;
class PartsModel;
class ModelObjNpc;
class ModelObj;
class ActorCameraInfo;

class KoopaParts {
public:
    KoopaParts(Koopa*, const JMapInfoIter&);

    KoopaFireStairs* emitFireStairsToPos(const KoopaBattleMapStair*, const TVec3f&, bool);
    void killFireStairsAll();
    void emitFireShort(bool, bool);
    void emitFireLongTime();
    void emitShockWave();
    TVec3f& getPlanetPos();
    f32 getPlanetRadius();
    void appearHoleSunPlanetInside();
    void appearHoleSunPlanetOutside();
    void killHoleSunPlanetOutside();
    void createPlanetShadow();
    void initVs1();
    void initVs2();
    void initVs3();
    void createRock();
    void createRollBall();
    void createCommonParts();
    void createFireStairs(bool);

private:
    /* 0x0 */ Koopa* mKoopa;
    /* 0x4 */ f32 mPlanetRadius;
    /* 0x8 */ LiveActor* _8;
    /* 0xC */ LiveActor* _C;
    /* 0x10 */ LiveActor* _10;
    /* 0x14 */ LiveActor* _14;
    /* 0x18 */ KoopaBattleMapPlanet* mBattleMapPlanet;
    /* 0x1C */ KoopaPlanetShadow* mPlanetShadow;
    /* 0x20 */ LiveActorGroup* _20;
    /* 0x24 */ LiveActorGroup* _24;
    /* 0x28 */ LiveActorGroup* _28;
    /* 0x2C */ u32 _2C;
    /* 0x30 */ u32 _30;
    /* 0x34 */ u32 _34;
    /* 0x38 */ LiveActor* _38;
    /* 0x3C */ KoopaRockBreak* _3C;
    /* 0x40 */ LiveActor* _40;
    /* 0x44 */ LiveActor* _44;
    /* 0x48 */ LiveActor* _48;
    /* 0x4C */ LiveActor* _4C;
    /* 0x50 */ PartsModel* _50;
    /* 0x54 */ LiveActor* _54;
    /* 0x58 */ PartsModel* _58;
    /* 0x5C */ ModelObjNpc* _5C;
    /* 0x60 */ ModelObjNpc* _60;
    /* 0x64 */ ModelObjNpc* _64;
    /* 0x68 */ ModelObj* _68;
    /* 0x6C */ ModelObj* _6C;
    /* 0x70 */ ModelObj* _70;
    /* 0x74*/ ActorCameraInfo* mActorCameraInfo;
};
