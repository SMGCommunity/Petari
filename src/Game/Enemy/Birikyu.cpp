#include "Game/Enemy/Birikyu.hpp"

Birikyu::Birikyu(const char *pName) : LiveActor(pName), _8C(nullptr), _90(gZeroVec), _9C(gZeroVec) { 
        _A8 = false;
        _A9 = false;
        _AC.x = 0.0f;
        _AC.y = 1.0f;
        _AC.z = 0.0f;
        _B8.x = 0.0f;
        _B8.y = 0.0f;
        _B8.z = 1.0f;
        _C4 = 0.0f;
        _C8 = 10.0f;
}

void Birikyu::init(const JMapInfoIter &rIter) {
    MR::getObjectName(&_8C, rIter);
    initModelManagerWithAnm(_8C, nullptr, false);
    MR::connectToSceneEnemy(this);
    initFromJmpArgs(rIter);
    initRail(rIter);
    MR::initDefaultPos(this, rIter);
    initCollision();
    initShadow();
    initEffectKeeper(0, nullptr, false);
    MR::setGroupClipping(this, rIter, 64);
    MR::joinToGroupArray(this, rIter, nullptr, 64);
    initSound(4, false);
    TVec3f offset;
    offset.x = 0.0f;
    offset.y = 0.0f;
    offset.z = 0.0f;
    MR::initStarPointerTarget(this, 100.0f, offset);
    if (_A9) {
        initNerve(&NrvBirikyu::HostTypeMove::sInstance);
    }
    else {
        initNerve(&NrvBirikyu::HostTypeMoveCircle::sInstance);
    }
    appear();
}

void Birikyu::initAfterPlacement() {
    if (_A9) {
        MR::moveCoordAndTransToNearestRailPos(this);
    }
    else {
        _9C.set(mPosition);
        TPos3f matrix;
        matrix.identity();
        MR::makeMtxRotate(matrix.toMtxPtr(), mRotation);
        f32 z1 = matrix.mMtx[2][1];
        f32 y1 = matrix.mMtx[1][1];
        f32 x1 = matrix.mMtx[0][1];
        _AC.set(x1, y1, z1);
        MR::normalize(&_AC);        
        f32 z2 = matrix.mMtx[2][2];
        f32 y2 = matrix.mMtx[1][2];
        f32 x2 = matrix.mMtx[0][2];
        _B8.set(x2, y2, z2);
        MR::normalize(&_B8);
        TVec3f add(_9C * 400.0f);
        TVec3f vec(_9C + add);
        mPosition.set(vec);        
    }
}

void Birikyu::appear() {
    LiveActor::appear();
    MR::emitEffect(this, _8C);
}

f32 Birikyu::getHitRadius() const {
    return 120.0f;
}

char *Birikyu::getCenterJointName() const {
    return "Root";
}

void Birikyu::attackSensor(HitSensor *pSender, HitSensor *pReceiver) {
    if (MR::isSensorPlayerOrRide(pReceiver) || MR::isSensorEnemy(pReceiver)) {
        if (MR::sendMsgEnemyAttackElectric(pReceiver, pSender)) {
            MR::sendMsgToGroupMember(105, this, getSensor("body"), "body");
            setNerve(&NrvBirikyu::HostTypeAttack::sInstance);
        }
        else {
            MR::sendMsgPush(pReceiver, pSender);
        }
    }
}

bool Birikyu::receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
    return MR::isMsgStarPieceReflect(msg);
}

bool Birikyu::receiveOtherMsg(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
    if (msg == 105) {
        bool bool4 = false;
        if (isNerve(&NrvBirikyu::HostTypeMove::sInstance) || isNerve(&NrvBirikyu::HostTypeMoveCircle::sInstance)) {
            bool4 = true;
        }
        if (bool4) {
            setNerve(&NrvBirikyu::HostTypeAttackWait::sInstance);
            return true;
        }
    }

    if (msg == 104) {
        bool bool5 = false;
        if (isNerve(&NrvBirikyu::HostTypeMove::sInstance) || isNerve(&NrvBirikyu::HostTypeMoveCircle::sInstance)) {
            bool5 = true;
        }
        if (!bool5) {
            goMove();
            return true;
        }
    }    
    return false;
}

