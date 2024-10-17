#include "Game/Ride/Pole.hpp"
#include <JSystem/JMath.hpp>

Pole::Pole(const char *pName) : LiveActor(pName) {
    _8C.x = 0.0f;
    _8C.y = 0.0f;
    _8C.z = 0.0f;
    _98.x = 0.0f;
    _98.y = 0.0f;
    _98.z = 0.0f;
    _A4 = 0.0f;
    _A8 = false;
    _A9 = false;
    _AA = false;
    _AB = false;
    _AC = false;
    _B0 = 0.0f;
    _B4 = 0.0f;
    mActor = nullptr;
    mInfo = nullptr;
    _C0.x = 0.0f;
    _C0.y = 0.0f;
    _C0.z = 0.0f;
    _CC.x = 1.0f;
    _CC.y = 0.0f;
    _CC.z = 0.0f;
    _D8.x = 0.0f;
    _D8.y = 1.0f;
    _D8.z = 0.0f;
    _E4.x = 0.0f;
    _E4.y = 0.0f;
    _E4.z = 1.0f;
    _120 = nullptr;
    _124 = nullptr;    
    _F0.identity();
    _128.identity();
}

void Pole::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    _8C.set(mPosition);
    f32 fNum = 100.0f * mScale.y;
    mScale.x = 1.0f;
    _A4 = fNum;
    mScale.y = 1.0f;
    mScale.z = 1.0f;
    MR::getJMapInfoArg0NoInit(rIter, &_A8);
    MR::getJMapInfoArg1NoInit(rIter, &_AB);
    s32 num = -1;
    MR::getJMapInfoArg2NoInit(rIter, &num);
    if (num == 1) {
        _AC = true;
    }
    const char *name = nullptr;
    MR::getObjectName(&name, rIter);
    if (strstr(name, "NoModel")) {
        _AA = true;
    }
    if (strstr(name, "Square")) {
        _A9 = true;
        _A8 = true;
    }
    TMtx34f vec;
    vec.identity();
    MR::makeMtxTR(vec.toMtxPtr(), this);
    f32 z1 = vec.mMtx[2][0];
    f32 y1 = vec.mMtx[1][0];
    f32 x1 = vec.mMtx[0][0];
    _CC.set(x1, y1, z1);
    f32 z2 = vec.mMtx[2][1];
    f32 y2 = vec.mMtx[1][1];
    f32 x2 = vec.mMtx[0][1];
    _D8.set(x2, y2, z2);
    f32 z3 = vec.mMtx[2][2];
    f32 y3 = vec.mMtx[1][2];
    f32 x3 = vec.mMtx[0][2];
    _E4.set(x3, y3, z3);
    _F0.set(vec);
    _F0.invert(_F0);
    _F0.mMtx[0][3] = 0.0f;
    _F0.mMtx[1][3] = 0.0f;
    _F0.mMtx[2][3] = 0.0f;
    initHitSensor(3);
    MR::addHitSensorBinder(this, "bind", 8, 60.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorRide(this, "ride", 8, 30.0f, TVec3f(0.0f, 50.0f, 0.0f));
    MR::addHitSensorPush(this, "push", 8, 100.0f, TVec3f(0.0f, 0.0f, 0.0f));

    if (!_AA) {
        initModelManagerWithAnm(name, nullptr, false);
        if (_AC) {
            MR::initCollisionParts(this, "Pole9m", getSensor("ride"), nullptr);
        }
        else {
            MR::initCollisionParts(this, name, getSensor("ride"), nullptr);
        }
    }
    else {
        _128.set(vec);
        if (MR::isEqualString(name, "PoleNoModel")) {
            ResourceHolder *resource1 = MR::createAndAddResourceHolder("Pole.arc");
            MR::initCollisionPartsFromResourceHolder(this, "Pole", getSensor("ride"), resource1, _128.toMtxPtr());
        }
        else {
            ResourceHolder *resource2 = MR::createAndAddResourceHolder("PoleSquare.arc");
            MR::initCollisionPartsFromResourceHolder(this, "PoleSquare", getSensor("ride"), resource2, _128.toMtxPtr());
        }
    }

    if (MR::isEqualString(name, "Pole") || MR::isEqualString(name, "PoleSquare")) {
        _120 = MR::getJointMtx(this, "PoleTop");
        _124 = MR::getJointMtx(this, "PoleBottom");
        if (MR::isEqualString(name, "Pole") && !_A8) {
            MR::hideMaterial(this, "PoleTopStopMat_v");
        }
        Pole::updateTopPos(_A4);
    }
    else {
        if (MR::isEqualString(name, "TreeCube")) {
            _A4 = 800.0f;
        }
        _98.set(_D8);
        _98.scale(_A4);
        _98.add(_8C);
        if (!_AA) {
            _124 = MR::getJointMtx(this, "world_root");
        }
    }

    if (!_AA) {
        MR::setMtxAxisXYZ(_124, _CC, _D8, _E4);
        MR::setMtxTrans(_124, _8C);
    }

    if (_AA) {
        MR::connectToSceneMapObjMovement(this);
    }
    else {
        MR::connectToSceneMapObj(this);
    }

    initSound(4, false);
    MR::initActorCamera(this, rIter, &mInfo);
    initNerve(&NrvPole::PoleNrvFree::sInstance);
    _C0.set(_D8);
    f32 var1 = _A4;  
    _C0.scale(var1 / 2);
    _C0.add(_8C);
    f32 var2 = _A4;
    MR::setClippingTypeSphere(this, (var2 / 2), &_C0);
    MR::useStageSwitchSleep(this, rIter);

    if (MR::tryRegisterDemoCast(this, rIter)) {
        makeActorDead();
    }
    else {
        makeActorAppeared();
    }
}

