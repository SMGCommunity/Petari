#pragma once

#include <JSystem/JGeometry/TMatrix.hpp>
#include <revolution/types.h>

class Koopa;
class JMapInfoIter;
class KoopaFireStairs;
class KoopaBattleMapStair;
class LiveActor;
class LiveActorGroup;
class KoopaBattleMapPlanet;
class KoopaPlanetShadow;
class KoopaSwitchKeeper;
class KoopaViewSwitchKeeper;
class KoopaPowerUpSwitch;
class KoopaRockBreak;
class PartsModel;
class ModelObjNpc;
class ModelObj;
class ActorCameraInfo;

class KoopaParts {
public:
    KoopaParts(Koopa* pKoopa, const JMapInfoIter& rIter);

    KoopaFireStairs* emitFireStairsToPos(const KoopaBattleMapStair* pMapStair, const TVec3f& rPosition, bool useFront);
    void killFireStairsAll();
    void emitFireShort(bool isFast, bool isCurve);
    void emitFireLongTime();
    void emitShockWave();
    TVec3f& getPlanetPos() const;
    f32 getPlanetRadius() const;
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

    /* 0x00 */ Koopa* mKoopa;
    /* 0x04 */ f32 mPlanetRadius;
    /* 0x08 */ LiveActor* mThornBig;
    /* 0x0C */ LiveActor* mThornSmall;
    /* 0x10 */ LiveActor* mArmorBreak;
    /* 0x14 */ LiveActor* mThornBreak;
    /* 0x18 */ KoopaBattleMapPlanet* mPlanetLv1;
    /* 0x1C */ KoopaPlanetShadow* mPlanetShadow;
    /* 0x20 */ LiveActorGroup* mShockWave;
    /* 0x24 */ LiveActorGroup* mFireShort;
    /* 0x28 */ LiveActorGroup* mFireStairs;
    /* 0x2C */ KoopaSwitchKeeper* mKoopaSwitchKeeper;
    /* 0x30 */ KoopaViewSwitchKeeper* mKoopaViewSwitchKeeper;
    /* 0x34 */ KoopaPowerUpSwitch* mKoopaPowerUpSwitch;
    /* 0x38 */ PartsModel* mRock;
    /* 0x3C */ KoopaRockBreak* mRockBreak;
    /* 0x40 */ PartsModel* mRollBall;
    /* 0x44 */ KoopaBattleMapPlanet* mPlanetLv2;
    /* 0x48 */ KoopaBattleMapPlanet* mPlanetLv3;
    /* 0x4C */ LiveActor* mHoleSunPlanetOutside;
    /* 0x50 */ PartsModel* mHoleSunPlanetOutsideBloom;
    /* 0x54 */ LiveActor* mHoleSunPlanetInside;
    /* 0x58 */ PartsModel* mHoleSunPlanetInsideBloom;
    /* 0x5C */ ModelObjNpc* mPeach;
    /* 0x60 */ ModelObjNpc* mKoopaJr;
    /* 0x64 */ ModelObjNpc* mKoopaJrShip;
    /* 0x68 */ ModelObj* mMeteor1;
    /* 0x6C */ ModelObj* mMeteor2;
    /* 0x70 */ ModelObj* mMeteor3;
    /* 0x74 */ ActorCameraInfo* mActorCameraInfo;
};