void Birikyu::startClipped() {
    LiveActor::startClipped();
    MR::forceDeleteEffect(this, _8C);
}

void Birikyu::endClipped() {
    LiveActor::endClipped();
    MR::emitEffect(this, _8C);
}

void Birikyu::initFromJmpArgs(const JMapInfoIter &rIter) {
    MR::getJMapInfoArg0NoInit(rIter, &_C8);
    s32 num = -1;
    MR::getJMapInfoArg2NoInit(rIter, &num);
    _A8 = num == true;
}

void Birikyu::initRail(const JMapInfoIter &rIter) {
    bool result = MR::isConnectedWithRail(rIter);
    _A9 = result;
    if (_A9) {
        initRailRider(rIter);
        MR::initAndSetRailClipping(&_90, this, 100.0f, 500.0f);
    }
}

void Birikyu::initCollision() {
    initHitSensor(1);
    TVec3f vec;
    vec.x = 0.0f;
    vec.y = 0.0f;
    vec.z = 0.0f;
    f32 num = getHitRadius() * mScale.x;   
    const char *joint = getCenterJointName();
    MR::addHitSensorAtJointEnemy(this, "body", joint, 16, num, vec);
}

void Birikyu::initShadow() {
    if (_A8) {
        f32 num = getHitRadius();
        MR::initShadowVolumeSphere(this, (0.80000001f * (num * mScale.x)));
        MR::onCalcShadowDropPrivateGravity(this, nullptr);
        MR::onCalcShadow(this, nullptr);
        MR::onCalcGravity(this);
        MR::setShadowDropDirectionPtr(this, nullptr, &getSensor("body")->mPosition);
    }
}

bool Birikyu::tryStopPointing() {
    if (MR::isStarPointerPointing2POnPressButton(this, "Hit", true, false)) {
        MR::sendMsgToGroupMember(105, this, getSensor("body"), "body");
        setNerve(&NrvBirikyu::HostTypeStopPointing::sInstance);
        return true;
    }
    return false;
}

void Birikyu::goMove() {
    if (_A9) {
        setNerve(&NrvBirikyu::HostTypeMove::sInstance);
    }
    else {
        setNerve(&NrvBirikyu::HostTypeMoveCircle::sInstance);
    }
}

void Birikyu::exeMove() {
    MR::startLevelSound(this, "SE_OJ_LV_BIRIKYU_MOVE", -1, -1, -1);
    if (!tryStopPointing()) {
        if (MR::isRailReachedGoal(this)) {
            MR::reverseRailDirection(this);
            s32 arg = 0;
            MR::getCurrentRailPointArg0NoInit(this, &arg);
            if (arg > 0) {
                setNerve(&NrvBirikyu::HostTypeWaitAtEdge::sInstance);
            }
            else {
                MR::emitEffect(this, "Clash");                
            }
        }
        MR::moveCoordAndFollowTrans(this, _C8);
    }
}

void Birikyu::exeMoveCircle() {
    MR::startLevelSound(this, "SE_OJ_LV_BIRIKYU_MOVE", -1, -1, -1);
    if (!tryStopPointing()) {
        f32 divis = _C8 / 400.0f;
        f32 sub = MR::subtractFromSum(divis, _C4, 0.0f);
        _C4 = MR::modAndAdd(0.0f, sub, 6.283185482025146f);
        TPos3f matrix;
        matrix.identity();
        matrix.makeRotate(_AC, _C4);
        TVec3f temp = _B8 * 400.0f;
        matrix.mult(temp, temp);
        TVec3f matrix2 = (_9C + temp);
        mPosition.set(matrix2);
    }
}

void Birikyu::exeWaitAtEdge() {
    MR::startLevelSound(this, "SE_OJ_LV_BIRIKYU_MOVE", -1, -1, -1);
    s32 arg = 0;
    MR::getCurrentRailPointArg0NoInit(this, &arg);
    if (MR::isStep(this, arg)) {
        setNerve(&NrvBirikyu::HostTypeMove::sInstance);
    }
}