void Pole::appear() {
    LiveActor::appear();
    setNerve(&NrvPole::PoleNrvDemoAppear::sInstance);
}

void Pole::calcAnim() {
    if (!_AA) {
        MR::updateMaterial(this);
    }
}

void Pole::exeDemoAppear() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensors(this);
        MR::invalidateCollisionParts(this);
    }

    updateTopPos(_A4 * MR::calcNerveRate(this, 60));

    if (MR::isDemoActive()) {
        MR::startLevelSound(this, "SE_OJ_LV_POLE_APPEAR", -1, -1, -1);
    }

    if (MR::isStep(this, 60)) {
        Pole::updateTopPos(_A4);
        MR::validateCollisionParts(this);
        MR::validateHitSensors(this);
        setNerve(&NrvPole::PoleNrvFree::sInstance);
    }
}

void Pole::exeFreeInvalid() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensors(this);
    }

    MR::calcPerpendicFootToLineInside(&mPosition, *MR::getPlayerCenterPos(), _98, _8C);

    if (MR::isOnGroundPlayer() || MR::calcDistanceToPlayer(this) > 300.0f || MR::isGreaterStep(this, 45)) {
        MR::validateHitSensors(this);
        setNerve(&NrvPole::PoleNrvFree::sInstance);
    }
}

void Pole::exeBindStart() {
    if (MR::isFirstStep(this)) {
        if (_A9) {
            MR::startBckPlayer("SquarePoleStart", (const char *)nullptr);
        }
        else {
            MR::startBckPlayer("TreeClimbStart", (const char *)nullptr);
        }
    }

    if (!_A9) {
        f32 num = 5.0f;
        if (isNerve(&NrvPole::PoleNrvBindStartFast::sInstance)) {
            num = 9.0f;
        }
        J3DFrameCtrl* ctrl = MR::getBckCtrl(mActor);       
        s16 frame = ctrl->mEndFrame; 
        mRotation.y += (num * MR::getEaseOutValue(1.0f - ((f32)getNerveStep() / frame), 0.0f, 1.0f, 1.0f));
    }

    if (!tryJump(false, 0.0f) && MR::isBckStopped(mActor)) {
        setNerve(&NrvPole::PoleNrvBindWait::sInstance);
    }
}

void Pole::exeBindWait() {
    if (MR::isFirstStep(this)) {
        if (_A9) {
            if (!MR::isBckPlaying(mActor, "SquarePoleWait")) {
                MR::startBckPlayer("SquarePoleWait", (const char *)nullptr);
            }
        }
        else {
            MR::startBckPlayer("TreeWait", (const char *)nullptr);
        }
        _B4 = 0.0f;
    }

    if (!tryJump(false, 0.0f)) {
        if (!isNerve(&NrvPole::PoleNrvBindTurnEnd::sInstance)) {
            if (tryTurn()) {
                return;                
            }
        }
        else if (!isEnableTurn()) {
            setNerve(&NrvPole::PoleNrvBindWait::sInstance);
            return;
        }

        if (!tryClimbUp()) {
            if (tryFallDown()) {
                return;                
            }
        }
    }
}

