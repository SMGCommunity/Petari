#include "Game/Enemy/IceMerameraKing.hpp"
#include "Game/Enemy/Meramera.hpp"
#include <JSystem/JMath.hpp>
#include "math_types.hpp"

namespace {
    static const char* hScaleJointName[3] = {
        "WideInSide",
        "WideOutSide",
        "Hirgh1"
    };

    static const f32 hOnAirParam[4] = {
        0.0f,
        3.0f,
        0.95999998f,
        0.0f
    };          

    static const f32 hOnGroundParam[4] = {
        0.0f,
        1.0f,
        0.80000001f,
        0.0f
    };    

    static const f32 hFlyParam[4] = {
        0.0f,
        0.5f,
        0.89999998f,
        1.0f
    };        

    static const f32 hAngryDemoParam[4] = {
        0.0f,
        0.5f,
        0.89999998f,
        1.5f
    };        

    static const f32 hEscapeOnGroundParam[4] = {
        -0.69999999f,
        1.0f,
        0.95999998f,
        180.0f
    };    

    static const f32 hEscapeOnAirParam[4] = {
        0.0f,
        3.0f,
        0.95999998f,
        0.0f
    };     

    static const f32 hPreRecoverJumpParam[4] = {
        0.0f,
        1.0f,
        0.95999998f,
        0.0f
    };

    static const f32 hDamageJumpParam[4] = {
        0.0f,
        1.9f,
        0.98000002f,
        0.0f
    };                           

    static const f32 hAttackParam[4] = {
        0.0f,
        6.0f,
        0.98000002f,
        0.0f
    };         

    static f32 hExtinguishOnAirParam[4] = {
        0.0f,
        0.125f,
        0.95999998f,
        0.0f
    };        

    static f32 hExtinguishFallOnAirParam[4] = {
        0.0f,
        4.0f,
        0.95999998f,
        0.0f
    };           
};

IceMerameraKing::IceMerameraKing(const char *pName) : LiveActor(pName), mFixedPos(nullptr), _90(nullptr), 
_94(nullptr), mActor(nullptr), _A0(0), _A4(0), _A8(nullptr), _AC(nullptr), _B0(0, 0, 1), _BC(0, 1, 0), 
_C8(0, 0, 0), _D4(0, 0, 0), _E0(0), _E4(0), _EC(3), _F0(0), mModelArray(), _100(0, 0, 0, 1) {
    _120 = false;
    _110.x = 0.0f;
    _110.y = 0.0f;
    _110.z = 0.0f; 
    _11C = 0.0f;
    _121 = false;
}

void IceMerameraKing::init(const JMapInfoIter &rIter) {
    if (MR::isValidInfo(rIter)) {
        MR::initDefaultPos(this, rIter);
        _C8.set<f32>(mPosition);
        _D4.set<f32>(mRotation);
        MR::useStageSwitchWriteDead(this, rIter);
        MR::useStageSwitchReadA(this, rIter);
    }

    mScale.x = 1.5f;
    mScale.y = 1.5f;
    mScale.z = 1.5f;
    initModelManagerWithAnm("IceMerameraKing", nullptr, false);
    MR::calcUpVec(&_BC, this);
    MR::calcFrontVec(&_B0, this);
    MR::connectToSceneEnemy(this);
    f32 v5 = (mScale.y / 1.8f);
    initBinder((130.0f * v5), (130.0f * v5), 0);
    MR::setBinderOffsetVec(this, &_110, true);
    initHitSensor(1);
    MR::addHitSensorAtJointEnemy(this, "body", "JointRoot", 32, (130.0f * v5), TVec3f(0.0f, 0.0f, 0.0f));
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    initNerve(&NrvIceMerameraKing::HostTypeNrvSearch::sInstance);
    MR::calcGravity(this);
    MR::initShadowVolumeSphere(this, (130.0f * mScale.y));
    MR::setShadowDropLength(this, nullptr, 1500.0f);
    MR::declarePowerStar(this);
    MR::declareStarPiece(this, 24);
    mFixedPos = new FixedPosition(this, "Top", TVec3f(0, 0, 0), TVec3f(0.0f, 0.0f, 0.0f));
    MR::tryRegisterDemoCast(this, rIter);
    mActor = new ThrowingIce*[0x6]; //
    s32 i = 0;
    _A0 = 6;

    while (i < 6) {
        mActor[i] = new ThrowingIce("投擲用の氷");
        mActor[i]->initWithoutIter();
        _A4 += 1;
        MR::tryRegisterDemoCast(mActor[i], rIter);
        i++;
    }

    MR::initLightCtrl(this);
    _A8 = new IceMerameraKingShockWave();
    _A8->initWithoutIter();
    _A8->mRotation.set<f32>(mRotation);
    _A8->makeActorDead();
    _AC = new ModelObj("壊れモデル", "IceMerameraKingBreak", nullptr, -2, -2, -2, false);
    _AC->initWithoutIter();
    _AC->mScale.set<f32>(mScale);
    _AC->makeActorDead();
    s32 childNum = MR::getChildObjNum(rIter);
    _F0 = childNum;
    mModelArray.mArray.mArr = new ThrowingIce*[childNum];

    for (s32 i = 0; i < _F0; i++) {
        mModelArray.mArray.mArr[i] = new ThrowingIce("メラメラ"); //wrong
        MR::initChildObj(mModelArray.mArray.mArr[i], rIter, i);
        mModelArray.mArray.mArr[i]->appear();
        MR::tryRegisterDemoCast(mModelArray.mArray.mArr[i], rIter);
    }

    MR::tryRegisterDemoCast(_AC, rIter);
    MR::registerDemoActionNerve(this, &NrvIceMerameraKing::HostTypeNrvBeginDemo::sInstance, nullptr);
    mSpinParticle = new SpinPullParticleCallBack();
    mJointController = MR::createJointDelegatorWithNullChildFunc(this, &IceMerameraKing::calcJoint, "JointRoot");
    makeActorAppeared();
    MR::emitEffect(this, "BodyIce");
    _F8 = new CameraTargetDemoActor(getBaseMtx(), "アクター注目");
    _F8->initWithoutIter();
    _F8->mPosition.set<f32>(_C8);
    _F8->mRotation.set<f32>(_D4);
    _F8->setName("メラキン注目ターゲット");
    MR::tryRegisterDemoCast(_F8, rIter);
    //major
}

