#include "Game/Boss/PoltaArm.hpp"
#include "Game/Boss/Polta.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "revolution/types.h"

namespace NrvPoltaArm {
    NEW_NERVE(PoltaArmNrvControlled, PoltaArm, Controlled);
    NEW_NERVE(PoltaArmNrvDamage, PoltaArm, Damage);
    NEW_NERVE(PoltaArmNrvBreak, PoltaArm, Break);
    NEW_NERVE(PoltaArmNrvBreakBody, PoltaArm, BreakBody);
    NEW_NERVE(PoltaArmNrvBroken, PoltaArm, Broken);
    NEW_NERVE_ONEND(PoltaArmNrvRepair, PoltaArm, Repair, Repair);
    NEW_NERVE(PoltaArmNrvWaitDamageEnd, PoltaArm, WaitDamageEnd);
    NEW_NERVE(PoltaArmNrvWaitRepairEnd, PoltaArm, WaitRepairEnd);

} //namespace NrvPoltaArm

PoltaArm::PoltaArm(const char* pName, const char* pModelName, MtxPtr pMtx) : ModelObj(pName, pModelName, pMtx, 18, -2, -2, false),
    mFormationModel(nullptr),
    mBreakModel(nullptr),
    _C8(2),
    _CC(2),
    mBrokenCounter(0),
    _D4(0),
    mIsLeftArm(true) 
{
    _98.identity();
    MR::initLightCtrl(this);
    
    if (MR::isEqualString(pModelName, "PoltaLeftArm")) {
        mIsLeftArm = true;
    } else if (MR::isEqualString(pModelName, "PoltaRightArm")) {
        mIsLeftArm = false;
    }
}

void PoltaArm::init(const JMapInfoIter& rIter) {
    ModelObj::init(rIter);
    MR::startBtk(this, "Inner");
    MR::initShadowFromCSV(this, "Shadow");
    MR::invalidateClipping(this);
    MR::startBva(this, "BreakLevel");
    MR::setBvaFrameAndStop(this, 0.0);
    initNerve(&NrvPoltaArm::PoltaArmNrvControlled::sInstance);
    initPartsModel();
    makeActorDead();
}

void PoltaArm::initPartsModel() {
    mBreakModel = MR::createModelObjEnemy("壊れモデル", "PoltaArmBreak", _98);
    mBreakModel->initWithoutIter();
    MR::initLightCtrl(mBreakModel);
    MR::invalidateClipping(mBreakModel);
    mBreakModel->makeActorDead();
    mFormationModel = MR::createModelObjEnemy("合体演出岩モデル", "PoltaFormationRock", getBaseMtx());
    mFormationModel->initWithoutIter();
    MR::initLightCtrl(mFormationModel);
    MR::invalidateClipping(mFormationModel);
    mFormationModel->makeActorDead();
}

void PoltaArm::appear() {
    LiveActor::appear();
}

void PoltaArm::makeActorDead() {
    LiveActor::makeActorDead();
    mBreakModel->kill();
}

void PoltaArm::control() {
    if (!MR::isDead(mBreakModel) && MR::isActionEnd(mBreakModel)) {
        mBreakModel->kill();
    }
}

void PoltaArm::exeDamage() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Damage");
        MR::startBva(this, "BreakLevel");
        f32 c8 = _C8;
        f32 cc = _CC;
        f32 ratio = c8 / cc;
        f32 bvaFrame = (1.0f - ratio) * 2.0f;
        MR::setBvaFrameAndStop(this, bvaFrame);
        MR::startSound(this, "SE_BM_POLTA_ROCK_DAMAGE", -1, -1);
        MR::tryRumblePadStrong(this, 0);
        MR::shakeCameraNormalStrong();
        MR::stopScene(5);
        appearBreakModel();
    }

    if (MR::isActionEnd(this)) {
        MR::startAction(this, "DamageWait");
        setNerve( &NrvPoltaArm::PoltaArmNrvWaitDamageEnd::sInstance);
    }
}

void PoltaArm::exeBreak() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Break");
        MR::startSound(this, "SE_BM_POLTA_ROCK_BREAK", -1, -1);
        MR::tryRumblePadStrong(this, 0);
        MR::shakeCameraNormalStrong();
        MR::stopScene(5);
        appearBreakModel();
    }
    if (MR::isActionEnd(this)) {
        MR::hideModel(this);
        setNerve(&NrvPoltaArm::PoltaArmNrvBroken::sInstance);
    }
}

void PoltaArm::exeBreakBody() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "BreakBody");
        appearBreakModel();
    }
    if (MR::isActionEnd(this)) {
        kill();
    }
}