void Pole::exeBindTurn() {
    if (MR::isFirstStep(this)) {
        if (_A9) {
            if (isNerve(&NrvPole::PoleNrvBindTurnLeft::sInstance)) {
                MR::startBckPlayer("SquarePoleTurnL", (const char *)nullptr);
            }
            else {
                MR::startBckPlayer("SquarePoleTurnR", (const char *)nullptr);
            }
        }
        else {
            MR::startBckPlayer("ClimbTurn", (const char *)nullptr);
        }
    }

    if (!_A9) {
        
        f32 pad = (2.5f * getPoleSubPadStickX());
        f32 sub = MR::subtractFromSum(pad, mRotation.y, 0.0f);
        mRotation.y += pad;
        mRotation.y = MR::modAndAdd(0.0f, sub);

    }

    f32 jump = 0.0f;

    if (_A9 && MR::isGreaterStep(this, 7)) {
        if (isNerve(&NrvPole::PoleNrvBindTurnLeft::sInstance)) {
            jump = -90.0f;
        }
        else {
            jump = 90.0f;
        }
    }

    if (!tryJump(false, jump)) {
        if (_A9) {
            if (MR::isBckStopped(mActor)) {
                if (isNerve(&NrvPole::PoleNrvBindTurnRight::sInstance)) {
                    mRotation.y += 90.0f;
                }
                else {
                    mRotation.y -= 90.0f;
                }
                mRotation.y = MR::modAndAdd(0.0f, MR::subtractFromSum_3(mRotation.y, 0.0f));
                MR::startBckPlayer("SquarePoleWait", "SquarePoleTurnEnd");
                setNerve(&NrvPole::PoleNrvBindTurnEnd::sInstance);
            }
            else {
                return;
            }
        }
        else if (!isEnableTurn()) {
            setNerve(&NrvPole::PoleNrvBindWait::sInstance);
        }
    }
}

void Pole::exeBindClimbUp() {
    if (MR::isFirstStep(this)) {
        if (_A9) {
            MR::startBckPlayer("SquarePoleClimb", (const char *)nullptr);
            MR::setBckRate(mActor, 1.6f);
        }
        else {
            MR::startBckPlayer("TreeClimb", (const char *)nullptr);
            MR::setBckRate(mActor, 1.75f);            
        }
        _B4 = 0.0f;
    }

    if (!tryJump(false, 0) && !tryTurn() && !tryFallDown()) {
        if (0.0f == getPoleSubPadStickY()) {
            setNerve(&NrvPole::PoleNrvBindWait::sInstance);
        }
        else {
            _B0 += 6.0f;
            if (_B0 > _A4) {
                _B0 = _A4;
                if (!_A8) {
                    updateBindTrans();
                    setNerve(&NrvPole::PoleNrvBindHandstandStart::sInstance);
                    return;
                }
                else if (_A9) {
                    if (!MR::isBckPlaying(mActor, "SquarePolePushWait")) {
                        MR::startBckPlayer("SquarePolePushWait", (const char *)nullptr);
                    }
                }
            }
            else {
                MR::tryRumblePadVeryWeak(this, 0);
            }
            updateBindTrans();   
        }
    }
}