void IceMerameraKing::initAfterPlacement() {
    MR::trySetMoveLimitCollision(this);  
    for (s32 i = 0; i < _F0; i++) {
        bool cChar;
        bool cc = mBinder->_1EC._0;
        mModelArray.mArray.mArr[i]->makeActorAppeared();
        mModelArray.mCount = mBinder->_24;
        //mModelArray.mArray.init();
        if (mBinder->_24) {
            cChar = cc & 0xDF;            
        }
        else {
            cChar = cc | 0x20;
        }
        cc = cChar;
    }
    //major
}


void IceMerameraKing::kill() {
    LiveActor::kill();

    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }
    MR::startAfterBossBGM();
    s32 temp = _A4;

    for (s32 i = 0; i < temp; i++) {
        if (!MR::isDead(mActor[i])) {
            mActor[i]->kill();
        }
    }
}

void IceMerameraKing::control() {
    TVec3f v7;
    MR::copyJointScale(this, "Center", &v7);
    f32 v3 = 130.0f * v7.x;
    MR::setBinderRadius(this, v3);
    _110.y = v3;
    _110.x = 0.0f;
    _110.z = 0.0f;
    getSensor("body")->mRadius = v3;
    MR::setShadowVolumeSphereRadius(this, nullptr, v3);

    if (MR::isValidSwitchA(this)) {
        if (MR::isOnSwitchA(this)) {
            MR::invalidateClipping(this);
        }
        else {
            MR::validateClipping(this);
        }
    }

    if (!isNerve(&NrvIceMerameraKing::HostTypeNrvEscape::sInstance) && !isNerve(&NrvIceMerameraKing::HostTypeNrvEscapeJump::sInstance)) {
        TQuat4f quad(0, 0, 0, 1);
        _100.slerp(quad, 0.25f);
    }

    if (!isNerve(&NrvIceMerameraKing::HostTypeNrvEscape::sInstance) && !isNerve(&NrvIceMerameraKing::HostTypeNrvEscapeJump::sInstance)) {
        MR::startLevelSound(this, "SE_BM_LV_ICEMERAKING_WAIT", -1, -1, -1);
    }

    if (MR::isDemoActive("メラキンオープニング")) {
        MR::startSystemLevelSE("SE_DM_LV_MUTE_BGM", -1, -1);
        if (MR::isDemoPartLastStep("カメラ寄る")) {
            MR::playLevelMarioPinchBGM(_120);
            _120 = true;
            _121 = true;
        }
    }
    else if (!isNerve(&NrvIceMerameraKing::HostTypeNrvDeathDemoWait::sInstance) && !isNerve(&NrvIceMerameraKing::HostTypeNrvDeathDemo::sInstance) && MR::isNearPlayer(this, 5000.0f) && _121) {
        MR::playLevelMarioPinchBGM(_120);
        _120 = true;
    }
}

void IceMerameraKing::exeSearch() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Wait");
    }
    addVelocityToInitPos();
    MR::moveAndTurnToPlayer(this, &_B0, hFlyParam[0], hFlyParam[1], hFlyParam[2], hFlyParam[3]);
    MR::addVelocityKeepHeightUseShadow(this, 400.0f, 1.5f, 300.0f, nullptr);
    f32 shadow = MR::calcDistanceToPlayer(this);

    if (!(4500.0f < shadow)) {
        if (shadow < 1000.0f) {
            setNerve(&NrvIceMerameraKing::HostTypeNrvPreAttack::sInstance);
        }
        else {
            if (isEnableThrow()) {
                if (!(_EC > 2)) {
                    if (MR::isGreaterStep(this, 200) && isDeadAllIce()) {
                        _E0 = 0;
                        setNerve(&NrvIceMerameraKing::HostTypeNrvThrow2nd::sInstance);
                    }
                }
                else if (MR::isGreaterStep(this, 100) && isDeadAllIce()) {
                    _E0 = 0;
                    setNerve(&NrvIceMerameraKing::HostTypeNrvThrow::sInstance);
                }
            }            
        }
    }
}

