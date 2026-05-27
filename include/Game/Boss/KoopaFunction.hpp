#pragma once

#include "JSystem/JGeometry/TVec.hpp"
#include <revolution/types.h>

class NameObj;
class Koopa;
class LiveActor;
class KoopaBattleMapStair;
class PartsModel;
class KoopaRockBreak;
class KoopaBattleMapPlanet;
class HitSensor;
class KoopaPlanetShadow;
class KoopaSwitchKeeper;
class KoopaViewSwitchKeeper;
class KoopaPowerUpSwitch;
class ModelObjNpc;
class ModelObj;

namespace {
    bool tryStartCameraDemo(Koopa*, const char*, const char*, const char*, bool);
}  // namespace

namespace MR {
    struct ActorSightParam {
        /* 0x0 */ f32 _0;
        /* 0x4 */ f32 _4;
        /* 0x8 */ f32 _8;
    };

    struct ActorMoveParam;
}  // namespace MR

namespace KoopaFunction {
    NameObj* createKoopaVs1(const char*);
    NameObj* createKoopaVs2(const char*);
    NameObj* createKoopaVs3(const char*);
    void initKoopaPartsVs1(Koopa*);
    void initKoopaPartsVs2(Koopa*);
    void initKoopaVs3(Koopa*);

    void createKoopaRock(Koopa*);
    void createKoopaRollBall(Koopa*);

    s32 registerBattleMapStair(KoopaBattleMapStair*);

    void setKoopaPos(Koopa*, const char*);
    TVec3f& getPlanetCenterPos(const Koopa*);
    f32 getPlanetRadius(const Koopa*);

    TVec3f& getKoopaFront(Koopa*);
    TVec3f* getKoopaFrontPtr(Koopa*);

    bool isKoopaSightPlayer(const Koopa*, const MR::ActorSightParam&);
    void escapeKoopaFromPlayer(Koopa*, const MR::ActorMoveParam&);

    bool isKoopaVs1(const Koopa*);
    bool isKoopaVs2(const Koopa*);
    bool isKoopaVs3(const Koopa*);
    bool isKoopaLv1(const Koopa*);
    bool isKoopaLv2(const Koopa*);
    bool isKoopaLv3(const Koopa*);

    bool isKoopaAngry(const Koopa*);
    void startKoopaAngry(Koopa*);
    void startFaceCtrl(Koopa*);
    void endFaceCtrl(Koopa*, s32);
    void endFaceCtrlDirect(Koopa*);

    void startRecoverKoopaArmor(Koopa*);
    void startBreakKoopaTailThorn(Koopa*);
    void startRecoverKoopaTailThorn(Koopa*);

    PartsModel* getKoopaRock(Koopa*);
    KoopaRockBreak* getKoopaRockBreak(Koopa*);
    PartsModel* getKoopaRollBall(Koopa*);

    void emitKoopaFireShortSlow(Koopa*);
    void emitKoopaFireShortFast(Koopa*);
    void emitKoopaFireShortCurve(Koopa*);
    void emitKoopaFireLongTime(Koopa*);

    void emitFireStairsToTarget(Koopa*, const KoopaBattleMapStair*, const TVec3f&, bool);
    void killKoopaFireStairsAll(Koopa*);

    void emitKoopaShockWave(Koopa*);

    KoopaBattleMapPlanet* getKoopaPlanet(Koopa*);
    KoopaBattleMapPlanet* getKoopaPlanetLv2(Koopa*);
    KoopaBattleMapPlanet* getKoopaPlanetLv3(Koopa*);

    void appearKoopaHoleSunPlanetInside(Koopa*);
    void appearKoopaHoleSunPlanetOutside(Koopa*);
    void killKoopaHoleSunPlanetOutside(Koopa*);
    KoopaPlanetShadow* getKoopaPlanetShadow(Koopa*);

    bool tryKoopaPushPlayer(HitSensor*, HitSensor*);
    bool tryKoopaBodyAttackPlayer(HitSensor*, HitSensor*);
    bool tryKoopaAttackPlayerMaximum(HitSensor*, HitSensor*);
    bool tryKoopaBodyAttackPlayerMaximum(HitSensor*, HitSensor*);
    bool tryKoopaAttackMapObj(HitSensor*, HitSensor*);
    bool tryKoopaReflectStarPiece(u32, HitSensor*, HitSensor*);

    HitSensor* getKoopaMessageSensor(Koopa*);

    void registerKoopaSwitchKeeper(LiveActor*);
    KoopaSwitchKeeper* getKoopaSwitchKeeper(Koopa*);

    void registerKoopaViewSwitchKeeper(LiveActor*);
    KoopaViewSwitchKeeper* getKoopaViewSwitchKeeper(Koopa*);

    void registerKoopaPowerUpSwitch(LiveActor*);
    KoopaPowerUpSwitch* getKoopaPowerUpSwitch(Koopa*);

    bool tryStartKoopaAndMarioCameraDemo(Koopa*, const char*, const char*, const char*);
    void tryStartKoopaCameraDemo(Koopa*, const char*, const char*, const char*);
    bool tryEndKoopaCameraDemo(Koopa*, const char*, const char*);

    ModelObjNpc* getKoopaDemoPeach(Koopa*);
    ModelObjNpc* getKoopaDemoKoopaJr(Koopa*);
    ModelObjNpc* getKoopaDemoKoopaJrShip(Koopa*);
    ModelObj* getKoopaDemoMeteor1(Koopa*);
    ModelObj* getKoopaDemoMeteor2(Koopa*);
    ModelObj* getKoopaDemoMeteor3(Koopa*);
    void endKoopaDemoMeetPeach(Koopa*);

    void invalidateKoopaNpcLod(Koopa*);

    void initKoopaCamera(Koopa*, const char*);
    void initKoopaAnimCamera(Koopa*, const char*);
    void startKoopaCamera(Koopa*, const char*);
    void startKoopaTargetCamera(Koopa*, const char*);
    void startKoopaAnimCamera(Koopa*, const char*, s32);
    void endKoopaCamera(Koopa*, const char*, bool, s32);
    void endKoopaAnimCamera(Koopa*, const char*, s32);

    void startKoopaPlateDamageAnimPowerStarAppear(Koopa*);

    void changeBgmStateNormal(u32);
    void changeBgmStateEscape();
    void changeBgmStateDamageTail();
    void changeBgmStateFlyAway();

    void tryRestartKoopa();

    void moveAndTurnKoopaToPlayer(Koopa*, const MR::ActorMoveParam&);
    void startBreakKoopaArmor(Koopa*);
    bool tryKoopaShellAttackPlayer(HitSensor*, HitSensor*);
    void startKoopaSpinHitBlow(Koopa*);
};  // namespace KoopaFunction