void Pole::exeBindFallDown() {
    if (MR::isFirstStep(this)) {
        if (_A9) {
            MR::startBckPlayer("SquarePoleFall", (const char *)nullptr);
        }
        else {
            MR::startBckPlayer("TreeFall", (const char *)nullptr);
        }
    }

    if (_A9) {
        _B4 -= 0.5f;
        _B4 = _B4 >= -18.0f ? _B4 : -18.0f;
    }
    else {
        _B4 -= 0.30000001f;
        _B4 = _B4 >= -15.0f ? _B4 : -15.0f;
    }

    _B0 += _B4;

    if (!_A9) {
        mRotation.y += 0.80000001f * _B4;
    }

    updateBindTrans();
    MR::tryRumblePadVeryWeak(this, 0);
    MR::startLevelSound(mActor, "SE_PM_LV_POLE_SLIDE", -1, -1, -1);

    if ((!_A9 && _B0 <= 120.0f) || (_A9 && _B0 <= 50.0f)) {
        TPos3f vec;
        calcGravityMtx(&vec);
        TVec3f sus(0.0f, 0.0f, 0.0f);
        sus.x = JMASinDegree(mRotation.y);
        sus.y = JMACosDegree(mRotation.y);
        vec.mult(sus, sus);
        TVec3f sus2(sus);
        sus2.scale(-30.0f);
        sus2.add(_8C);
        MR::setPlayerPos(sus2);
        MR::setPlayerFrontTargetVec(sus, 1);
        MR::endActorCamera(this, mInfo, true, -1);
        MR::endBindAndPlayerWait(this);
        mActor = nullptr;
        setNerve(&NrvPole::PoleNrvFreeInvalid::sInstance);
    }
    else if(!tryJump(false, 0.0f) && !tryTurn() && !tryClimbUp()) {
        if (0.0f == getPoleSubPadStickY()) {
            setNerve(&NrvPole::PoleNrvBindWait::sInstance);
        }
    }
}

void Pole::exeBindHandstandStart() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("TreeHandstandStart", (const char *)nullptr);
        MR::startSound(mActor, "SE_PM_POLE_STAND", -1, -1);
        MR::startSound(mActor, "SE_PV_LIFT_UP", -1, -1);
    }

    if (MR::isLessStep(this, 25)) {
        if (Pole::tryJump(false, 0.0f)) {
            return;
        }
    }
    else {
        if (Pole::tryJump(true, 0.0f)) {
            return;
        }
    }

    if (!Pole::tryHandstandTurn()) {
        if (MR::isBckStopped(mActor)) {
            setNerve(&NrvPole::PoleNrvBindHandstandWait::sInstance);
        }
    }
}

void Pole::exeBindHandstandWait() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("TreeHandstandWait", (const char *)nullptr);
    }

    if (!Pole::tryJump(true, 0.0f) && !Pole::tryHandstandTurn()) {
        if (getPoleSubPadStickY() < -0.80000001f) {
            setNerve(&NrvPole::PoleNrvBindHandstandEnd::sInstance);
        }
    }
}

void Pole::exeBindHandstandEnd() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("TreeHandstandEnd", (const char *)nullptr);
    }

    if (MR::isLessStep(this, 25)) {
        if (tryJump(true, 0.0f)) {
            return;
        }
    }
    else if (tryJump(false, 0.0f)) {
        return;

    }

    if (!tryTurn()) {
        if (MR::isBckStopped(mActor)) {
            setNerve(&NrvPole::PoleNrvBindWait::sInstance);
        }    
    }
}

void Pole::exeBindHandstandTurn() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("TreeHandstandTurn", (const char *)nullptr);
    }

    f32 pad = (2.5f * getPoleSubPadStickX()); 
    f32 temp = MR::subtractFromSum(pad, mRotation.y, 0.0f);
    mRotation.y += pad;
    mRotation.y = MR::modAndAdd(0.0f, temp);

    if (!tryJump(true, (0.0f)) && !isEnableTurn()) {
        setNerve(&NrvPole::PoleNrvBindHandstandWait::sInstance);
    }
}

void Pole::attackSensor(HitSensor *pSender, HitSensor *pReceiver) {
    if (mActor && MR::isSensorRide(pSender)) {
        MR::tryGetItem(pSender, pReceiver);
    }
    else if (MR::isSensorPush(pSender)) {
        switch (isNerve(&NrvPole::PoleNrvFreeInvalid::sInstance)) {
        case false:
            if (MR::isOnGroundPlayer()) {
                MR::sendMsgPush(pReceiver, pSender);                
            }
            break;
        default:
            break;
        }
    }
}

bool Pole::receiveMsgEnemyAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
    if (mActor && MR::isSensor(pReceiver, "bind")) {
        MR::endActorCamera(this, mInfo, true, -1);
        MR::endBindAndPlayerDamageMsg(this, msg);
        mActor = nullptr;
        setNerve(&NrvPole::PoleNrvFreeInvalid::sInstance);
        return true;
    }
    return false;
}