void IceMerameraKing::exeThrow() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "AttackThrow");
        if (isNerve(&NrvIceMerameraKing::HostTypeNrvBeginDemo::sInstance)) {
            MR::calcFrontVec(&_B0, _F8);
        }
    }
    MR::moveAndTurnToPlayer(this, &_B0, hFlyParam[0], hFlyParam[1], hFlyParam[2], hFlyParam[3]);
    MR::addVelocityKeepHeightUseShadow(this, 400.0f, 1.5f, 300.0f, nullptr);
    addVelocityToInitPos();
    
    if (MR::isStep(this, 22)) {   
        _90 = getDeadWeaponAndAppear();
        mFixedPos->_1C.getTrans(_90->mPosition);
        if (!_90) {
            setNerve(&NrvIceMerameraKing::HostTypeNrvSearch::sInstance);
            return;
        }
        _E0++;
        TVec3f v11(*MR::getPlayerVelocity());
        v11.scale(35.0f);
        TVec3f v12(*MR::getPlayerCenterPos());
        v12.add(v11);
        _90->emitIce(mPosition, v12, -5.0f, mGravity);
        _90 = nullptr;
        MR::startSound(this, "SE_BM_ICEMERAKING_THROW", -1, -1);
    }

    if (MR::isActionEnd(this)) {
        if (MR::calcDistanceToPlayer(this) > 1000.0f) {
            setNerve(&NrvIceMerameraKing::HostTypeNrvPreAttack::sInstance);
        }
        else {
            s32 v10 = 3;
            if (isNerve(&NrvIceMerameraKing::HostTypeNrvThrow2nd::sInstance)) {
                v10 = 6;
            }            

            if (v10 > _E0 || isEnableThrow()) {
                setNerve(&NrvIceMerameraKing::HostTypeNrvSearch::sInstance);
            }          
            else {
                if (isNerve(&NrvIceMerameraKing::HostTypeNrvThrow2nd::sInstance)) {
                    setNerve(&NrvIceMerameraKing::HostTypeNrvThrow2nd::sInstance);
                }
                else {
                    setNerve(&NrvIceMerameraKing::HostTypeNrvThrow::sInstance);
                }                
            }  
        }
    }
    //minor
}

void IceMerameraKing::tearDownThrow() {
    if (_90) {
        _90->kill();
        _90 = nullptr;
    }

    if (_94) {
        _94->kill();
        _94 = nullptr;
    }
}

/*
void IceMerameraKing::exeExtinguish() {
    hOnAirParam;
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "GoOut");
        MR::startSound(this, "SE_BM_ICEMERAKING_BLOW", -1, -1);
        MR::startSound(this, "SE_BM_ICEMERAKING_SMOKE", -1, -1);
        MR::deleteEffect(this, "BodyIce");        
        MR::emitEffectWithParticleCallBack(this, "BodyIceOff", mSpinParticle);
        TVec3f v3 = *MR::getPlayerCenterPos();
        TVec3f v4(mPosition);
        v4.subtract(v3);
        MR::vecKillElement(v4, mGravity, &v4);
        MR::normalizeOrZero(&v4);
        MR::addVelocitySeparateHV(this, v4, 20.0f, 0.0f);
        _11C = 200.0f + MR::getShadowNearProjectionLength(this);
    }

    if (MR::isLessStep(this, 40)) {
        MR::addVelocityKeepHeightUseShadow(this, _11C, 1.3f, 700.0f, nullptr);     
        MR::moveAndTurnToPlayer(this, &_B0, hExtinguishOnAirParam[0], hExtinguishOnAirParam[1], hExtinguishOnAirParam[2], hExtinguishOnAirParam[3]);
    }
    else if (MR::isOnGround(this) && MR::isActionEnd(this)) {
        MR::moveAndTurnToPlayer(this, &_B0, hOnGroundParam[0], hOnGroundParam[1], hOnGroundParam[2], hOnGroundParam[3]);
        MR::startSound(this, "SE_BM_ICEMERAKING_LAND", -1, -1);
        _E8 = 0;
        setNerve(&NrvIceMerameraKing::HostTypeNrvEscape::sInstance);
    }
    else {
        MR::moveAndTurnToPlayer(this, &_B0, hExtinguishFallOnAirParam[0], hExtinguishFallOnAirParam[1], hExtinguishFallOnAirParam[2], hExtinguishFallOnAirParam[3]);
    }
    //minor. problems on the params
}
*/

void IceMerameraKing::exeEscape() {
    MR::isFirstStep(this);
    s32 v3 = _E8 + 1;
    _E8 = v3;

    if (v3 > 0x258u && MR::isOnGround(this)) {
        _E8 = 0;
        setNerve(&NrvIceMerameraKing::HostTypeNrvPreRecover::sInstance);
    }
    else {
        MR::emitEffect(this, "Rolling");
        if (MR::isOnGround(this) ){
            f32 v11 = MR::getLinerValueFromMinMax(PSVECMag(&mVelocity), 2.0f, 6.0f, 0.0f, 100.0f);
            MR::startLevelSound(this, "SE_BM_LV_ICEMERAKING_ROLL", v11, -1, 15);
        }

        if (MR::isOnGround(this)) {
            f32 radius = getSensor("body")->mRadius;
            MR::rotateQuatRollBall(&_100, mVelocity, *MR::getGroundNormal(this), (0.5f * radius));
            MR::moveAndTurnToPlayer(this, &_B0, hEscapeOnGroundParam[0], hEscapeOnGroundParam[1], hEscapeOnGroundParam[2], hEscapeOnGroundParam[3]);
            if (MR::isBindedWallFront(this, -_B0, 0.25f)) {
                setNerve(&NrvIceMerameraKing::HostTypeNrvEscapeJump::sInstance);
            }
        }
        else {
            f32 sensor = getSensor("body")->mRadius;
            MR::rotateQuatRollBall(&_100, mVelocity, -mGravity, 0.5f * sensor);
            MR::moveAndTurnToPlayer(this, &_B0, hEscapeOnAirParam[0], hEscapeOnAirParam[1], hEscapeOnAirParam[2], hEscapeOnAirParam[3]);
        }
    }
}

