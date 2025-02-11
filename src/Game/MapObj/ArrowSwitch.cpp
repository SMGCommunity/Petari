#include "Game/MapObj/ArrowSwitch.hpp"
#include <cmath>
#include "math_types.hpp"

namespace {
    const f32 sRotYTargetList[4] = { 0.0f, 90.0f, 180.0f, -90.0f };
};

ArrowSwitch::ArrowSwitch(const char *pName) : LiveActor(pName) {
    _8C = 0.0f;
    mRotationIdx = 0;
    _94 = 0.0f;
    _98 = -1;
    _9C = 0;
    _9D = 1;
    _9E = 0;
    _9F = 0;
}

/*
void ArrowSwitch::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    mScale.x = 1.0f;
    mScale.y = 1.0f;
    mScale.z = 1.0f;
    initModelManagerWithAnm("ArrowSwitch", nullptr, false);
    MR::connectToSceneNoShadowedMapObjStrongLight(this);
    TVec3f up_vec; 
    MR::calcUpVec(&up_vec, this);
    TVec3f neg_up_vec;
    neg_up_vec.negateInlineAndStore(up_vec, mGravity);
    MR::initShadowFromCSV(this, "Shadow");
    MR::onCalcShadow(this, nullptr);
    MR::onCalcGravity(this);
    if (MR::isInAreaObj("PlaneModeCube", mPosition)) {
        initHitSensor(1);
        MR::addHitSensorMapObj(this, "body", 8, 100.0f, TVec3f(0.0f, 50.0f, 250.0f));
    }
    else {
        initHitSensor(2);
        MR::addHitSensorMapObj(this, "body", 8, 100.0f, TVec3f(0.0f, 50.0f, 250.0f));
        MR::addHitSensorMapObjMoveCollision(this, "collision", 8, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
        MR::initCollisionParts(this, "ArrowSwitch", getSensor("collision"), nullptr);
    }

    initSound(4, false);
    initNerve(&NrvArrowSwitch::ArrowSwitchNrvWait::sInstance);
    MR::getJMapInfoArg0WithInit(rIter, &_98);
    MR::getJMapInfoArg1WithInit(rIter, &_9C);
    s32 arg = -1;
    MR::getJMapInfoArg2WithInit(rIter, &arg);

    if (arg >= 0) {
        MR::setShadowDropLength(this, nullptr, arg);
    }
    
    MR::needStageSwitchWriteA(this, rIter);

    if (!_9C) {
        void (ArrowSwitch::*switchOff)(void) = &ArrowSwitch::listenOffSwitch;
        void (ArrowSwitch::*switchOn)(void) = &ArrowSwitch::listenOnSwitch;
        MR::listenStageSwitchOnOffB(this, MR::Functor(this, switchOff), MR::Functor(this, switchOn));
    }

    makeActorAppeared();
}
*/

void ArrowSwitch::control() {

}

void ArrowSwitch::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();
    MtxPtr baseMtx = getBaseMtx();
    TPos3f pos;
    pos.setInline(baseMtx);
    TMtx34f tr_mtx;
    tr_mtx.identity();
    f32 v11 = _8C;
    v11 = v11 * PI_180;
    f32 v12 = sin(v11);
    f32 v13 = cos(v11);
    tr_mtx.mMtx[0][2] = v12;
    tr_mtx.mMtx[1][1] = 1.0f;
    tr_mtx.mMtx[0][0] = v13;
    tr_mtx.mMtx[2][0] = -v12;
    tr_mtx.mMtx[2][2] = v13;
    tr_mtx.mMtx[2][1] = 0.0f;
    tr_mtx.mMtx[1][2] = 0.0f;
    tr_mtx.mMtx[1][0] = 0.0f;
    tr_mtx.mMtx[0][1] = 0.0f;
    pos.concat(pos, tr_mtx);
    MR::setBaseTRMtx(this, pos);
}

void ArrowSwitch::attackSensor(HitSensor *a1, HitSensor *a2) {
    if (MR::isSensorPlayer(a2)) {
        MR::sendMsgPush(a2, a1);
    }
}

bool ArrowSwitch::receiveMsgPlayerAttack(u32 msg, HitSensor *a2, HitSensor *a3) {
    if (MR::isMsgPlayerHitAll(msg)) {
        return requestPunch(a2, a3);
    }   

    return false;
}

bool ArrowSwitch::receiveMsgPush(HitSensor *, HitSensor *) {
    return false;
}

bool ArrowSwitch::requestPunch(HitSensor *a1, HitSensor *a2) {
    if (getSensor("body") != a2) {
        return false;
    }

    if (!isNerve(&NrvArrowSwitch::ArrowSwitchNrvWait::sInstance)) {
        return false;
    }

    TVec3f side;
    TVec3f direction;
    MR::calcSideVec(&side, this);
    MR::calcSensorDirection(&direction, a1, a2);

    if (isPlusLimit()) {
        _9D = 0;
    }
    else if (isMinusLimit()) {
        _9D = 1;
    }
    else {
        f32 dot = side.dot(direction);

        if (dot > 0.0f) {
            _9D = 1;
        }
        else {
            _9D = 0;
        }
    }

    s32 step = _9D ? getOneStep() : -getOneStep();
    mRotationIdx += step;
    mRotationIdx = (mRotationIdx + 4) % 4;
    _94 = _9D ? -6.0f : 6.0f;
    _9F = 1;
    MR::invalidateClipping(this);
    setNerve(&NrvArrowSwitch::ArrowSwitchNrvRotate::sInstance);
    return true; 
}