bool Pole::receiveOtherMsg(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
    if (MR::isMsgIsRushTakeOver(msg)) {
        return true;
    }
    else {
        if (MR::isMsgAutoRushBegin(msg)) {
            if (MR::isOnGroundPlayer() || isNerve(&NrvPole::PoleNrvFreeInvalid::sInstance)) {
                return false;
            }
            else {
                TVec3f v23(*MR::getPlayerVelocity());
                MR::vecKillElement(v23, _D8, &v23);
                f32 v8 = PSVECMag(v23.toCVec());
                f32 v11 = _D8.dot(*MR::getPlayerVelocity());
                if (__fabsf(v8) < 5.0f && v11 > 1.0f) {
                    return false;
                }
                else {
                    TVec3f v22(mPosition);
                    v22.sub(*MR::getPlayerPos());
                    MR::vecKillElement(v22, _D8, &v22);
                    if (MR::isNearZero(v22, 0.001f)) {
                        v22.set(_E4);
                    }
                    else {
                        MR::normalize(&v22);
                    }
                    _F0.mult(v22, v22);
                    v22.y = 0.0f;
                    if (!MR::isNearZero(v22, 0.001f)) {
                        MR::normalize(&v22);
                    }
                    else {
                        v22.set(_E4);
                    }
                    mActor = pSender->mActor;
                    mRotation.y = MR::calcRotateY(v22.x, v22.z);
                    if (_A9) {
                        s32 temp = (45.0f + mRotation.y) / 90.0f;
                        mRotation.y = 90.0f * temp;
                    }
                    TVec3f v21(mPosition);
                    v21.sub(_8C);  
                    _B0 = v21.dot(_D8);     
                    f32 v16 = _A4;
                    f32 temp;
                    if (_B0 < 120.0f) {
                        temp = 120.0f;
                    }
                    else if (_B0 > v16){
                        temp = v16;
                    }
                    else {
                        temp = _B0;
                    }
                    _B0 = temp;
                    _B4 = 0.0f;
                    updateBindTrans();
                    MR::invalidateClipping(this);
                    MR::startSound(mActor, "SE_PM_GRAB_OBJ", -1, -1);
                    MR::startSound(mActor, "SE_PV_LIFT_UP", -1, -1);
                    MR::startActorCameraNoTarget(this, mInfo, -1);
                    MR::tryRumblePadWeak(this, 0);
                    if (v8 > 10.0f) {
                        setNerve(&NrvPole::PoleNrvBindStartFast::sInstance);
                    }
                    else {
                        if (v8 > 6.0f) {
                            setNerve(&NrvPole::PoleNrvBindStart::sInstance);                            
                        }
                        else {
                            setNerve(&NrvPole::PoleNrvBindWait::sInstance);
                        }
                    }                 
                    return false;   
                }
            }
        }
        else if (MR::isMsgUpdateBaseMtx(msg)) {
            TMtx34f v24;
            TMtx34f v25;
            v25.identity();
            v25.mMtx[0][0] = _CC.x;
            v25.mMtx[1][0] = _CC.y;
            v25.mMtx[2][0] = _CC.z;
            v25.mMtx[0][1] = _D8.x;
            v25.mMtx[1][1] = _D8.y;
            v25.mMtx[2][1] = _D8.z;
            v25.mMtx[0][2] = _E4.x;
            v25.mMtx[1][2] = _E4.y;
            v25.mMtx[2][2] = _E4.z;
            v24.identity();
            MR::makeMtxTransRotateY(v24.toMtxPtr(), this);
            PSMTXConcat(v25.toMtxPtr(), v24.toMtxPtr(), v25.toMtxPtr());
            v25.mMtx[0][3] = mPosition.x;
            v25.mMtx[1][3] = mPosition.y;
            v25.mMtx[2][3] = mPosition.z;
            MR::setBaseTRMtx(mActor, v25.toMtxPtr());
            return true;
        }
        else {
            if (MR::isMsgRushCancel(msg)) {
                if (!MR::isDemoActive()) {
                    MR::startSound(mActor, "SE_PM_JUMP_M", -1, -1);
                    MR::startSound(mActor, "SE_PV_JUMP_JOY", -1, -1);
                }
                mActor = nullptr;
                setNerve(&NrvPole::PoleNrvFree::sInstance);
                return true;
            }
            return false;
        }
    }
}