void IceMerameraKing::exeEscapeJump() {
    if (MR::isStep(this, 20)) {
        MR::addVelocitySeparateHV(this, _B0, 20.0f, 90.0f);
        MR::startSound(this, "SE_BM_ICEMERAKING_JUMP", -1, -1);
    }
    _E8++;

    if (MR::isOnGround(this)) {
        f32 radius = getSensor("body")->mRadius;
        MR::rotateQuatRollBall(&_100, mVelocity, *MR::getGroundNormal(this), (0.5f * radius));
        MR::moveAndTurnToPlayer(this, &_B0, hEscapeOnGroundParam[0], hEscapeOnGroundParam[1], hEscapeOnGroundParam[2], hEscapeOnGroundParam[3]);
        if (MR::isGreaterStep(this, 20)) {
            MR::emitEffect(this, "Land");
            MR::startSound(this, "SE_BM_ICEMERAKING_LAND", -1, -1);
            setNerve(&NrvIceMerameraKing::HostTypeNrvEscape::sInstance);
        } 
    }
    else {
        f32 radius = getSensor("body")->mRadius;
        MR::rotateQuatRollBall(&_100, mVelocity, -mGravity, (0.5f * radius));
        MR::moveAndTurnToPlayer(this, &_B0, hEscapeOnAirParam[0], hEscapeOnAirParam[1], hEscapeOnAirParam[2], hEscapeOnAirParam[3]);
    }
}

void IceMerameraKing::exeDamage() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Damage");
        MR::emitEffect(this, "Damage");
        MR::startBlowHitSound(this);
        MR::startSound(this, "SE_BM_ICEMERAKING_DAMAGE", -1, -1),
        MR::setVelocityJump(this, 80.0f);
        MR::stopSceneForDefaultHit(4);
    }

    if (MR::isGreaterStep(this, 20)) {
        MR::addVelocityKeepHeightUseShadow(this, 900.0f, 3.0f, 300.0f, nullptr);
    }

    if (MR::isOnGround(this)) {         
        MR::moveAndTurnToPlayer(this, &_B0, hOnGroundParam[0], hOnGroundParam[1], hOnGroundParam[2], hOnGroundParam[3]);
    }
    else {
        MR::moveAndTurnToPlayer(this, &_B0, hDamageJumpParam[0], hDamageJumpParam[1], hDamageJumpParam[2], hDamageJumpParam[3]);
    }

    if (MR::isGreaterStep(this, 60)) {
        if (_EC == 1) {
            if (MR::tryStartDemoRegisteredMarioPuppetable(this, "怒りデモ")) {
                setNerve(&NrvIceMerameraKing::HostTypeNrvAngryDemo2nd::sInstance);
            }
        }
        else if (MR::tryStartDemoRegisteredMarioPuppetable(this, "怒りデモ")) {
            setNerve(&NrvIceMerameraKing::HostTypeNrvAngryDemo1st::sInstance);
        }
    }
}

void IceMerameraKing::exePreRecover() {
    if (MR::isFirstStep(this)) {
        MR::setVelocityJump(this, 70.0f);
        MR::startSound(this, "SE_BM_ICEMERAKING_JUMP", -1, -1);
    }
    addVelocityToInitPos();

    if (MR::isLessStep(this, 50)) {
        MR::moveAndTurnToPlayer(this, &_B0, hPreRecoverJumpParam[0], hPreRecoverJumpParam[1], hPreRecoverJumpParam[2], hPreRecoverJumpParam[3]);
    }
    else if (MR::isStep(this, 50)) {
        MR::startAction(this, "PreRecover");
        MR::setBinderRadius(this, 130.0f);
        HitSensor* sensor = getSensor("body");
        sensor->mRadius = 130.0f;
        MR::setShadowVolumeSphereRadius(this, nullptr, 130.0f);
        MR::moveAndTurnToPlayer(this, &_B0, hFlyParam[0], hFlyParam[1], hFlyParam[2], hFlyParam[3]);
    }
    else if (MR::isLessStep(this, 120)) {
        MR::addVelocityKeepHeightUseShadow(this, 750.0f, 1.5f, 300.0f, nullptr);
        MR::moveAndTurnToPlayer(this, &_B0, hFlyParam[0], hFlyParam[1], hFlyParam[2], hFlyParam[3]);
    }
    else {
        setNerve(&NrvIceMerameraKing::HostTypeNrvRecover::sInstance);
    }
}

void IceMerameraKing::exeRecover() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Recover");
        MR::emitEffect(this, "BodyIce");
        MR::startSound(this, "SE_BM_ICEMERAKING_PRE_RECOVER", -1, -1);
    }

    if (MR::isStep(this, 80)) {
        MR::startSound(this, "SE_BM_ICEMERAKING_RECOVER", -1, -1);
    }
    addVelocityToInitPos();
    MR::moveAndTurnToPlayer(this, &_B0, hFlyParam[0], hFlyParam[1], hFlyParam[2], hFlyParam[3]);
    MR::addVelocityKeepHeightUseShadow(this, 750.0f, 1.5f, 300.0f, nullptr);

    if (MR::isGreaterStep(this, 160)) {
        setNerve(&NrvIceMerameraKing::HostTypeNrvSearch::sInstance);
    }
}

void IceMerameraKing::exePreAttack() {
    MR::isFirstStep(this);
    if (MR::isLessStep(this, 120)) {
        MR::addVelocityKeepHeightUseShadow(this, 800.0f, 1.5f, 300.0f, nullptr);
    }
    else if (MR::isDead(_A8)) {
        setNerve(&NrvIceMerameraKing::HostTypeNrvAttack::sInstance);
        return;
    }
    addVelocityToInitPos();
    MR::moveAndTurnToPlayer(this, &_B0, hFlyParam[0], hFlyParam[1], hFlyParam[2], hFlyParam[3]);
}

