#pragma once

#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/types.h>

class Polta;
class PoltaArm;
class HitSensor;

namespace PoltaFunction {
    void onMovement(Polta* pPolta);
    void emitEffectShadow(Polta* pPolta);
    PoltaArm* getLeftArmActor(Polta* pPolta);
    PoltaArm* getRightArmActor(Polta* pPolta);
    bool isCoreSensor(const Polta* pPolta, const HitSensor* pSensor);
    bool isBodySensor(const Polta* pPolta, const HitSensor* pSensor);
    bool isArmSensor(const Polta* pPolta, const HitSensor* pSensor);
    bool isLeftArmSensor(const Polta* pPolta, const HitSensor* pSensor);
    bool isRightArmSensor(const Polta* pPolta, const HitSensor* pSensor);
    bool isHeadSensor(const Polta* pPolta, const HitSensor* pSensor);
    void setScreamSensorSize(Polta* pPolta, f32 screamSensorSize);
    bool attackScreamSensor(Polta* pPolta, HitSensor* pSender, HitSensor* pReceiver);
    void startAction(Polta* pPolta, const char* pActionName, bool isAffectBody);
    void startBckBody(Polta* pPolta, const char* pActionName);
    void setBodyHP(Polta* pPolta, s32 hp);
    void damageLeftArm(Polta* pPolta);
    void damageRightArm(Polta* pPolta);
    void requestStartControllArm(Polta* pPolta);
    void startArm(Polta* pPolta);
    void breakLeftArm(Polta* pPolta);
    void breakRightArm(Polta* pPolta);
    void killLeftArm(Polta* pPolta);
    void killRightArm(Polta* pPolta);
    bool isBrokenLeftArm(Polta* pPolta);
    bool isBrokenRightArm(Polta* pPolta);
    bool isEnableAttackRightArm(Polta* pPolta);
    bool isEnableAttackLeftArm(Polta* pPolta);
    bool isMaxGenerateBombTeresa(Polta* pPolta);
    s32 getCountDeadGroundRock(Polta* pPolta);
    bool appearGroundRock(Polta* pPolta, f32 distance, f32 angle);
    bool appearRockCircle(Polta* pPolta, const TVec3f& rCenter, f32 radius, s32 index, s32 count, s32 rockType);
    bool appearWhiteRockCircle(Polta* pPolta, const TVec3f& rCenter, f32 radius, s32 index, s32 count);
    bool appearBlackRockCircle(Polta* pPolta, const TVec3f& rCenter, f32 radius, s32 index, s32 count);
    bool appearYellowRockCircle(Polta* pPolta, const TVec3f& rCenter, f32 radius, s32 index, s32 count);
    bool appearBombTeresaFromRoot(Polta* pPolta, f32 param2, f32 param3, s32 param4);
    bool appearBombTeresaNormal(Polta* pPolta, const TVec3f& rPosition, const TVec3f& rVelocity);
    void disperseBombTeresa(Polta* pPolta);
    void breakGroundRock(Polta* pPolta);
    void killBombTeresa(Polta* pPolta);
    void killPoltaRock(Polta* pPolta);
    void killGroundRock(Polta* pPolta);
}  // namespace PoltaFunction
