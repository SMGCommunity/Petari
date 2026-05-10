#include "Game/Boss/DinoPackunStateDamage.hpp"
#include "Game/Boss/DinoPackun.hpp"
#include "Game/Boss/DinoPackunEggShell.hpp"
#include "Game/Boss/DinoPackunTail.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Util.hpp"

namespace {
    static const TVec3f sOutPosition(0.0f, 100.0f, -300.0f);
    static const TVec3f sEggOutPosition(0.0f, 0.0f, -400.0f);
};  // namespace

namespace NrvDinoPackunStateDamage {
    NEW_NERVE(DinoPackunStateDamageNrvPunched, DinoPackunStateDamage, Punched);
    NEW_NERVE(DinoPackunStateDamageNrvTryPulledDemo, DinoPackunStateDamage, TryPulledDemo);
    NEW_NERVE(DinoPackunStateDamageNrvPulled, DinoPackunStateDamage, Pulled);
    NEW_NERVE(DinoPackunStateDamageNrvEggBroken, DinoPackunStateDamage, EggBroken);
    NEW_NERVE(DinoPackunStateDamageNrvDamage, DinoPackunStateDamage, Damage);
    NEW_NERVE(DinoPackunStateDamageNrvLastDamage, DinoPackunStateDamage, LastDamage);
};  // namespace NrvDinoPackunStateDamage

DinoPackunStateDamage::DinoPackunStateDamage(DinoPackun* pBoss) : ActorStateBase< DinoPackun >("ディノパックンダメージ状態", pBoss) {
    _10 = 0;
    _14 = 1;
    initNerve(&NrvDinoPackunStateDamage::DinoPackunStateDamageNrvPunched::sInstance);
}

void DinoPackunStateDamage::appear() {
    mIsDead = false;
    setNerve(&NrvDinoPackunStateDamage::DinoPackunStateDamageNrvPunched::sInstance);
}

bool DinoPackunStateDamage::isDamageMessage(u32 msg) const {
    return msg == 184;
}

bool DinoPackunStateDamage::receiveOtherMsg(u32 msg, HitSensor* a2, HitSensor* a3) {
    if (msg == 185 && isNerve(&NrvDinoPackunStateDamage::DinoPackunStateDamageNrvPunched::sInstance)) {
        setNerve(&NrvDinoPackunStateDamage::DinoPackunStateDamageNrvTryPulledDemo::sInstance);
        return true;
    } else if (msg == 186 && isNerve(&NrvDinoPackunStateDamage::DinoPackunStateDamageNrvPulled::sInstance)) {
        MR::emitEffectHitBetweenSensors(getHost(), a2, a3, 0.0f, "Hit");

        switch (_14) {
        case 0:
            setNerve(&NrvDinoPackunStateDamage::DinoPackunStateDamageNrvEggBroken::sInstance);
            break;
        case 1:
            setNerve(&NrvDinoPackunStateDamage::DinoPackunStateDamageNrvDamage::sInstance);
            break;
        case 2:
            setNerve(&NrvDinoPackunStateDamage::DinoPackunStateDamageNrvLastDamage::sInstance);
            break;
        }

        getHost()->offAimTailBall(-1);
        return true;
    }

    return false;
}

void DinoPackunStateDamage::setDamageEgg() {
    _14 = 0;
}

void DinoPackunStateDamage::setDamageNormal() {
    _14 = 1;
}

void DinoPackunStateDamage::setDamageLast() {
    _14 = 2;
}

void DinoPackunStateDamage::exePunched() {
    if (MR::isFirstStep(this)) {
        getHost()->onAimTailBall(-1);
        getHost()->startDamageCamera();
        MR::zeroVelocity(getHost());
        getHost()->mTail->lockNodePosition(1);
        MR::startSound(getHost(), "SE_BV_D_PAKKUN_SPIN_HIT", -1, -1);
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_SLAVER", -1, -1);
    }

    getHost()->adjustTailRootPosition(!_14 ? sOutPosition : sEggOutPosition, 0.1f);
    getHost()->updateNormalVelocity();
}

void DinoPackunStateDamage::exePulled() {
    TVec3f v21(0.0f, 0.0f, 0.0f);
    getHost()->mTail->getEndTailPosition(&v21);

    if (MR::isFirstStep(this)) {
        getHost()->startDemo();

        if (_14) {
            TVec3f v20;
            MR::calcSideVec(&v20, getHost());
            TVec3f v19;
            MR::calcUpVec(&v19, getHost());
            TVec3f stack_20;
            stack_20.set(MR::getSensorPos(getHost()->getSensor("head")));
            TVec3f stack_14(v21 - stack_20);
            MR::normalizeOrZero(&stack_14);

            if (v19.dot(stack_14) > 0.69999999f) {
                MR::startBck(getHost(), "PunchedTailTop", nullptr);
                _10 = 3;
            } else {
                if (v20.dot(stack_14) < 0.0f) {
                    MR::startBck(getHost(), "PunchedTailRight", nullptr);
                    _10 = 2;
                } else {
                    MR::startBck(getHost(), "PunchedTailLeft", nullptr);
                    _10 = 1;
                }
            }
        }
    }

    getHost()->adjustTailRootPosition(!_14 ? sOutPosition : sEggOutPosition, 0.1f);
    if (PSVECDistance(&v21, &getHost()->mPosition) > 800.0f) {
        TVec3f v15;
        getHost()->mTail->getTailNodePosition(&v15, 3);
        MR::turnDirectionFromTargetDegree(getHost(), &getHost()->_E8, v15, 6.0f);
    }

    getHost()->updateNormalVelocity();
}