void IceMerameraKing::exeAttack() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Attack");
        MR::startSound(this, "SE_BM_ICEMERAKING_PRE_ATTACK", -1, -1);
    }

    if (!MR::isActionEnd(this)) {
        MR::addVelocityKeepHeightUseShadow(this, 800.0f, 1.5f, 300.0f, nullptr);
        MR::moveAndTurnToPlayer(this, &_B0, hFlyParam[0], hFlyParam[1], hFlyParam[2], hFlyParam[3]);        
    }
    else {
        MR::moveAndTurnToPlayer(this, &_B0, hAttackParam[0], hAttackParam[1], hAttackParam[2], hAttackParam[3]);
    }

    if (MR::isOnGround(this)) {
        setNerve(&NrvIceMerameraKing::HostTypeNrvAttackAfter::sInstance);
    }
}

void IceMerameraKing::exeAttackAfter() {
    if (MR::isFirstStep(this)) {
        _A8->appear();
        _A8->mPosition.set<f32>(mPosition);
        _A8->mPosition.set<f32>(_D4);
        MR::emitEffect(this, "Land");
        MR::startAction(this, "AttackEnd");
        MR::startSound(this, "SE_BM_ICEMERAKING_HIP_DROP", -1, -1);
        MR::tryRumblePadAndCameraDistanceStrong(this, 800.0f, 1200.0f, 2000.0f);
    }
    MR::moveAndTurnToPlayer(this, &_B0, hOnAirParam[0], hOnAirParam[1], hOnAirParam[2], hOnAirParam[3]);

    if (MR::isGreaterStep(this, 90)) {
        setNerve(&NrvIceMerameraKing::HostTypeNrvSearch::sInstance);
    }
}

void IceMerameraKing::exeAngryDemo() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "BodyIce");
        if (isNerve(&NrvIceMerameraKing::HostTypeNrvAngryDemo2nd::sInstance)) {
            MR::startAction(this, "Summon");
            MR::startSound(this, "SE_BM_ICEMERAKING_PRE_ANGRY2", -1, -1); 
        }
        else {
            MR::startAction(this, "Recover");
            MR::startSound(this, "SE_BM_ICEMERAKING_PRE_ANGRY1", -1, -1);             
        }
    }

    if (MR::isStep(this, 1)) {
        MR::resetPosition(this, _C8);
    }
    MR::moveAndTurnToPlayer(this, &_B0, hAngryDemoParam[0], hAngryDemoParam[1], hAngryDemoParam[2], hAngryDemoParam[3]);
    MR::addVelocityKeepHeightUseShadow(this, 400.0f, 1.5f, 300.0f, nullptr);

    if (isNerve(&NrvIceMerameraKing::HostTypeNrvAngryDemo2nd::sInstance) && MR::isStep(this, 90)) {
        for (s32 i = 0; i < _F0; i++) {
            mModelArray.mArray.mArr[i]->makeActorAppeared();
        }
    }

    if (isNerve(&NrvIceMerameraKing::HostTypeNrvAngryDemo2nd::sInstance) && MR::isStep(this, 85)) {
        MR::startSound(this, "SE_BM_ICEMERAKING_ANGRY2", -1, -1);
    }
    else if (MR::isStep(this, 85)) {
        MR::startSound(this, "SE_BM_ICEMERAKING_ANGRY1", -1, -1);
    }
    MR::playLevelMarioPinchBGM(true);

    if (MR::isDemoPartLastStep("怒りデモ")) {
        if (!(_EC > 2)) {
            TVec3f v7(mGravity);
            v7.scale(200.0f);
            TVec3f v8(mPosition);
            v8.sub(v7);
            MR::appearStarPiece(this, v8, 8, 15.0f, 70.0f, false);                  
        }
        else {
            TVec3f v5(mGravity);
            v5.scale(200.0f);
            TVec3f v6(mPosition);
            v6.sub(v5);
            MR::appearStarPiece(this, v6, 16, 15.0f, 70.0f, false);      
        }
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
        setNerve(&NrvIceMerameraKing::HostTypeNrvSearch::sInstance);
    }
}

void IceMerameraKing::exeDeathDemo() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Death");
        MR::startSound(this, "SE_BM_ICEMERAKING_DEAD", -1, -1);
        _F8->mPosition.set<f32>(mPosition);
        _AC->appear();
        _AC->mRotation.set<f32>(_D4);
        MR::resetPosition(_AC, mPosition);
        MR::startAction(_AC, "Break");
        MR::requestMovementOn(_AC);
        for (s32 i = 0; i < _F0; i++) {
            mModelArray.mArray.mArr[i]->kill();
        }
        MR::invalidateShadow(this, nullptr);
    }
    s32 frame = MR::getBckFrameMax(this);

    if (MR::isOnGround(this)) {
        mVelocity.zero();
    }
    else {
        MR::moveAndTurnToPlayer(this, &_B0, hOnAirParam[0], hOnAirParam[1], hOnAirParam[2], hOnAirParam[3]);
    }

    if (!MR::isHiddenModel(this) && MR::isGreaterStep(this, frame - 37)) {
        MR::hideModelAndOnCalcAnim(this);
    }

    if (!MR::isHiddenModel(_AC) && MR::isActionEnd(_AC)) {
        MR::hideModel(_AC);
    }

    if (MR::isActionEnd(this) && MR::isHiddenModel(this) && MR::isHiddenModel(_AC)) {
        MR::endDemo(this, "メラキンオープニング");
    }

    if (!MR::isDemoActive()) {
        kill();
        _AC->kill();
        MR::requestAppearPowerStar(this, mPosition);        
    }
}