void ArrowSwitch::listenOnSwitch() {
    if (mRotationIdx == 0) {
        switch(_98) {
            case 2:
            case -1:
                _9D = 1;
                _94 = 6.0f;
                mRotationIdx = 2;
                break;
            case 3:
                _9D = 0;
                _94 = 6.0f;
                mRotationIdx = 2;
                break;
            case 0:
                _9D = 1;
                _94 = -6.0f;
                mRotationIdx = 1;
                break;
            case 1:
                _9D = 0;
                _94 = -6.0f;
                mRotationIdx = 3;
                break;
            default:
                return;
        }

        MR::invalidateClipping(this);
        setNerve(&NrvArrowSwitch::ArrowSwitchNrvRotate::sInstance);
    }
}

void ArrowSwitch::listenOffSwitch() {
    if (mRotationIdx != 0) {
        mRotationIdx = 0;

        switch(_98) {
            case 2:
            case 0:
            case -1:
                _9D = 0;
                _94 = -6.0f;
                break;
            case 1:
            case 3:
                _9D = 1;
                _94 = 6.0f;
                break;
            default:
                return;
        }

        MR::invalidateClipping(this);
        setNerve(&NrvArrowSwitch::ArrowSwitchNrvRotate::sInstance);
    }
}

void ArrowSwitch::exeWait() {
    if (MR::isFirstStep(this)) {
        if (mRotationIdx != 0) {
            MR::startBtk(this, "On");
        }
        else {
            MR::startBtk(this, "Off");
        }

        MR::validateClipping(this);
        MR::onCalcShadow(this, nullptr);
    }
}

#ifdef NON_MATCHING
void ArrowSwitch::exeRotate() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_ARROW_SWITCH_ON", -1, -1);
        MR::onCalcShadow(this, nullptr);

        if (_9C) {
            MR::invalidateHitSensors(this);
            if (MR::isExistCollisionParts(this)) {
                MR::invalidateCollisionParts(this);
            }
        }
    }

    MR::startLevelSound(this, "SE_OJ_LV_ARROW_SWITCH_MOVE", -1, -1, -1);
    f32 v2 = (_8C + _94) - -180.0f;
    _8C += _94;
    f32 v3 = fmod(360.0 + v2, 360.0);
    f32 v4 = (-180.0f + v3);
    f32 v5 = (sRotYTargetList[mRotationIdx] - (-180.0f + v3));
    _8C = v4;
    f32 v6 = (-180.0 + fmod((360.0 + (v5 - -180.0)), 360.0));

    if (_9D && v6 < 0.0f || !_9D && v6 > 0.0f) {
        f32 rot = sRotYTargetList[mRotationIdx];
        _94 = 0.0f;
        _8C = rot;
        if (MR::isValidSwitchA(this) && _9F) {
            if (mRotationIdx != 0) {
                MR::onSwitchA(this);
            }
            else {
                MR::offSwitchA(this);
            }

            MR::startSystemSE("SE_SY_GRAVITY_SWITCHED", -1, -1);
        }

        MR::startSound(this, "SE_OJ_ARROW_SWITCH_STOP", -1, -1);
        _9F = 0;
        if (_9C) {
            setNerve(&NrvArrowSwitch::ArrowSwitchNrvLock::sInstance);
        }
        else {
            setNerve(&NrvArrowSwitch::ArrowSwitchNrvWait::sInstance);
        }
    }
}
#endif

void ArrowSwitch::exeLock() {
    if (MR::isFirstStep(this)) {
        if (mRotationIdx == 0) {
            MR::startBtk(this, "Off");
        }
        else {
            MR::startBtk(this, "On");
        }

        MR::validateClipping(this);
        MR::onCalcShadow(this, nullptr);
    }
}

bool ArrowSwitch::isPlusLimit() const {
    switch (_98) {
        case 2:
            return mRotationIdx == 2;
        case 0:
            return mRotationIdx == 1;
        case 1:
            return mRotationIdx == 0;
        case 3:
            return mRotationIdx == 0;
        default:
            return false;
    }
}

bool ArrowSwitch::isMinusLimit() const {
    switch (_98) {
        case 2:
            return mRotationIdx == 0;
        case 0:
            return mRotationIdx == 0;
        case 1:
            return mRotationIdx == 3;
        case 3:
            return mRotationIdx == 2;
        default:
            return false;
    }
}

/*
s32 ArrowSwitch::getOneStep() const {
    switch (_98) {
        case 2:
        case 3:
            return 1;
        case -1:
            return 2;
    }

    return 0;
}
*/

namespace NrvArrowSwitch {
    INIT_NERVE(ArrowSwitchNrvWait);
    INIT_NERVE(ArrowSwitchNrvRotate);
    INIT_NERVE(ArrowSwitchNrvLock);

    void ArrowSwitchNrvLock::execute(Spine *pSpine) const {
        ArrowSwitch* sw = reinterpret_cast<ArrowSwitch*>(pSpine->mExecutor);
        sw->exeLock();
    }

    void ArrowSwitchNrvRotate::execute(Spine *pSpine) const {
        ArrowSwitch* sw = reinterpret_cast<ArrowSwitch*>(pSpine->mExecutor);
        sw->exeRotate();
    }

    void ArrowSwitchNrvWait::execute(Spine *pSpine) const {
        ArrowSwitch* sw = reinterpret_cast<ArrowSwitch*>(pSpine->mExecutor);
        sw->exeWait();
    }
};

ArrowSwitch::~ArrowSwitch() {

}