void DinoPackunStateDamage::exeEggBroken() {
    if (MR::isFirstStep(this)) {
        MR::stopBck(getHost());
        getHost()->mTail->lockEndNodePosition();
        MR::startSystemSE("SE_SY_VS_BOSS_DAMAGE_1", -1, -1);
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_EGG_BRK_HIT", -1, -1);
        MR::startSound(getHost(), "SE_BV_D_PAKKUN_EGG_BREAK", -1, -1);
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_SLAVER", -1, -1);
        MR::shakeCameraStrong();
        MR::tryRumblePadMiddle(getHost(), 0);
        MR::zeroVelocity(getHost());
        MR::startBva(getHost()->getEggShell(), "EggBroken");
        MR::invalidateHitSensor(getHost(), "egg");
    }

    if (MR::isStep(this, 0)) {
        MR::stopScene(5);
    }

    if (MR::isStep(this, 30)) {
        getHost()->getEggShell()->kill();
        getHost()->getEggBrokenModel()->appear();
        MR::startBck(getHost()->getEggBrokenModel(), "Break", nullptr);
        MR::startBrk(getHost()->getEggBrokenModel(), "Break");
        MR::startBck(getHost(), "EggDamageTop", nullptr);
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_HEAD_EGG_BRK", -1, -1);
        getHost()->mTail->unlockEndNodePosition();
        getHost()->mTail->unlockNodePosition(1);
        MR::shakeCameraStrong();
        MR::tryRumblePadStrong(getHost(), 0);
    }

    if (MR::isGreaterStep(this, 120)) {
        getHost()->endDemo("ディノパックンダメージ");
        getHost()->getEggBrokenModel()->kill();
        kill();
        getHost()->endDamageCamera();
    }
}

void DinoPackunStateDamage::exeDamage() {
    if (MR::isFirstStep(this)) {
        MR::startSystemSE("SE_SY_VS_BOSS_DAMAGE_1", -1, -1);
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_DAMAGE", -1, -1);
        MR::startSound(getHost(), "SE_BV_D_PAKKUN_DAMAGE", -1, -1);
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_SLAVER", -1, -1);

        switch (_10) {
        case 3:
            MR::startBck(getHost(), "DamageTop", nullptr);
            break;
        case 2:
            MR::startBck(getHost(), "DamageRight", nullptr);
            break;
        case 1:
            MR::startBck(getHost(), "DamageLeft", nullptr);
            break;
        }
    }

    updateDamage();
}

void DinoPackunStateDamage::exeLastDamage() {
    if (MR::isFirstStep(this)) {
        MR::startSystemSE("SE_SY_VS_BOSS_LAST_HIT", -1, -1);
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_LAST_DAMAGE", -1, -1);
        MR::startSound(getHost(), "SE_BV_D_PAKKUN_LAST_DAMAGE", -1, -1);
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_SLAVER", -1, -1);

        switch (_10) {
        case 3:
            MR::startBck(getHost(), "LastDamageTop", nullptr);
            break;
        case 2:
            MR::startBck(getHost(), "LastDamageRight", nullptr);
            break;
        case 1:
            MR::startBck(getHost(), "LastDamageLeft", nullptr);
            break;
        }
    }

    updateDamage();
}

void DinoPackunStateDamage::updateDamage() {
    if (MR::isFirstStep(this)) {
        MR::tryRumblePadVeryStrong(getHost(), 0);
        MR::shakeCameraNormalStrong();
        MR::stopScene(15);
        MR::zeroVelocity(getHost());
        getHost()->mTail->unlockNodePosition(1);
    }

    if (MR::isBckStopped(getHost())) {
        getHost()->endDemo("ディノパックンダメージ");
        kill();
        getHost()->endDamageCamera();
    }
}

void DinoPackunStateDamage::exeTryPulledDemo() {
    if (MR::tryStartDemoMarioPuppetable(getHost(), "ディノパックンダメージ")) {
        setNerve(&NrvDinoPackunStateDamage::DinoPackunStateDamageNrvPulled::sInstance);
    }
}

DinoPackunStateDamage::~DinoPackunStateDamage() {
}