bool Pole::tryJump(bool statement, f32 num) {
    if (MR::testCorePadTriggerA(0) || MR::testSystemTriggerA()) {
        TPos3f pos;
        calcGravityMtx(&pos);
        f32 v7 = MR::modAndAdd(0.0f, MR::subtractFromSum_2(mRotation.y, num, 0.0f));
        TVec3f mtx(0.0f, 0.0f, 0.0f);
        mtx.x = JMASinDegree(v7);
        mtx.z = JMASinDegree(v7);

        if (statement) {
            mtx.scale(-1.0f);
        }

        TVec3f vec;
        vec.x = 10.0f * mtx.x;
        vec.y = 25.0f;
        vec.z = 10.0f * mtx.z;
        pos.mult(vec, vec);
        pos.mult(mtx, mtx);
        MR::setPlayerFrontTargetVec(mtx, 1);

        if (statement) {
            MR::startBckPlayer("TreeHandStandJump", (const char *)nullptr);
        }
        else {
            MR::startBckPlayer("WallJump", (const char *)nullptr);
        }

        MR::startSound(mActor, "SE_PM_JUMP_M", -1, -1);
        MR::startSound(mActor, "SE_PV_JUMP_JOY", -1, -1);
        MR::endActorCamera(this, mInfo, 1, -1);

        if (_A9) {
            TVec3f vec2(mtx);
            vec2.scale(50.0f);
            TVec3f pos;
            vec2.add(*MR::getPlayerPos());
            MR::setPlayerPos(vec2);
        }

        MR::endBindAndPlayerJump(this, vec, 0);

        if (statement) {
            MR::becomeContinuousBckPlayer();
        }

        mActor = nullptr;
        setNerve(&NrvPole::PoleNrvFreeInvalid::sInstance);
        return true;
    }
    return false;
}

bool Pole::tryTurn() {
    if (isEnableTurn()) {
        if (getPoleSubPadStickX() > 0.0f) {
            setNerve(&NrvPole::PoleNrvBindTurnLeft::sInstance);
        }
        else {
            setNerve(&NrvPole::PoleNrvBindTurnRight::sInstance);
        }        
        return true;
    }
    return false;
}

bool Pole::tryClimbUp() {
    if (getPoleSubPadStickY() > 0.0f) {
        setNerve(&NrvPole::PoleNrvBindClimbUp::sInstance);
        return true;
    }
    return false;
}

bool Pole::tryFallDown() {
    if (getPoleSubPadStickY() < 0.0f) {
        setNerve(&NrvPole::PoleNrvBindFallDown::sInstance);
        return true;
    }
    return false;
}

bool Pole::tryHandstandTurn() {
    if (isEnableTurn()) {
        setNerve(&NrvPole::PoleNrvBindHandstandTurn::sInstance);
        return true;
    }
    return false;
}

bool Pole::isEnableTurn() const {
    f32 num = __fabsf(getPoleSubPadStickX()); 

    if ( num > 0.80000001f) {
        return true;
    }
    return false;
}

void Pole::updateBindTrans() {
    mPosition.set(_D8);
    mPosition.scale(_B0);
    mPosition.add(_8C);
}

void Pole::calcGravityMtx(TPos3f *pos) {
    pos->identity();
    MR::calcGravity(this);
    TVec3f vec(-mGravity.x, -mGravity.y, -mGravity.z);
    MR::makeMtxUpFront(pos, vec, _E4);
}

void Pole::updateTopPos(f32 num) {
    f32 fNum = num;

    if (_A9) {
        f32 number = 100.0f;
        fNum = num + number;
    }

    _98.set(_D8);
    _98.scale(fNum);
    _98.add(_8C);
    MR::setMtxAxisXYZ(_120, _CC, _D8, _E4);
    MR::setMtxTrans(_120, _98);
}

f32 Pole::getPoleSubPadStickX() const {
    if (_AB) {
        f32 negNum = -MR::getSubPadStickX(0);
        return negNum;
    }
    else {
        return MR::getSubPadStickX(0);
    }
}