void Birikyu::exeAttack() {
    if (MR::isFirstStep(this)) {
        mVelocity.z = 0.0f;
        mVelocity.y = 0.0f;
        mVelocity.x = 0.0f;
        MR::emitEffect(this, "Hit");
    }
    
    MR::startLevelSound(this, "SE_OJ_LV_BIRIKYU_MOVE", -1, -1, -1);

    if (MR::isStep(this, 90)) {
        MR::sendMsgToGroupMember(104, this, getSensor("body"), "body");
        goMove();
    }
}

void Birikyu::exeStopPointing() {
    if (MR::isFirstStep(this)) {
        MR::startDPDHitSound();
        if (MR::isRegisteredEffect(this, "Touch")) {
            MR::emitEffect(this, "Touch");
        }
    }

    MR::startDPDFreezeLevelSound(this);
    
    if (!MR::isStarPointerPointing2POnPressButton(this, "Hit", true, false)) {
        if (MR::isRegisteredEffect(this, "Touch")) {
            MR::deleteEffect(this, "Touch");
        }
        MR::sendMsgToGroupMember(104, this, getSensor("body"), "body");
        goMove();
    }
}

BirikyuWithFace::BirikyuWithFace(const char *pName) : Birikyu(pName) {
    _CC = false;
}

void BirikyuWithFace::init(const JMapInfoIter &rIter) {
    Birikyu::init(rIter);
    s32 num = -1;
    MR::getJMapInfoArg1NoInit(rIter, &num);
    _CC = num == 1;
}

void BirikyuWithFace::calcAndSetBaseMtx() {
    TPos3f vec3;    
    vec3.identity();
    if (_A9) {
        TVec3f vec(MR::getRailDirection(this));
        if (_CC) {
            vec.negate();
        }
        if (!MR::isRailGoingToEnd(this)) {
            vec.negate();
        }
        TVec3f vec2(_AC);
        if (MR::isSameDirection(vec2, vec, 0.0099999998f)) {
            MR::makeMtxSideFront(&vec3, vec, _B8);
        }
        else {
            MR::makeMtxSideUp(&vec3, vec, vec2);
        }
    }
    else {
        vec3.makeRotate(_AC, _C4);
    }
    vec3.mMtx[0][3] = mPosition.x;
    vec3.mMtx[1][3] = mPosition.y;
    vec3.mMtx[2][3] = mPosition.z;
    MR::setBaseTRMtx(this, vec3);
}

f32 BirikyuWithFace::getHitRadius() const {
    return 50.0f;
}

char *BirikyuWithFace::getCenterJointName() const {
    return "Center";
}

namespace NrvBirikyu {
    INIT_NERVE(HostTypeMove);
    INIT_NERVE(HostTypeMoveCircle);
    INIT_NERVE(HostTypeAttack);
    INIT_NERVE(HostTypeAttackWait);
    INIT_NERVE(HostTypeWaitAtEdge);
    INIT_NERVE(HostTypeStopPointing);    

	void HostTypeMove::execute(Spine *pSpine) const {
		Birikyu *pActor = (Birikyu*)pSpine->mExecutor;
		pActor->exeMove();
	}    

	void HostTypeMoveCircle::execute(Spine *pSpine) const {
		Birikyu *pActor = (Birikyu*)pSpine->mExecutor;
		pActor->exeMoveCircle();
	}    

	void HostTypeAttack::execute(Spine *pSpine) const {
		Birikyu *pActor = (Birikyu*)pSpine->mExecutor;
		pActor->exeAttack();
	}

	void HostTypeAttackWait::execute(Spine *pSpine) const {
	}    

	void HostTypeWaitAtEdge::execute(Spine *pSpine) const {
		Birikyu *pActor = (Birikyu*)pSpine->mExecutor;
		pActor->exeWaitAtEdge();
	}    

	void HostTypeStopPointing::execute(Spine *pSpine) const {
		Birikyu *pActor = (Birikyu*)pSpine->mExecutor;
		pActor->exeStopPointing();
	}
};