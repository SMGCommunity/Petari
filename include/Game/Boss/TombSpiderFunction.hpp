#pragma once

#include <JSystem/JGeometry/TMatrix.hpp>

class TombSpider;
class TombSpiderGland;
class TombSpiderVitalSpot;
class HitSensor;
class PlanetMap;
class ModelObj;

namespace TombSpiderFunction {
    TombSpiderGland* getGlandFrontL(const TombSpider*);
    TombSpiderGland* getGlandFrontR(const TombSpider*);
    TombSpiderGland* getGlandRearL(const TombSpider*);
    TombSpiderGland* getGlandRearR(const TombSpider*);

    TombSpiderVitalSpot* getVitalSpotC(const TombSpider*);
    TombSpiderVitalSpot* getVitalSpotL(const TombSpider*);
    TombSpiderVitalSpot* getVitalSpotR(const TombSpider*);

    MtxPtr getMtxEye(const TombSpider*);
    MtxPtr getMtxMouth(const TombSpider*);
    MtxPtr getMtxFrontL(const TombSpider*);
    MtxPtr getMtxFrontR(const TombSpider*);
    MtxPtr getMtxRearL(const TombSpider*);
    MtxPtr getMtxRearR(const TombSpider*);

    void appearThreadAttacherAll(TombSpider*);
    void killThreadAttacherAll(TombSpider*);

    void startAcid(TombSpider*);
    bool updateAcid(TombSpider*);
    void endAcid(TombSpider*);

    bool isAttackEnable(const TombSpiderGland*);
    bool isAttackDirection(HitSensor*, HitSensor*);

    bool tryDamageEye(HitSensor*, HitSensor*);
    bool tryDamageHip(HitSensor*, HitSensor*);
    bool tryDamageVitalSpot(HitSensor*, HitSensor*);

    void invalidateAllPartsSensor(TombSpider*);
    void invalidateMainPartsSensor(TombSpider*);
    void invalidateGlandSensor(TombSpider*);
    void invalidateVitalSpotSensor(TombSpider*);
    void invalidateAttackSensor(TombSpider*);

    bool isSpringAttacker(const HitSensor*);
    bool isEnemyAttack(const HitSensor*);
    bool isSpiderBody(const HitSensor*);
    bool isSpiderAttack(const HitSensor*);

    PlanetMap* getPlanet(const TombSpider*);
    ModelObj* getCocoon(const TombSpider*);

    void startTombSpiderDemo(TombSpider*, const char*, const char*);
    void startTombSpiderAnimCameraDemo(TombSpider*, const char*, const char*, s32);
    void endTombSpiderDemo(TombSpider*, const char*, const char*);
    void endTombSpiderAnimCameraDemo(TombSpider*, const char*, const char*);

    void pauseOffTombSpiderParts(const TombSpider*);
    void resetPlayerPosTombSpider(const TombSpider*, bool);
    bool isRotateLeftToPlayer(const TombSpider*);
    f32 calcRotateSpeedToPlayer(TombSpider*, s32);

    void emitAcidMouth(TombSpider*);
    void emitAcidGland(TombSpider*);

    bool tryDamageGland(TombSpider*, HitSensor*, HitSensor*);

    void validateMainPartsSensor(TombSpider*);
    void validateGlandSensor(TombSpider*);
    void validateVitalSpotSensor(TombSpider*);
    void validateAttackSensor(TombSpider*);
}  // namespace TombSpiderFunction
