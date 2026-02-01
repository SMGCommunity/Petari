#include "Game/Boss/PoltaFunction.hpp"
#include "Game/Boss/Polta.hpp"
#include "Game/Boss/PoltaArm.hpp"
#include "Game/Boss/PoltaRock.hpp"
#include "Game/Boss/PoltaSensorCtrl.hpp"
#include "Game/Enemy/BombTeresa.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Boss/PoltaRockHolder.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "revolution/types.h"
#include "Game/Boss/PoltaGroundRockHolder.hpp"
#include "Game/Boss/PoltaGroundRock.hpp"

namespace PoltaFunction {

    void onMovement(Polta* pPolta) {
        MR::forceDeleteEffectAll(pPolta);
        MR::requestMovementOn(pPolta);
        MR::requestMovementOn(pPolta->mBreakModel);
        MR::requestMovementOn(pPolta->mFormationModel);
        MR::requestMovementOn(pPolta->mLeftArm);
        MR::requestMovementOn(pPolta->mLeftArm->mBreakModel);
        MR::requestMovementOn(pPolta->mLeftArm->mFormationModel);
        MR::requestMovementOn(pPolta->mRightArm);
        MR::requestMovementOn(pPolta->mRightArm->mBreakModel);
        MR::requestMovementOn(pPolta->mRightArm->mFormationModel);
    }

    void emitEffectShadow(Polta* pPolta) {}

    PoltaArm* getLeftArmActor(Polta* pPolta) {
        return pPolta->mLeftArm;
    } 

    PoltaArm* getRightArmActor(Polta* pPolta) {
        return pPolta->mRightArm;
    } 

    bool isCoreSensor(const Polta* pPolta, const HitSensor* pSensor) {
        return pPolta->mSensorCtrl->isCoreSensor(pSensor);
    }

    bool isBodySensor(const Polta* pPolta, const HitSensor* pSensor) {
        return pPolta->mSensorCtrl->isBodySensor(pSensor);
    }

    bool isArmSensor(const Polta* pPolta, const HitSensor* pSensor) {
        return isLeftArmSensor(pPolta, pSensor) || isRightArmSensor(pPolta, pSensor);
    }

    bool isLeftArmSensor(const Polta* pPolta, const HitSensor* pSensor) {
        return pPolta->mSensorCtrl->isLeftArmSensor(pSensor);
    }

    bool isRightArmSensor(const Polta* pPolta, const HitSensor* pSensor) {
        return pPolta->mSensorCtrl->isRightArmSensor(pSensor);
    }

    bool isHeadSensor(const Polta* pPolta, const HitSensor* pSensor) {
        return pPolta->mSensorCtrl->isHeadSensor(pSensor);
    }

    void setScreamSensorSize(Polta* pPolta, f32 screamSensorSize) {
        pPolta->mSensorCtrl->setScreamSensorSize(screamSensorSize);
    }

    bool attackScreamSensor(Polta* pPolta, HitSensor* pSender, HitSensor* pReceiver) {
        if (pPolta->mSensorCtrl->isScreamSensor(pSender)) {
            if (!MR::isSensorPlayer(pReceiver)) {
                MR::sendMsgToEnemyAttackShockWave(pReceiver, pSender);
            }
            return true;
        }
        return false;
    }

    void startAction(Polta* pPolta, const char* pActionName, bool isAffectBody) {
        if (isAffectBody) {
            MR::startAction(pPolta, pActionName);
        }
        pPolta->mLeftArm->requestControlled(pActionName);
        pPolta->mRightArm->requestControlled(pActionName);
    }

    void startBckBody(Polta* pPolta, const char* pActionName) {
        MR::startAction(pPolta, pActionName);
    }

    void damageLeftArm(Polta* pPolta) {
        pPolta->mLeftArm->requestDamage();
    }

    void damageRightArm(Polta* pPolta) {
        pPolta->mRightArm->requestDamage();
    }

    void requestStartControllArm(Polta* pPolta) {
        pPolta->mLeftArm->requestStartControll();
        pPolta->mRightArm->requestStartControll();
    }

    void startArm(Polta* pPolta) {
        pPolta->mLeftArm->start();
        pPolta->mRightArm->start();
    }

    void breakLeftArm(Polta* pPolta) {
        pPolta->mLeftArm->requestBreakBody();
    }

    void breakRightArm(Polta* pPolta) {
        pPolta->mRightArm->requestBreakBody();
    }

    void killLeftArm(Polta* pPolta) {
        pPolta->mLeftArm->kill();
    }

    void killRightArm(Polta* pPolta) {
        pPolta->mRightArm->kill();
    }

    bool isBrokenLeftArm(Polta* pPolta) {
        return pPolta->mLeftArm->isBroken();
    }

    bool isBrokenRightArm(Polta* pPolta) {
        return pPolta->mRightArm->isBroken();
    }

