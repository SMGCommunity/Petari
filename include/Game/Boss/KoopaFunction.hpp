#pragma once

#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/types.h>

class HitSensor;
class Koopa;
class KoopaBattleMapPlanet;
class KoopaBattleMapStair;
class KoopaFireStairs;
class KoopaPlanetShadow;
class KoopaPowerUpSwitch;
class KoopaRockBreak;
class KoopaSwitchKeeper;
class KoopaViewSwitchKeeper;
class LiveActor;
class ModelObj;
class ModelObjNpc;
class NameObj;
class PartsModel;

namespace {
    bool tryStartCameraDemo(Koopa*, const char*, const char* pName, const char* pDemoName, bool isMario);
};  // namespace

namespace MR {
    struct ActorSightParam {
        /* 0x0 */ f32 _0;
        /* 0x4 */ f32 _4;
        /* 0x8 */ f32 _8;
    };

    struct ActorMoveParam;
};  // namespace MR

namespace KoopaFunction {
    NameObj* createKoopaVs1(const char* pName);
    NameObj* createKoopaVs2(const char* pName);
    NameObj* createKoopaVs3(const char* pName);
    void initKoopaPartsVs1(Koopa* pKoopa);
    void initKoopaPartsVs2(Koopa* pKoopa);
    void initKoopaVs3(Koopa* pKoopa);
    void createKoopaRock(Koopa* pKoopa);
    void createKoopaRollBall(Koopa* pKoopa);
    s32 registerBattleMapStair(KoopaBattleMapStair* pBattleMapStair);
    void setKoopaPos(Koopa* pKoopa, const char* pDemoName);
    TVec3f& getPlanetCenterPos(const Koopa* pKoopa);
    f32 getPlanetRadius(const Koopa* pKoopa);
    const TVec3f& getKoopaFront(const Koopa* pKoopa);
    TVec3f* getKoopaFrontPtr(Koopa* pKoopa);
    bool isKoopaSightPlayer(const Koopa* pKoopa, const MR::ActorSightParam& rSightParam);
    bool escapeKoopaFromPlayer(Koopa* pKoopa, const MR::ActorMoveParam& rMoveParam);
    bool isKoopaVs1(const Koopa* pKoopa);
    bool isKoopaVs2(const Koopa* pKoopa);
    bool isKoopaVs3(const Koopa* pKoopa) NO_INLINE;  // otherwise inlines in startKoopaPlateDamageAnimPowerStarAppear
    bool isKoopaLv1(const Koopa* pKoopa);
    bool isKoopaLv2(const Koopa* pKoopa);
    bool isKoopaLv3(const Koopa* pKoopa);
    bool isKoopaAngry(const Koopa* pKoopa);
    void startKoopaAngry(Koopa* pKoopa);
    void startFaceCtrl(Koopa* pKoopa);
    void endFaceCtrl(Koopa* pKoopa, s32 val);
    void endFaceCtrlDirect(Koopa* pKoopa);
    void startRecoverKoopaArmor(Koopa* pKoopa);
    void startBreakKoopaTailThorn(Koopa* pKoopa);
    void startRecoverKoopaTailThorn(Koopa* pKoopa);
    PartsModel* getKoopaRock(Koopa* pKoopa);
    KoopaRockBreak* getKoopaRockBreak(Koopa* pKoopa);
    PartsModel* getKoopaRollBall(Koopa* pKoopa);
    void emitKoopaFireShortSlow(Koopa* pKoopa);
    void emitKoopaFireShortFast(Koopa* pKoopa);
    void emitKoopaFireShortCurve(Koopa* pKoopa);
    void emitKoopaFireLongTime(Koopa* pKoopa);
    KoopaFireStairs* emitFireStairsToTarget(Koopa* pKoopa, const KoopaBattleMapStair* pStair, const TVec3f& rPosition, bool useFront);
    void killKoopaFireStairsAll(Koopa* pKoopa);
    void emitKoopaShockWave(Koopa* pKoopa);
    KoopaBattleMapPlanet* getKoopaPlanet(Koopa* pKoopa);
    KoopaBattleMapPlanet* getKoopaPlanetLv2(Koopa* pKoopa);
    KoopaBattleMapPlanet* getKoopaPlanetLv3(Koopa* pKoopa);
    void appearKoopaHoleSunPlanetInside(Koopa* pKoopa);
    void appearKoopaHoleSunPlanetOutside(Koopa* pKoopa);
    void killKoopaHoleSunPlanetOutside(Koopa* pKoopa);
    KoopaPlanetShadow* getKoopaPlanetShadow(Koopa* pKoopa);
    bool tryKoopaPushPlayer(HitSensor* pSender, HitSensor* pReceiver);
    bool tryKoopaBodyAttackPlayer(HitSensor* pSender, HitSensor* pReceiver);
    bool tryKoopaBodyAttackPlayerMaximum(HitSensor* pSender, HitSensor* pReceiver);
    bool tryKoopaAttackPlayerMaximum(HitSensor* pSender, HitSensor* pReceiver);
    bool tryKoopaAttackMapObj(HitSensor* pSender, HitSensor* pReceiver);
    bool tryKoopaReflectStarPiece(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    HitSensor* getKoopaMessageSensor(Koopa* pKoopa);
    void registerKoopaSwitchKeeper(LiveActor* pActor);
    KoopaSwitchKeeper* getKoopaSwitchKeeper(Koopa* pKoopa);
    void registerKoopaViewSwitchKeeper(LiveActor* pActor);
    KoopaViewSwitchKeeper* getKoopaViewSwitchKeeper(Koopa* pKoopa);
    void registerKoopaPowerUpSwitch(LiveActor* pActor);
    KoopaPowerUpSwitch* getKoopaPowerUpSwitch(Koopa* pKoopa);
    bool tryStartKoopaAndMarioCameraDemo(Koopa* pKoopa, const char*, const char*, const char*);
    bool tryStartKoopaCameraDemo(Koopa* pKoopa, const char*, const char*, const char*);
    bool tryEndKoopaCameraDemo(Koopa* pKoopa, const char*, const char*);
    ModelObjNpc* getKoopaDemoPeach(Koopa* pKoopa);
    ModelObjNpc* getKoopaDemoKoopaJr(Koopa* pKoopa);
    ModelObjNpc* getKoopaDemoKoopaJrShip(Koopa* pKoopa);
    ModelObj* getKoopaDemoMeteor1(Koopa* pKoopa);
    ModelObj* getKoopaDemoMeteor2(Koopa* pKoopa);
    ModelObj* getKoopaDemoMeteor3(Koopa* pKoopa);
    void endKoopaDemoMeetPeach(Koopa* pKoopa);
    void invalidateKoopaNpcLod(Koopa* pKoopa);
    void initKoopaCamera(Koopa* pKoopa, const char* pName);
    void initKoopaAnimCamera(Koopa* pKoopa, const char* pName);
    void startKoopaCamera(Koopa* pKoopa, const char* pName);
    void startKoopaTargetCamera(Koopa* pKoopa, const char* pName);
    void startKoopaAnimCamera(Koopa* pKoopa, const char* pName, s32);
    void endKoopaCamera(Koopa* pKoopa, const char* pName, bool, s32);
    void endKoopaAnimCamera(Koopa* pKoopa, const char* pName, s32);
    void startKoopaPlateDamageAnimPowerStarAppear(Koopa* pKoopa);
    void changeBgmStateNormal(u32);
    void changeBgmStateEscape();
    void changeBgmStateDamageTail();
    void changeBgmStateFlyAway();
    void tryRestartKoopa();
    bool moveAndTurnKoopaToPlayer(Koopa* pKoopa, const MR::ActorMoveParam& rMoveParam);
    void startBreakKoopaArmor(Koopa* pKoopa);
    bool tryKoopaShellAttackPlayer(HitSensor* pSender, HitSensor* pReceiver);
    void startKoopaSpinHitBlow(Koopa* pKoopa);
};  // namespace KoopaFunction
