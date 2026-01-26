#include "Game\MapObj\PhantomTorch.hpp"
#include "Game/Effect/SpinPullParticleCallBack.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

namespace NrvPhantomTorch {
    NEW_NERVE(HostTypeWait, PhantomTorch, Wait);
    NEW_NERVE(HostTypeAttack, PhantomTorch, Attack);
    NEW_NERVE(HostTypeExtinguish, PhantomTorch, Extinguish);
};

PhantomTorch::~PhantomTorch() {}

PhantomTorch::PhantomTorch(const char* pName) : LiveActor(pName) {
    mParticle = nullptr;
    mItem = -1;
    mPullParticle = nullptr;
    mIsDecorative = false;
}

void PhantomTorch::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    f32 x = mScale.x;
    MR::getJMapInfoArg0NoInit(rIter, &mItem);
    s32 arg = -1;
    MR::getJMapInfoArg1NoInit(rIter, &arg);
    mIsDecorative = arg != -1;
    MR::getObjectName(&mParticle, rIter);

    if (mItem == -1) {
        MR::declareCoin(this, 1);
    }

    if (!mItem) {
        MR::declareStarPiece(this, 1);
    }

    initEffectKeeper(0, mParticle, false);
    mPullParticle = new SpinPullParticleCallBack();    
    initSound(4, false);
    initHitSensor(1);
    TVec3f vec;
    vec.x = 0.0f;
    vec.y = 70.0f * x;
    vec.z = 0.0f;
    MR::addHitSensorMapObj(this, "body", 8, 50.0f * x, vec);
    MR::setClippingTypeSphere(this, 500.0f * x);
    MR::joinToGroupArray(this, rIter, nullptr, 32);
    initNerve(&NrvPhantomTorch::HostTypeWait::sInstance);
    MR::connectToSceneMapObjMovement(this);
    makeActorAppeared();
    MR::useStageSwitchWriteDead(this, rIter);
    MR::useStageSwitchSleep(this, rIter);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::listenStageSwitchOnOffAppear(this, MR::Functor(this, &PhantomTorch::soundOrder), MR::Functor(this, &PhantomTorch::soundOrder));
        makeActorDead();
    }
}

void PhantomTorch::makeActorAppeared() {
    LiveActor::makeActorAppeared();
    if (!MR::isClipped(this)) {
        MR::emitEffect(this, mParticle);
    }
}

void PhantomTorch::makeActorDead() {
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }
    MR::deleteEffect(this, mParticle);
    LiveActor::makeActorDead();
}

void PhantomTorch::kill() {
    switch (mItem) {
        case -1:
            MR::appearCoinPop(this, mPosition, 1);
            break;     
        case 1:  
            break;
        case 2:        
            break;
        case 0:
            if (MR::appearStarPiece(this, mPosition, 1, 10.0f, 40.0f, false)) {
                MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
            }
            break;
    }        
    soundOrder();
    LiveActor::kill();
}

void PhantomTorch::startClipped() {
    LiveActor::startClipped();
    MR::deleteEffect(this, mParticle);
}

void PhantomTorch::endClipped() {
    MR::emitEffect(this, mParticle);
    LiveActor::endClipped();
}

void PhantomTorch::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!mIsDecorative && isNerve(&NrvPhantomTorch::HostTypeWait::sInstance) && MR::sendMsgEnemyAttackFire(pReceiver, pSender)) {
        setNerve(&NrvPhantomTorch::HostTypeAttack::sInstance);
    }
}

bool PhantomTorch::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (mIsDecorative) {
        return false;
    }
    
    if (MR::isInSpinStormRange(msg, pSender, pReceiver, 350.0f) && isNerve(&NrvPhantomTorch::HostTypeWait::sInstance)) {
        setNerve(&NrvPhantomTorch::HostTypeExtinguish::sInstance);
        return true;
    }    
    return false;
}

void PhantomTorch::soundOrder() {
    if (MR::getGroupFromArray(this) && MR::getGroupFromArray(this)->mObjectCount > 0) {
        s32 i;
        LiveActorGroup* group;
        s32 num = 0;
        group = MR::getGroupFromArray(this);

        for (i = 0; i < group->mObjectCount; i++) {
            if (!MR::isDead(group->getActor(i))) {
                num++;
            }
        }

        switch (num) {
            case 1:
                MR::startSystemSE("SE_SY_CANDLE_COMBO_5", -1, -1);            
                break;
            case 2:
                MR::startSystemSE("SE_SY_CANDLE_COMBO_4", -1, -1);            
                break;                
            case 3:
                MR::startSystemSE("SE_SY_CANDLE_COMBO_3", -1, -1);
                break;
            case 4:
                MR::startSystemSE("SE_SY_CANDLE_COMBO_2", -1, -1);                     
                break;
            case 5:    
            default:
                MR::startSystemSE("SE_SY_CANDLE_COMBO_1", -1, -1);
                break;
        }
    }
}

void PhantomTorch::exeWait() {
    if (MR::isEqualString(mParticle, "PhantomBonfire")) {
        MR::startLevelSound(this, "SE_OJ_LV_BONFIRE_BURN", -1, -1, -1);
    }
    else {
        MR::startLevelSound(this, "SE_OJ_LV_PHANTOM_TOACH_BURN", -1, -1, -1);
    }
}

void PhantomTorch::exeAttack() {
    if (MR::isStep(this, 30)) {
        setNerve(&NrvPhantomTorch::HostTypeWait::sInstance);
    }
    else if (MR::isEqualString(mParticle, "PhantomBonfire")) {
        MR::startLevelSound(this, "SE_OJ_LV_BONFIRE_BURN", -1, -1, -1);
    }
    else {
        MR::startLevelSound(this, "SE_OJ_LV_PHANTOM_TOACH_BURN", -1, -1, -1);
    }
}

void PhantomTorch::exeExtinguish() {
    if (MR::isFirstStep(this)) {
        MR::deleteEffect(this, mParticle);
        MR::emitEffectWithParticleCallBack(this, "Extinguish", mPullParticle);
        MR::startSound(this, "SE_OJ_PHANTOM_TOACH_OFF", -1, -1);
    }
    
    if (MR::isStep(this, 15)) {
        kill();
    }
}