void PoltaArm::exeRepair() {
    if (MR::isFirstStep(this)) {
        MR::invalidateShadowAll(this);
        MR::showModel(this);
        MR::startAction(this, "Repair");
        mFormationModel->appear();
        MR::startAction(mFormationModel, mIsLeftArm ? "RepairArmL" : "RepairArmR");
        _C8 = _CC;
    }

    if (MR::isActionEnd(this)) {
        MR::validateShadowAll(this);
        MR::startAction(this, "RepairWait");
        MR::startBva(this, "BreakLevel");
        MR::setBvaFrameAndStop(this, 0.0f);
        setNerve(&NrvPoltaArm::PoltaArmNrvWaitRepairEnd::sInstance);
    }
}

void PoltaArm::exeBroken() {
    if (!_D4) {
        mBrokenCounter++;
    }
    if (mBrokenCounter > 1200) {
        setNerve(&NrvPoltaArm::PoltaArmNrvRepair::sInstance);
    }
}

bool PoltaArm::isEnableHitSensor() const {
    if (MR::isDead(this)) {
        return false;
    }

    if (isNerve(&NrvPoltaArm::PoltaArmNrvControlled::sInstance) || isNerve(&NrvPoltaArm::PoltaArmNrvWaitDamageEnd::sInstance) ||
        isNerve(&NrvPoltaArm::PoltaArmNrvWaitRepairEnd::sInstance)) {
        return true;
    }
    return false;
}

bool PoltaArm::isEnableAttack() const {
    if (MR::isDead(this)) {
        return false;
    }

    return isNerve(&NrvPoltaArm::PoltaArmNrvControlled::sInstance);
}

bool PoltaArm::isBroken() const {
    if (MR::isDead(this)) {
        return true;
    }

    if (isNerve(&NrvPoltaArm::PoltaArmNrvBreak::sInstance) || isNerve(&NrvPoltaArm::PoltaArmNrvBreakBody::sInstance) ||
        isNerve(&NrvPoltaArm::PoltaArmNrvBroken::sInstance)) {
        return true;
    }
    return false;
}

void PoltaArm::start() {
    if (MR::isDead(this)) {
        appear();
    }

    MR::showModel(this);
    MR::startAction(this, "Wait");
    MR::startBva(this, "BreakLevel");
    MR::setBvaFrameAndStop(this, 0.0f);

    s32 CC_temp = _CC; //required for matching, maybe they were planning to do something?
    mBrokenCounter = 0;
    _C8 = CC_temp;
    setNerve(&NrvPoltaArm::PoltaArmNrvControlled::sInstance);
}

bool PoltaArm::requestBreakBody() {
    if (MR::isDead(this)) {
        return false;
    }    

    if (isNerve(&NrvPoltaArm::PoltaArmNrvBroken::sInstance)) {
        kill();
        return true;
    }

    setNerve(&NrvPoltaArm::PoltaArmNrvBreakBody::sInstance);
    return true;
}

bool PoltaArm::requestDamage() {

    if (MR::isDead(this)) {
        return false;
    }
    if (isNerve(&NrvPoltaArm::PoltaArmNrvControlled::sInstance) || isNerve(&NrvPoltaArm::PoltaArmNrvWaitDamageEnd::sInstance) ||
        isNerve(&NrvPoltaArm::PoltaArmNrvWaitRepairEnd::sInstance)) {
        _C8--;
        if (_C8 <= 0) {
            _C8 = 0;
            mBrokenCounter = 0;
            setNerve(&NrvPoltaArm::PoltaArmNrvBreak::sInstance);
        } else {
            setNerve(&NrvPoltaArm::PoltaArmNrvDamage::sInstance);
        }
        return true;
    }
    return false;
}

bool PoltaArm::requestStartControll() {
    if (MR::isDead(this)) {
        return false;
    }

    if (isNerve(&NrvPoltaArm::PoltaArmNrvWaitRepairEnd::sInstance) || isNerve(&NrvPoltaArm::PoltaArmNrvWaitDamageEnd::sInstance)) {
        setNerve(&NrvPoltaArm::PoltaArmNrvControlled::sInstance);
        return true;
    }

    return false;
}

bool PoltaArm::requestControlled(const char* pActionName) {
    if (MR::isDead(this)) {
        return false;
    }
    if (!isNerve(&NrvPoltaArm::PoltaArmNrvControlled::sInstance)) {
        return false;
    }

    MR::startAction(this, pActionName);
    return true;
}

void PoltaArm::appearBreakModel() {
    TVec3f jointPos;
    MR::copyJointPos(this, mIsLeftArm ? "HandL" : "HandR", &jointPos);
    TVec3f negativeGravity(mGravity.negateInline());
    MR::makeMtxUpNoSupportPos(&_98, negativeGravity, jointPos);
    mBreakModel->mScale.set(1.0f, 1.0f, 1.0f);
    mBreakModel->makeActorAppeared();
    MR::invalidateClipping(mBreakModel);
    MR::startAction(mBreakModel, "Break");
}

inline void PoltaArm::exeWaitRepairEnd() {}

inline void PoltaArm::exeWaitDamageEnd() {}

inline void PoltaArm::endRepair() {
    mFormationModel->kill();
}

inline void PoltaArm::exeControlled() {}