    bool isEnableAttackLeftArm(Polta* pPolta) {
        return pPolta->mLeftArm->isEnableAttack();
    }

    bool isEnableAttackRightArm(Polta* pPolta) {
        return pPolta->mRightArm->isEnableAttack();
    }   

    bool isMaxGenerateBombTeresa(Polta* pPolta) {
        return pPolta->mBombTeresaHolder->getDeadActor() == nullptr;
    }

    s32 getCountDeadGroundRock(Polta* pPolta) {
        return pPolta->mGroundRockHolder->getObjectCount() - pPolta->mGroundRockHolder->getLivingActorNum();
    }

    bool appearGroundRock(Polta* pPolta, float param2, float param3) {
        PoltaGroundRock* deadMember = (PoltaGroundRock*)pPolta->mGroundRockHolder->getDeadActor();
        if (deadMember) {
            deadMember = (PoltaGroundRock*)pPolta->mGroundRockHolder->getDeadActor();
        } else {
            return false;
        }
        if (deadMember) {
            TVec3f v28(*MR::getPlayerPos());
            JMathInlineVEC::PSVECSubtract(v28, pPolta->mPosition, v28);
            JMAVECScaleAdd(pPolta->mGravity, v28, v28, -pPolta->mGravity.dot(v28));
            if (MR::normalizeOrZero(&v28)) {
                v28.setPS(pPolta->_C4);
            }
            MR::rotateVecDegree(&v28, pPolta->mGravity, param2);
            JMAVECScaleAdd(v28, pPolta->mPosition, v28, param3);
            deadMember->start(pPolta, v28);
            return true;
        }
    }

    //bool appearRockCircle(Polta* pPolta, const TVec3f& rVec, f32 param3, s32 param4, s32 param5, s32 rockType)

    bool appearWhiteRockCircle(Polta* pPolta, const TVec3f& rVec, f32 param3, s32 param4, s32 param5) {
        appearRockCircle(pPolta, rVec, param3, param4, param5, 0);
    }

    bool appearBlackRockCircle(Polta* pPolta, const TVec3f& rVec, f32 param3, s32 param4, s32 param5) {
        appearRockCircle(pPolta, rVec, param3, param4, param5, 1);
    }

    bool appearYellowRockCircle(Polta* pPolta, const TVec3f& rVec, f32 param3, s32 param4, s32 param5) {
        appearRockCircle(pPolta, rVec, param3, param4, param5, 2);
    }

    //All the params besides pPolta go unused.
    bool appearBombTeresaFromRoot(Polta* pPolta, float param2, float param3, s32 param4) {
        TVec3f v8;
        JMAVECScaleAdd(pPolta->mGravity, pPolta->mPosition, &v8, -120.0f);
        BombTeresa* deadMember = pPolta->mBombTeresaHolder->getDeadMember();
        if (!deadMember) {
            return false;
        }
        deadMember->appearShadow(v8, TVec3f(0.0f,0.0f,0.0f));
        return true;
    }

    bool appearBombTeresaNormal(Polta*pPolta, const TVec3f& rPosition, const TVec3f& rVelocity) {
        BombTeresa* deadMember = pPolta->mBombTeresaHolder->getDeadMember();
        if (!deadMember) {
            return false;
        }
        deadMember->appearNormal(rPosition, rVelocity);
        return true;
    }

    void disperseBombTeresa(Polta* pPolta) {
        pPolta->mBombTeresaHolder->disperseAll();
    }
    
    void breakGroundRock(Polta* pPolta) {
        pPolta->mGroundRockHolder->breakAll();
    }

    void killBombTeresa(Polta* pPolta) {
        pPolta->mBombTeresaHolder->killAll();
    }
    
    void killPoltaRock(Polta* pPolta) {
        pPolta->mRockHolder->killAll();
    }

    void killGroundRock(Polta* pPolta) {
        pPolta->mGroundRockHolder->killAll();
    }

    void setBodyHP(Polta* pPolta, s32 param2) {
        MR::startBva(pPolta, "BreakLevel");
        MR::setBvaFrameAndStop(pPolta, (3.0f - param2));

        switch (param2) {
            case 3:
                MR::deleteEffect(pPolta, "DamageSmoke1");
                MR::deleteEffect(pPolta, "DamageSmoke2");
                break;
            case 2:
                MR::emitEffect(pPolta, "DamageSmoke1");
                MR::deleteEffect(pPolta, "DamageSmoke2");
                break;
            case 1:
                MR::emitEffect(pPolta, "DamageSmoke1");
                MR::emitEffect(pPolta, "DamageSmoke2");
                break;
            case 0:
            default:
                MR::deleteEffect(pPolta, "DamageSmoke1");
                MR::deleteEffect(pPolta, "DamageSmoke2");
                break;    
        }
    }
    
}; //namespace PoltaFunction