void IceMerameraKing::attackSensor(HitSensor *pSender, HitSensor *pReceiver) {
    if (pSender == getSensor("body") && MR::isSensorPlayer(pReceiver)) {
        if (isNerve(&NrvIceMerameraKing::HostTypeNrvDamage::sInstance)
        || isNerve(&NrvIceMerameraKing::HostTypeNrvExtinguish::sInstance)
        || isNerve(&NrvIceMerameraKing::HostTypeNrvEscape::sInstance)
        || isNerve(&NrvIceMerameraKing::HostTypeNrvEscapeJump::sInstance)
        || isNerve(&NrvIceMerameraKing::HostTypeNrvPreRecover::sInstance)
        || isNerve(&NrvIceMerameraKing::HostTypeNrvRecover::sInstance)
        || isNerve(&NrvIceMerameraKing::HostTypeNrvDeathDemoWait::sInstance)
        || isNerve(&NrvIceMerameraKing::HostTypeNrvAngryDemo1st::sInstance)
        || isNerve(&NrvIceMerameraKing::HostTypeNrvAngryDemo2nd::sInstance)
        || isNerve(&NrvIceMerameraKing::HostTypeNrvDeathDemo::sInstance)
        || MR::isPlayerElementModeIce()) {
            MR::sendMsgPush(pReceiver, pSender);
        }
        else {
            MR::sendMsgEnemyAttackFreeze(pReceiver, pSender);
        }
    }
}

bool IceMerameraKing::receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
    if (MR::isMsgStarPieceReflect(msg)) {
        return true;
    }

    if (!isNerve(&NrvIceMerameraKing::HostTypeNrvEscape::sInstance) && !isNerve(&NrvIceMerameraKing::HostTypeNrvEscapeJump::sInstance)) {
        return false;
    }

    if (MR::isMsgFreezeAttack(msg)) {
        if (!--_EC) {
            MR::stopMarioPinchBGMSoon();
            MR::requestStartTimeKeepDemo(this, "メラキンオープニング", &NrvIceMerameraKing::HostTypeNrvDeathDemo::sInstance, &NrvIceMerameraKing::HostTypeNrvDeathDemoWait::sInstance, "死亡デモ");            
        }
        else {
            setNerve(&NrvIceMerameraKing::HostTypeNrvDamage::sInstance);
        }
        return true;
    }
    else if (MR::isMsgPlayerSpinAttack(msg)) {
        if (!--_EC) {
            MR::requestStartTimeKeepDemo(this, "メラキンオープニング", &NrvIceMerameraKing::HostTypeNrvDeathDemo::sInstance, &NrvIceMerameraKing::HostTypeNrvDeathDemoWait::sInstance, "死亡デモ");
        }
        else {
            setNerve(&NrvIceMerameraKing::HostTypeNrvDamage::sInstance);            
        }
        return true;
    }
    else {
        return false;
    }
}

bool IceMerameraKing::receiveOtherMsg(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
    if (MR::isInSpinStormRange(msg, pSender, pReceiver, 600.0f)) {
        if (isNerve(&NrvIceMerameraKing::HostTypeNrvDeathDemo::sInstance)
        || isNerve(&NrvIceMerameraKing::HostTypeNrvExtinguish::sInstance)
        || isNerve(&NrvIceMerameraKing::HostTypeNrvDamage::sInstance)
        || isNerve(&NrvIceMerameraKing::HostTypeNrvAngryDemo1st::sInstance)
        || isNerve(&NrvIceMerameraKing::HostTypeNrvAngryDemo2nd::sInstance)
        || isNerve(&NrvIceMerameraKing::HostTypeNrvEscape::sInstance)
        || isNerve(&NrvIceMerameraKing::HostTypeNrvEscapeJump::sInstance)
        || isNerve(&NrvIceMerameraKing::HostTypeNrvPreRecover::sInstance)
        || isNerve(&NrvIceMerameraKing::HostTypeNrvRecover::sInstance)) {
            return false;
        }
        else {
            TVec3f v10(*MR::getPlayerCenterPos());
            v10.sub(mPosition);
            MR::vecKillElement(v10, mGravity, &v10);
            if (PSVECMag(&v10) > getSensor("body")->mRadius) {
                return false;
            }
            else {
                setNerve(&NrvIceMerameraKing::HostTypeNrvExtinguish::sInstance);
                return true;
            }
        }
    }
    else {
        return false;
    }
}

/*
void IceMerameraKing::addVelocityToInitPos() {
    TVec3f v12(_C8);
    v12.sub(mPosition);
    
    if (0.0f < mGravity.dot(v12)) {
        MR::vecKillElement(v12, mGravity, &v12);
    }
    f32 squared = v12.squaredInline();
    f32 half = 0.5f;

    if (squared <= 0.0000038146973f) {
        squared = squared;
    }
    else {
        f32 inv = JGeometry::TUtil<f32>::inv_sqrt(squared);
        f32 v9 = inv * half;
        v12.scale(v9);
    }
    mVelocity.add(v12);
}
*/

void IceMerameraKing::calcAndSetBaseMtx() {
    TPos3f v3;
    MR::makeMtxUpFrontPos(&v3, _BC, _B0, mPosition);
    MR::setBaseTRMtx(this, v3);
    mFixedPos->calc();
    mJointController->registerCallBack();
}

bool IceMerameraKing::isDeadAllIce() {
    s32 temp = _A4;
    for (s32 i = 0; i < temp; i++) {
        if (!MR::isDead(mActor[i])) {
            return false;
        }
    }
    return true;
}

