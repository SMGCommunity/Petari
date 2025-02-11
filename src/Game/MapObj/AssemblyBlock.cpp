#include "Game/MapObj/AssemblyBlock.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "JSystem/JMath/JMath.hpp"

namespace {
    static const char* sReturnPosName = "合体ブロック故郷点";
};

AssemblyBlock::AssemblyBlock(const char *pName) : LiveActor(pName) {
    _11C = -1;
    mActivationRange = -1.0f;
    _124.x = 0.0f;
    _124.y = 0.0f;
    _124.z = 0.0f;
    _130 = 0.0f;
    mActivationTime = 300;
    mBloomModel = nullptr;
    _13C = false;
    _8C.identity();
    _BC.identity();
    _EC.identity();
}

void AssemblyBlock::init(const JMapInfoIter &rIter) {
    TVec3f stack_3C;
    TVec3f stack_30;
    TVec3f stack_24;
    TVec3f stack_18;
    char name[0x100];

    MR::initDefaultPos(this, rIter);
    MR::tryRegisterNamePosLinkObj(this, rIter);

    if (!MR::tryFindLinkNamePos(this, sReturnPosName, _EC.toMtxPtr())) {
        stack_18.x = 0.0f;
        stack_18.y = 1000.0f;
        stack_18.z = 1000.0f;
        stack_3C.add(mPosition, stack_18);
        _EC.setTrans(stack_3C);
    }

    MR::getMapPartsObjectNameIfExistShapeID(name, 0x100, rIter);
    initModelManagerWithAnm(name, nullptr, false);

    if (MR::isEqualString(name, "AssemblyBlockPartsTimerA")) {
        _13C = true;
    }
    else {
        _13C = false;
    }

    JMath::gekko_ps_copy12(&_BC, getBaseMtx());
    JMath::gekko_ps_copy12(&_8C, &_EC);
    MR::setBaseTRMtx(this, _8C);

    if (MR::isEqualSubString(name, "PartsIce")) {
        MR::connectToSceneIndirectMapObj(this);
    }
    else {
        MR::connectToSceneMapObj(this);
    }

    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);
    MR::initCollisionPartsAutoEqualScaleOne(this, name, getSensor(nullptr), nullptr);

    _BC.getTrans(stack_30);
    _EC.getTrans(stack_24);
    _124.add(stack_30, stack_24);
    _124.x *= 0.5f;
    _124.y *= 0.5f;
    _124.z *= 0.5f;
    f32 boundRadius;
    MR::calcModelBoundingRadius(&boundRadius, this);
    f32 dist = PSVECDistance(&_124, &stack_24);
    MR::setClippingTypeSphere(this, (boundRadius + dist), &_124);
    MR::getJMapInfoArg0NoInit(rIter, &mActivationRange);
    if (mActivationRange <= 0.0f) {
        mActivationRange = 2.0f * boundRadius;
    }

    MR::getJMapInfoArg1NoInit(rIter, &mActivationTime);
    MR::getJMapInfoArg7NoInit(rIter, &_11C);
    if (MR::getRandom((s32)0, (s32)2)) {
        _130 = 0.0f;
    }
    else {
        _130 = -0.1f;
    }

    if (_11C == -1) {
        initEffectKeeper(0, "AssemblyBlock", false);
    }
    else {
        initEffectKeeper(0, nullptr, false);
    }

    initSound(4, false);
    TVec3f stack_C;
    stack_C.x = 0.0f;
    stack_C.y = 0.0f;
    stack_C.z = 0.0f;
    MR::initStarPointerTarget(this, 0.0f, stack_C);

    if (MR::isEqualString(name, "AssemblyBlockPartsTimerA")) {
        mBloomModel = MR::createBloomModel(this, getBaseMtx());
    }

    MR::tryStartAllAnim(this, "Wait");
    initNerve(&NrvAssemblyBlock::AssemblyBlockNrvWait::sInstance);
    makeActorAppeared();
}

// AssemblyBlock::exeWait

