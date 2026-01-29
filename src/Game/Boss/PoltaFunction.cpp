#include "Game/Boss/PoltaFunction.hpp"
#include "Game/Boss/Polta.hpp"
#include "Game/Boss/PoltaArm.hpp"
#include "Game/Boss/PoltaSensorCtrl.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"

namespace PoltaFunction {
    //onMovement
    //emitEffectShadow

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
}; //namespace PoltaFunction