f32 Pole::getPoleSubPadStickY() const {
    if (_AB) {
        f32 negNum = -MR::getSubPadStickY(0);
        return negNum;
    }
    else {
        return MR::getSubPadStickY(0);
    }
}

MtxPtr Pole::getBaseMtx() const {
    if (_AA) {
        return (MtxPtr)&_128;
    }
    else {
        return LiveActor::getBaseMtx();
    }
}

namespace NrvPole {
    INIT_NERVE(PoleNrvDemoAppear);
    INIT_NERVE(PoleNrvFree);
    INIT_NERVE(PoleNrvFreeInvalid);
    INIT_NERVE(PoleNrvBindStart);
    INIT_NERVE(PoleNrvBindStartFast);
    INIT_NERVE(PoleNrvBindWait);    
    INIT_NERVE(PoleNrvBindTurnLeft);
    INIT_NERVE(PoleNrvBindTurnRight);
    INIT_NERVE(PoleNrvBindTurnEnd);
    INIT_NERVE(PoleNrvBindClimbUp);
    INIT_NERVE(PoleNrvBindFallDown);
    INIT_NERVE(PoleNrvBindHandstandStart);       
    INIT_NERVE(PoleNrvBindHandstandWait);
    INIT_NERVE(PoleNrvBindHandstandEnd);
    INIT_NERVE(PoleNrvBindHandstandTurn);       

	void PoleNrvDemoAppear::execute(Spine *pSpine) const {
		Pole *pActor = (Pole*)pSpine->mExecutor;
		pActor->exeDemoAppear();
	}    

	void PoleNrvFree::execute(Spine *pSpine) const {
		Pole *pActor = (Pole*)pSpine->mExecutor;
		pActor->exeCalcFoot();
	}    

	void PoleNrvFreeInvalid::execute(Spine *pSpine) const {
		Pole *pActor = (Pole*)pSpine->mExecutor;
		pActor->exeFreeInvalid();
	}         

	void PoleNrvBindStart::execute(Spine *pSpine) const {
		Pole *pActor = (Pole*)pSpine->mExecutor;
		pActor->exeBindStart();
	}    

	void PoleNrvBindStartFast::execute(Spine *pSpine) const {
		Pole *pActor = (Pole*)pSpine->mExecutor;
		pActor->exeBindStart();
	}    
    
	void PoleNrvBindWait::execute(Spine *pSpine) const {
		Pole *pActor = (Pole*)pSpine->mExecutor;
		pActor->exeBindWait();
	}       

	void PoleNrvBindTurnLeft::execute(Spine *pSpine) const {
		Pole *pActor = (Pole*)pSpine->mExecutor;
		pActor->exeBindTurn();
	}    

	void PoleNrvBindTurnRight::execute(Spine *pSpine) const {
		Pole *pActor = (Pole*)pSpine->mExecutor;
		pActor->exeBindTurn();
	}    
    
	void PoleNrvBindTurnEnd::execute(Spine *pSpine) const {
		Pole *pActor = (Pole*)pSpine->mExecutor;
		pActor->exeBindWait();
	}       

	void PoleNrvBindClimbUp::execute(Spine *pSpine) const {
		Pole *pActor = (Pole*)pSpine->mExecutor;
		pActor->exeBindClimbUp();
	}    

	void PoleNrvBindFallDown::execute(Spine *pSpine) const {
		Pole *pActor = (Pole*)pSpine->mExecutor;
		pActor->exeBindFallDown();
	}    
    
	void PoleNrvBindHandstandStart::execute(Spine *pSpine) const {
		Pole *pActor = (Pole*)pSpine->mExecutor;
		pActor->exeBindHandstandStart();
	}       

	void PoleNrvBindHandstandWait::execute(Spine *pSpine) const {
		Pole *pActor = (Pole*)pSpine->mExecutor;
		pActor->exeBindHandstandWait();
	}    

	void PoleNrvBindHandstandEnd::execute(Spine *pSpine) const {
		Pole *pActor = (Pole*)pSpine->mExecutor;
		pActor->exeBindHandstandEnd();
	}    
    
	void PoleNrvBindHandstandTurn::execute(Spine *pSpine) const {
		Pole *pActor = (Pole*)pSpine->mExecutor;
		pActor->exeBindHandstandTurn();
	}       
};