void AssemblyBlock::exeAssemble() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "Blur");

        if (_13C) {
            MR::startSound(this, "SE_OJ_ASSEMBLE_BLOCK_ST_F_", -1, -1);
        }
        else {
            MR::startSound(this, "SE_OJ_ASSEMBLE_BLOCK_START", -1, -1);
        }

        MR::invalidateCollisionParts(this);
        MR::invalidateHitSensors(this);
    }

    MR::blendMtx(_BC.toMtxPtr(), _EC.toMtxPtr(), MR::calcNerveRate(this, 10), _8C.toMtxPtr());

    if (MR::isStep(this, 10)) {
        if (_13C) {
            MR::startSound(this, "SE_OJ_ASSEMBLE_BLOCK_ED_F", -1, -1);
        }
        else {
            MR::startSound(this, "SE_OJ_ASSEMBLE_BLOCK_END", -1, -1);
        }

        setNerve(&NrvAssemblyBlock::AssemblyBlockNrvWait::sInstance);
    }
}

void AssemblyBlock::exeAssembleWait() {
    if (MR::isFirstStep(this)) {
        if (MR::isEffectValid(this, "Blur")) {
            MR::deleteEffect(this, "Blur");
        }

        MR::tryRumblePadWeak(this, 0);
        MR::validateCollisionParts(this);
        MR::validateHitSensors(this);
    }

    if (!_11C) {
        setNerve(&NrvAssemblyBlock::AssemblyBlockNrvTimer::sInstance);
    }
    else {
        tryStartReturn();
    }
}

void AssemblyBlock::exeReturn() {
    if (MR::isFirstStep(this)) {
        MR::tryRumblePadWeak(this, 0);
        MR::emitEffect(this, "Blur");

        if (_13C) {
            MR::startSound(this, "SE_OJ_ASSEMBLE_BLOCK_RET_F", -1, -1);
        }
        else {
            MR::startSound(this, "SE_OJ_ASSEMBLE_BLOCK_RETURN", -1, -1);
        }

        MR::invalidateCollisionParts(this);
        MR::invalidateHitSensors(this);
    }

    MR::blendMtx(_BC.toMtxPtr(), _EC.toMtxPtr(), MR::calcNerveRate(this, 10), _8C.toMtxPtr());

    if (MR::isStep(this, 10)) {
        setNerve(&NrvAssemblyBlock::AssemblyBlockNrvWait::sInstance);
    }
}

void AssemblyBlock::exeTimer() {
    if (MR::isStep(this, mActivationTime - 100)) {
        MR::tryStartAllAnim(this, "Disappear");
    }

    if (MR::isStep(this, mActivationTime)) {
        if (_13C) {
            MR::startSound(this, "SE_OJ_ASSEMBLE_BLOCK_VAN_F", -1, -1);
        }
        else {
            MR::startSound(this, "SE_OJ_ASSEMBLE_BLOCK_VANISH", -1, -1);
        }

        if (mBloomModel) {
            mBloomModel->kill();
        }

        kill();
    }
}

void AssemblyBlock::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _8C);
}

bool AssemblyBlock::tryStartAssemble() {
    bool isNotNear = !MR::isNearPlayerAnyTime(this, mActivationRange);

    if (isNotNear) {
        return false;
    }

    if (MR::isPlayerDead()) {
        return false;
    }

    setNerve(&NrvAssemblyBlock::AssemblyBlockNrvAssemble::sInstance);
    return true;
}

bool AssemblyBlock::tryStartReturn() {
    if (MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        return false;
    }

    if (MR::isNearPlayerAnyTime(this, (mActivationRange + 50.0f))) {
        return false;
    }

    if (MR::isPlayerDead()) {
        return false;
    }

    setNerve(&NrvAssemblyBlock::AssemblyBlockNrvReturn::sInstance);
    return true;
}

AssemblyBlock::~AssemblyBlock() {

}

namespace NrvAssemblyBlock {
    INIT_NERVE(AssemblyBlockNrvWait);
    INIT_NERVE(AssemblyBlockNrvAssemble);
    INIT_NERVE(AssemblyBlockNrvAssembleWait);
    INIT_NERVE(AssemblyBlockNrvReturn);
    INIT_NERVE(AssemblyBlockNrvTimer);
};
