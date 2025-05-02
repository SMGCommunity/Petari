#include "Game/Enemy/ElectricPressureBullet.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "revolution/types.h"
#include <cstddef>

ElectricPressureBullet::ElectricPressureBullet(const char *pName) : LiveActor(pName) {
    _8C.x = 0.0f;
    _8C.y = 0.0f;
    _8C.z = 0.0f;
    _98 = nullptr;
    _9C = 0.0f;
}

void ElectricPressureBullet::init(const JMapInfoIter &rIter) {
    initModelManagerWithAnm("ElectricBullet", nullptr, false);
    MR::connectToSceneEnemy(this);
    initHitSensor(1);
    TVec3f v5;
    v5.x = 0.0f;
    v5.y = 0.0f;
    v5.z = 0.0f;
    MR::addHitSensorEnemyAttack(this, "body", 8, 100.0f, v5);
    initBinder(100.0f, 0.0f, 0);
    initEffectKeeper(0, nullptr, false);
    initSound(1, false);
    MR::initStarPointerTarget(this, 100.0f, TVec3f(0, 0, 0));
    MR::initShadowVolumeCylinder(this, 75.0f);
    initNerve(&NrvElectricPressureBullet::ElectricPressureBulletNrvFly::sInstance);
    makeActorDead();
}

void ElectricPressureBullet::kill() {
    MR::emitEffect(this, "Break");
    MR::startSound(this, "SE_EM_ELECBUBLLET_BREAK", -1, -1);
    LiveActor::kill();
}

void ElectricPressureBullet::shotElectricBullet(LiveActor *actor, const TPos3f &pos, const f32 &value) {
    _98 = actor;
    _9C = value;
    _8C.set<f32>(pos.get(0, 2), pos.get(1, 2), pos.get(2, 2));
    mVelocity.scale(_9C, _8C);
    mPosition.set<f32>(pos.get(0, 3), pos.get(1, 3), pos.get(2, 3));
    mRotation.z = 0.0f;
    mRotation.y = 0.0f;
    mRotation.x = 0.0f;
    makeActorAppeared();
    MR::validateHitSensors(this);
    MR::invalidateClipping(this);
    setNerve(&NrvElectricPressureBullet::ElectricPressureBulletNrvFly::sInstance);
}

void ElectricPressureBullet::exeFly() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Shot", nullptr);
        MR::startBtk(this, "ElectricBullet");
    }

    if (MR::isBckOneTimeAndStopped(this)) {
        MR::startBck(this, "Move", nullptr);
    }
    MR::startLevelSound(this, "SE_EM_LV_ELECBUBLLET_FLY", -1, -1, -1);
    if (MR::isGreaterEqualStep(this, 10)) {
        if (MR::isLessStep(this, 150)) {
            s32 v2 = getNerveStep();   
            if (!(v2 % 5)) {
                TVec3f *v1 = MR::getPlayerCenterPos();
                TVec3f v3;
                JMathInlineVEC::PSVECSubtract2(v1, &mPosition, &v3);
                MR::normalize(&v3);
                MR::turnVecToVecDegree(&_8C, _8C, v3, 4.0f, TVec3f(0, 1, 0));
                mVelocity.scale(_9C, _8C);
            }
        }
    }     
    
    if (MR::isBinded(this)) {
        kill();
    }
    else if (MR::isStep(this, 360)) {
        kill();
    }
}

void ElectricPressureBullet::control() {
    if (MR::isStarPointerPointing2POnTriggerButton(this,"弱", true, false)) {
        kill();
    }
}

void ElectricPressureBullet::attackSensor(HitSensor *pSender,HitSensor *pReceiver) {
    if (MR::isSensorPlayer(pReceiver) && MR::sendMsgEnemyAttackElectric(pReceiver, pSender)) {
        kill();
    }
}

namespace NrvElectricPressureBullet {
    INIT_NERVE(ElectricPressureBulletNrvFly);

	void ElectricPressureBulletNrvFly::execute(Spine *pSpine) const {
		ElectricPressureBullet *pActor = (ElectricPressureBullet*)pSpine->mExecutor;
		pActor->exeFly();
	}    
};