bool IceMerameraKing::isEnableThrow() {
    if (MR::isValidSwitchA(this) && !MR::isOnSwitchA(this)) {
        return false;
    }

    if (MR::isPlayerInWaterMode() || MR::isPlayerOnWaterSurface()) {
        return false;
    }
    TVec3f v10(*MR::getPlayerCenterPos());
    v10.sub(mPosition);
    MR::normalize(&v10);
    TVec3f v9;
    MR::calcUpVec(&v9, this);
    bool flag = false;
    flag = -0.60000002f < v9.dot(v10);
    return flag;
}

ThrowingIce* IceMerameraKing::getDeadWeaponAndAppear() {
    ThrowingIce* v7;
    s32 tempE4 = _E4;    
    s32 temp = _A4;
    for (s32 i = 0; i < temp; i++) {
        if (MR::isDead(mActor[tempE4])) {
            MR::resetPosition(mActor[tempE4], mPosition);
            mActor[tempE4]->appear();
            _E4 += temp + 1 - ((_E4 + temp + 1) / (temp * temp));
            v7 = mActor[tempE4];
            return v7;
        }
        tempE4 += temp + 1 - ((tempE4 + temp + 1) / (temp * temp));
    }
    v7 = nullptr;
    return v7;
    //major
}

/*
bool IceMerameraKing::calcJoint(TPos3f *vec, const JointControllerInfo &info) {
    TMtx34f v16;
    TVec3f v15;
    v16.mMtx[0][3] = 0.0f;
    v16.mMtx[1][3] = 0.0f;
    v16.mMtx[2][3] = 0.0f;
    f32 v3 = _100.y;
    f32 v4 = _100.x;
    f32 v5 = 2.0f * _100.y;    
    f32 v6 = _100.z;    

    f32 v7 = 2.0f * _100.x;
    f32 v8 = 2.0f * _100.w;
    f32 v9 = ((1.0f - ((2.0f * _100.x) * _100.x)) - ((2.0f * _100.z) * _100.z));
    f32 v10 = (((2.0f * _100.x) * _100.y) + ((2.0f * _100.w) * _100.z));
    v16.mMtx[0][0] = (1.0f - ((2.0f * _100.y) * _100.y)) - ((2.0f * _100.z) * _100.z);
    f32 v11 = ((1.0f - (v7 * v4)) - (v5 * v3));
    v16.mMtx[0][1] = (v7 * v3) - (v8 * v6);
    f32 v12 = (v8 * v3);
    v16.mMtx[1][1] = v9;
    f32 v13 = (v8 * v4);
    v16.mMtx[1][0] = v10;
    v16.mMtx[2][2] = v11;
    v16.mMtx[0][2] = (v7 * v6) + v12;
    v16.mMtx[1][2] = (v5 * v6) - v13;
    v16.mMtx[2][0] = (v7 * v6) - v12;
    v16.mMtx[2][1] = (v5 * v6) + v13;
    vec->getTrans(v15);
    mPosition.x = 0.0f;
    mRotation.y = 0.0f;
    mScale.z = 0.0f;
    vec->concat(v16, *vec);
    mPosition.x = v15.x;
    mRotation.y = v15.y;
    mScale.z = v15.z;
    return true;
}
*/

IceMerameraKing::~IceMerameraKing() {
    
}

IceMerameraKingShockWave::IceMerameraKingShockWave() : ModelObj("衝撃", "IceMerameraKingShock", nullptr, -2, -2, -2, false) {
    initHitSensor(2);
    MR::addHitSensorEnemyAttack(this, "circle", 16, 15.0f, TVec3f(0.0f, -100.0f, 0.0f));
    MR::addHitSensorEnemyAttack(this, "circle_end", 16, 24.0f, TVec3f(0.0f, -100.0f, 0.0f));
}

void IceMerameraKingShockWave::appear() {
    LiveActor::appear();
    MR::startAction(this, "Shock");
    HitSensor* sensor = getSensor("circle");
    sensor->mRadius = 0.0f;
}

void IceMerameraKingShockWave::control() {
    TVec3f v7;
    MR::copyJointScale(this, hScaleJointName[2], &v7);
    f32 v3 = 15.0f * v7.y;
    getSensor("circle")->mRadius = v3;
    MR::copyJointScale(this, hScaleJointName[0], &v7);
    f32 v4 = 25.0f * v7.y;
    getSensor("circle_end")->mRadius = v4;
    if (MR::isActionEnd(this)) {
        kill();
    }
}

void IceMerameraKingShockWave::attackSensor(HitSensor *pSender, HitSensor *pReceiver) {
    if (pSender != getSensor("circle_end") && MR::isSensorPlayer(pReceiver)) {
        HitSensor* sensor = getSensor("circle_end");
        TVec3f v17(sensor->mPosition);
        v17.sub(pSender->mPosition);
        f32 radius1 = sensor->mRadius;
        f32 radius2 = pReceiver->mRadius;
        if (PSVECMag(&v17) > (radius2 + radius1)) {
            TVec3f v15;
            TVec3f v16(pSender->mPosition);
            v16.sub(pSender->mPosition);
            MR::calcUpVec(&v15, this);
            if (!(__fabsf(v16.dot(v15)) < 200.0f)) {
                MR::vecKillElement(v16, v15, &v16);
                MR::sendMsgEnemyAttackFlipToDir(pReceiver, pSender, v16);
            }
        }
    }
}

IceMerameraKingShockWave::~IceMerameraKingShockWave() {

}

namespace NrvIceMerameraKing {
    INIT_NERVE(HostTypeNrvSearch);
    INIT_NERVE(HostTypeNrvBeginDemo);
    INIT_NERVE(HostTypeNrvThrow);
    INIT_NERVE(HostTypeNrvThrow2nd);
    INIT_NERVE(HostTypeNrvExtinguish);
    INIT_NERVE(HostTypeNrvEscape);        
    INIT_NERVE(HostTypeNrvEscapeJump);
    INIT_NERVE(HostTypeNrvDamage);
    INIT_NERVE(HostTypeNrvPreRecover);
    INIT_NERVE(HostTypeNrvRecover);
    INIT_NERVE(HostTypeNrvPreAttack);
    INIT_NERVE(HostTypeNrvAttack);
    INIT_NERVE(HostTypeNrvAttackAfter);
    INIT_NERVE(HostTypeNrvAngryDemo1st);
    INIT_NERVE(HostTypeNrvAngryDemo2nd);     
    INIT_NERVE(HostTypeNrvDeathDemoWait);          
    INIT_NERVE(HostTypeNrvDeathDemo);        

	void HostTypeNrvSearch::execute(Spine *pSpine) const {
		IceMerameraKing *pActor = (IceMerameraKing*)pSpine->mExecutor;
		pActor->exeSearch();
	}    

	void HostTypeNrvBeginDemo::execute(Spine *pSpine) const {
		IceMerameraKing *pActor = (IceMerameraKing*)pSpine->mExecutor;
		pActor->exeThrow();
	}    

	void HostTypeNrvBeginDemo::executeOnEnd(Spine *pSpine) const {
		IceMerameraKing *pActor = (IceMerameraKing*)pSpine->mExecutor;
		pActor->tearDownThrow();
	}        
	void HostTypeNrvThrow::execute(Spine *pSpine) const {
		IceMerameraKing *pActor = (IceMerameraKing*)pSpine->mExecutor;
		pActor->exeThrow();
	}    
    
	void HostTypeNrvThrow::executeOnEnd(Spine *pSpine) const {
		IceMerameraKing *pActor = (IceMerameraKing*)pSpine->mExecutor;
		pActor->tearDownThrow();
	}    

	void HostTypeNrvThrow2nd::execute(Spine *pSpine) const {
		IceMerameraKing *pActor = (IceMerameraKing*)pSpine->mExecutor;
		pActor->exeThrow();
	}        

    void HostTypeNrvThrow2nd::executeOnEnd(Spine *pSpine) const {
		IceMerameraKing *pActor = (IceMerameraKing*)pSpine->mExecutor;
		pActor->tearDownThrow();
	}    
    
	void HostTypeNrvExtinguish::execute(Spine *pSpine) const {
		IceMerameraKing *pActor = (IceMerameraKing*)pSpine->mExecutor;
		pActor->exeExtinguish();
	}    

	void HostTypeNrvEscape::execute(Spine *pSpine) const {
		IceMerameraKing *pActor = (IceMerameraKing*)pSpine->mExecutor;
		pActor->exeEscape();
	}        

    void HostTypeNrvEscape::executeOnEnd(Spine *pSpine) const {
		IceMerameraKing *pActor = (IceMerameraKing*)pSpine->mExecutor;
		pActor->exeDeleteEffect();
	}    
    
	void HostTypeNrvEscapeJump::execute(Spine *pSpine) const {
		IceMerameraKing *pActor = (IceMerameraKing*)pSpine->mExecutor;
		pActor->exeEscapeJump();
	}    

	void HostTypeNrvDamage::execute(Spine *pSpine) const {
		IceMerameraKing *pActor = (IceMerameraKing*)pSpine->mExecutor;
		pActor->exeDamage();
	}        
    	void HostTypeNrvPreRecover::execute(Spine *pSpine) const {
		IceMerameraKing *pActor = (IceMerameraKing*)pSpine->mExecutor;
		pActor->exePreRecover();
	}    
    
	void HostTypeNrvRecover::execute(Spine *pSpine) const {
		IceMerameraKing *pActor = (IceMerameraKing*)pSpine->mExecutor;
		pActor->exeRecover();
	}    

	void HostTypeNrvPreAttack::execute(Spine *pSpine) const {
		IceMerameraKing *pActor = (IceMerameraKing*)pSpine->mExecutor;
		pActor->exePreAttack();
	}        
    	void HostTypeNrvAttack::execute(Spine *pSpine) const {
		IceMerameraKing *pActor = (IceMerameraKing*)pSpine->mExecutor;
		pActor->exeAttack();
	}    
    
	void HostTypeNrvAttackAfter::execute(Spine *pSpine) const {
		IceMerameraKing *pActor = (IceMerameraKing*)pSpine->mExecutor;
		pActor->exeAttackAfter();
	}    

	void HostTypeNrvAngryDemo1st::execute(Spine *pSpine) const {
		IceMerameraKing *pActor = (IceMerameraKing*)pSpine->mExecutor;
		pActor->exeAngryDemo();
	}      
      
	void HostTypeNrvAngryDemo2nd::execute(Spine *pSpine) const {
		IceMerameraKing *pActor = (IceMerameraKing*)pSpine->mExecutor;
		pActor->exeAngryDemo();
	}            

	void HostTypeNrvDeathDemoWait::execute(Spine *pSpine) const {
		IceMerameraKing *pActor = (IceMerameraKing*)pSpine->mExecutor;
		pActor->exeDeathDemoWait();
	}    

	void HostTypeNrvDeathDemo::execute(Spine *pSpine) const {
		IceMerameraKing *pActor = (IceMerameraKing*)pSpine->mExecutor;
		pActor->exeDeathDemo();
	}                

    TVec3f hBinderOffset(-100.586f, 222.65601f, -91.796898f);      
    TVec3f hEscapeBinderOffset(-39.0625f, 91.796898f, -47.851601f);        
};
