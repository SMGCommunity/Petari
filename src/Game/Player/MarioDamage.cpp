#include "Game/Player/MarioDamage.hpp"
#include "Game/Enemy/KarikariDirector.hpp"
#include "Game/Map/CollisionCode.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioAbyssDamage.hpp"
#include "Game/Player/MarioAccess.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioBlown.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioDarkDamage.hpp"
#include "Game/Player/MarioFaint.hpp"
#include "Game/Player/MarioFireDamage.hpp"
#include "Game/Player/MarioFireDance.hpp"
#include "Game/Player/MarioFireRun.hpp"
#include "Game/Player/MarioFreeze.hpp"
#include "Game/Player/MarioMapCode.hpp"
#include "Game/Player/MarioParalyze.hpp"
#include "Game/Player/MarioSwim.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/SequenceUtil.hpp"

bool Mario::isDamaging() const {
    if (isAnimationRun("水上ダメージ中")) {
        return true;
    }
    if (_41E) {
        return true;
    }
    if (mMovementStates._1B) {
        return true;
    }
    if (mMovementStates._27) {
        return true;
    }
    if (mMovementStates._2C) {
        return true;
    }
    if (_10.jumping) {
        return true;
    }
    if (_10._14) {
        return true;
    }
    if (_10._18) {
        return true;
    }

    switch (getCurrentStatus()) {
    case MarioStatus_FireDamage:
    case MarioStatus_FireDance:
    case MarioStatus_FireRun:
    case MarioStatus_Paralyze:
    case MarioStatus_AbyssDamage:
    case MarioStatus_Freeze:
    case MarioStatus_Crush:
        return true;
    case MarioStatus_Damage:
        return mDamage->_12;
    case MarioStatus_Faint:
        return mFaint->mTookDamage;
    default:
        return false;
    }
}

bool Mario::damageLarge(const TVec3f& direction) {
    if (damage(direction)) {
        if (isStatusActive(MarioStatus_Swim)) {
            mSwim->_AD = 1;
        } else {
            mDamage->setVecSize(mActor->getConst().getTable()->mJumpDistLargeDamage, mActor->getConst().getTable()->mJumpHeightLargeDamage);
            playSound("投げられ");
        }
        return true;
    }
    return false;
}

void Mario::decDamageAfterTimer() {
    MarioDamage* damage = mDamage;
    if (damage->_16) {
        damage->_16--;
    }
    MarioFaint* faint = mFaint;
    if (faint->_14) {
        faint->_14--;
    }
    MarioParalyze* paralyze = mParalyze;
    if (paralyze->_16) {
        paralyze->_16--;
    }
    MarioFreeze* freeze = mFreeze;
    if (freeze->_1C) {
        freeze->_1C--;
    }
    mFireDamage->decAfterTimer();
    if (_41E) {
        _41E--;
    }
}

bool Mario::checkDamage() {
    decDamageAfterTimer();
    if (mMovementStates._1F) {
        return false;
    }
    if (mActor->_EA4) {
        return false;
    }
    if (MR::isDemoActive()) {
        return false;
    }
    if (isStatusActive(MarioStatus_Talk)) {
        return false;
    }
    if (isStatusActive(MarioStatus_Recovery) || isInvincible()) {
        mMovementStates._1B = false;
        mMovementStates._27 = false;
        mMovementStates._2C = false;
        _10.jumping = false;
        _10._14 = false;
        _10._18 = false;
        mFaint->mNoDamage = false;
        return false;
    }
    if (isStatusActive(MarioStatus_Swim)) {
        checkWaterDamage();
        return false;
    }

    if (mMovementStates._1B) {
        mMovementStates._1B = false;
        changeStatus(mDamage);
        mMovementStates._27 = false;
        mFaint->mNoDamage = false;
        mMovementStates._2C = false;
        return true;
    }
    if (mMovementStates._27) {
        mMovementStates._27 = false;
        mMovementStates._2C = false;
        changeStatus(mFaint);
        return true;
    }
    if (mMovementStates._2C) {
        mMovementStates._2C = false;
        changeStatus(mBlown);
        return true;
    }
    if (_10.jumping) {
        _10.jumping = false;
        doFireDanceWithInitialDamage(1);
        return true;
    }
    if (_10._14) {
        _10._14 = false;
        doParalyze();
        return true;
    }
    if (_10._18) {
        _10._18 = false;
        if (tryCrush()) {
            return true;
        }
    }
    checkKarikariDamage();
    return false;
}

u16 Mario::getDamageAfterTimer() const {
    u16 timer = mDamage->_16;
    if (timer < mFaint->_14) {
        timer = mFaint->_14;
    }
    if (timer < mParalyze->_16) {
        timer = mParalyze->_16;
    }
    if (timer < mFreeze->_1C) {
        timer = mFreeze->_1C;
    }
    if (timer < mFireDamage->_12) {
        timer = mFireDamage->_12;
    }
    if (timer < _41E) {
        timer = _41E;
    }
    return timer;
}

bool Mario::damageFloorCheck() {
    if (mMovementStates._1F) {
        return false;
    }
    if (_1C._16) {
        return false;
    }
    switch (_960) {
    case CollisionFloorCode_Needle:
        if (checkCurrentFloorCodeSevere(CollisionFloorCode_Needle) && doNeedleWithInitialDamage(mGroundPolygon)) {
            return true;
        }
        break;
    case 0x81:
        if (checkCurrentFloorCodeSevere(0x81) && doFireDanceWithInitialDamage(1)) {
            return true;
        }
        break;
    case CollisionFloorCode_Death:
        MarioAccess::forceKill(3, 0);
        return true;
    case CollisionFloorCode_DamageFire:
        if (checkCurrentFloorCodeSevere(CollisionFloorCode_DamageFire) && doFireDanceWithInitialDamage(1)) {
            return true;
        }
        break;
    case CollisionFloorCode_DamageNormal:
        if (isDamaging()) {
            return false;
        }
        if (damage(_368 * 10.0f)) {
            return true;
        }
        break;
    case CollisionFloorCode_DamageElectric:
        if (doParalyze()) {
            return true;
        }
        break;
    case CollisionFloorCode_PullBack:
        if (doRecovery()) {
            return true;
        }
        break;
    }
    return false;
}

bool Mario::damageWallCheck() {
    if (mMovementStates._1F) {
        return false;
    }
    TVec3f normal;
    if (checkWallCodeNorm(CollisionWallCode_Rebound, &normal, false)) {
        return doFlipJump(normal * 5.0f);
    }
    if (checkWallFloorCode(CollisionFloorCode_Death)) {
        mActor->forceKill(3);
        return true;
    }
    if (checkWallFloorCode(CollisionFloorCode_DamageFire) && doFireDanceWithInitialDamage(1)) {
        return true;
    }
    if (checkWallFloorCode(CollisionFloorCode_Needle) && doNeedleWithInitialDamage(1)) {
        return true;
    }
    if (checkWallFloorCode(CollisionFloorCode_DamageElectric) && doParalyze()) {
        return true;
    }
    if (checkWallFloorCode(CollisionFloorCode_PullBack) && doRecovery()) {
        return true;
    }
    if (checkWallFloorCode(CollisionFloorCode_DamageNormal) && damage(getWallNorm() * 10.0f)) {
        return true;
    }
    return false;
}

bool Mario::damagePolygonCheck(const Triangle* triangle) {
    if (mMovementStates._1F) {
        return false;
    }
    switch (_95C->getCode(triangle)) {
    case 0x81:
        if (doFireDanceWithInitialDamage(1)) {
            return true;
        }
        break;
    case CollisionFloorCode_Death:
        MarioAccess::forceKill(3, 0);
        return true;
    case CollisionFloorCode_DamageFire:
        if (doFireDanceWithInitialDamage(1)) {
            return true;
        }
        break;
    case CollisionFloorCode_DamageNormal:
        mSwim->addDamage(*MR::getNormal(triangle) * 10.0f);
        return true;
    case CollisionFloorCode_DamageElectric:
        if (doParalyze()) {
            return true;
        }
        break;
    case CollisionFloorCode_PullBack:
        if (doRecovery()) {
            return true;
        }
        break;
    case CollisionFloorCode_Needle:
        doNeedleWithInitialDamage(triangle);
        return true;
    case CollisionFloorCode_SinkDeath:
        return true;
    }
    return false;
}

bool Mario::flipLarge(const TVec3f& direction) {
    if (damage(direction)) {
        if (isStatusActive(MarioStatus_Swim)) {
            mSwim->_AD = 1;
            mSwim->mDamageType = 1;
        } else {
            mDamage->_11 = 1;
        }
        mDamage->setVecSize(direction.length(), 0.0f);
        return true;
    }
    return false;
}

bool Mario::isEnableAddDamage() const {
    if (getCurrentStatus() == MarioStatus_Talk) {
        return false;
    }
    if (isDamaging()) {
        return false;
    }
    if (mActor->_390) {
        return false;
    }
    if (isInvincible()) {
        return false;
    }
    return getDamageAfterTimer() == 0;
}

bool Mario::damage(const TVec3f& direction) {
    _7C4 = direction;
    if (!isEnableAddDamage()) {
        return false;
    }
    if (mFaint->_14 || mDamage->_16 || mMovementStates._1B) {
        return false;
    }
    if (mMovementStates._F) {
        forceStopTornado();
    }
    mDamage->setVec(direction);
    stopWalk();
    forceStopTornado();
    mActor->damageDropThrowMemoSensor();
    if (isStatusActive(MarioStatus_Damage)) {
        closeStatus(mDamage);
    }
    mMovementStates._1B = true;
    return true;
}

MarioDamage::MarioDamage(MarioActor* actor) : MarioState(actor, MarioStatus_Damage) {
    _11 = 0;
    _12 = 0;
    _14 = 0;
    _16 = 0;
    _18 = 0;
    _1C.zero();
    _28 = nullptr;
    _2C = nullptr;
}

bool MarioDamage::start() {
    _14 = 0;
    _18 = 0;
    if (_1C.dot(getPlayer()->mFrontVec) > 0.0f) {
        changeAnimationNonStop("中後ダメージ");
        _28 = "中後ダメージ空中";
        _2C = "中後ダメージ着地";
        getPlayer()->setFrontVecKeepUp(_1C);
    } else {
        changeAnimationNonStop("中ダメージ");
        _28 = "中ダメージ空中";
        _2C = "中ダメージ着地";
        getPlayer()->setFrontVecKeepUp(-_1C);
    }
    if (!_11) {
        playEffect("ダメージ");
    }
    startPadVib(3);
    getPlayer()->mMovementStates._1 = false;
    getPlayer()->mMovementStates.jumping = true;
    getPlayer()->mMovementStates._B = false;
    getPlayer()->mMovementStates._3E = 0;
    _1C += -mActor->_240 * mActor->getConst().getTable()->mJumpHeightDamage;
    getPlayer()->mJumpVec = _1C;
    addVelocity(_1C);

    _12 = !_11;
    if (_11) {
        playSound("声投げられ");
        playSound("投げられ");
        _11 = 0;
        mActor->resetPlayerModeOnNoDamage();
    } else {
        playSound("声小ダメージ");
        playSound("ダメージ");
        mActor->decLifeMiddle();
        mActor->resetPlayerModeOnDamage();
    }
    return true;
}

void MarioDamage::setVec(const TVec3f& direction) {
    MR::vecKillElement(direction, mActor->_240, &_1C);
    _1C.setLength(mActor->getConst().getTable()->mJumpDistDamage);
}

void MarioDamage::setVecSize(f32 horizontal, f32 vertical) {
    _1C.setLength(horizontal);
    _1C += -mActor->_240 * vertical;
}

void MarioDamage::stopHead(const TVec3f& normal) {
    if (!_18) {
        TVec3f horizontal;
        f32 verticalSpeed = MR::vecKillElement(_1C, mActor->_240, &horizontal);
        f32 towardWall = MR::vecKillElement(horizontal, normal, &_1C);
        _1C += mActor->_240 * verticalSpeed;
        if (towardWall < 0.0f) {
            _1C += normal * -towardWall * 0.5f;
        }
    } else {
        TVec3f horizontal;
        MR::vecKillElement(normal, getAirGravityVec(), &horizontal);
        if (!MR::normalizeOrZero(&horizontal)) {
            f32 towardWall = MR::vecKillElement(_1C, normal, &_1C);
            if (towardWall < 0.0f) {
                _1C += normal * -towardWall * 0.5f;
            }
        }
    }
}

bool MarioDamage::update() {
    _14++;
    if (mActor->_EA4) {
        return true;
    }
    switch (_18) {
    case 0:
        addVelocity(_1C);
        _1C += mActor->_240 * mActor->getConst().getTable()->mGravityDamage;
        if (_14 > 20) {
            if (_28) {
                changeAnimation(_28, static_cast< const char* >(nullptr));
            }
            if (getPlayer()->_1C._0) {
                f32 vertical = MR::vecKillElement(_1C, getAirGravityVec(), &_1C);
                _1C.x *= 0.95f;
                _1C.y *= 0.95f;
                _1C.z *= 0.95f;
                _1C += getAirGravityVec() * vertical;
            }
        }
        if (getPlayer()->mMovementStates._1) {
            if (getPlayer()->mVerticalSpeed > 30.0f && mActor->selectDamagePop(getSensor(getGroundPolygon()))) {
                _1C += getPlayer()->_368 * 20.0f;
                getPlayer()->mMovementStates._1 = false;
                getPlayer()->mMovementStates.jumping = true;
            } else {
                getPlayer()->mMovementStates.jumping = false;
                playSound("吹っ飛び倒れ");
                changeAnimation(_2C, static_cast< const char* >(nullptr));
                playEffect("共通ダメージ着地");
                MR::vecKillElement(_1C, mActor->_240, &_1C);
                _14 = 0;
                _18++;
                if (!mActor->mHealth) {
                    _18 = 2;
                }
            }
        } else if (!mActor->mHealth) {
            if (_14 > 240) {
                mActor->forceGameOverAbyss();
            }
        } else if (_14 > 360) {
            mActor->forceGameOverAbyss();
        }
        break;
    case 1:
        if (!getPlayer()->mMovementStates._1) {
            getPlayer()->mMovementStates.jumping = true;
            _18 = 0;
            break;
        }
        MR::vecKillElement(_1C, getAirGravityVec(), &_1C);
        addVelocity(_1C);
        _1C.x *= 0.95f;
        _1C.y *= 0.95f;
        _1C.z *= 0.95f;
        if (!isAnimationRun(_2C)) {
            return false;
        }
        if (_14 > 15 && checkTrgA()) {
            getPlayer()->tryJump();
            return false;
        }
        break;
    case 2:
        if (!getPlayer()->mMovementStates._1) {
            getPlayer()->mMovementStates.jumping = true;
            _18 = 0;
            break;
        }
        if (_14 == 40) {
            if (!mActor->mHealth) {
                mActor->forceGameOver();
            } else {
                return false;
            }
        }
        break;
    }
    getPlayer()->mJumpVec = _1C;
    return true;
}

bool MarioDamage::close() {
    stopAnimation("ダメージ");
    stopAnimation("ダメージ着地", "基本");
    if (_12) {
        _16 = 120;
    }
    return true;
}

bool MarioDamage::notice() {
    if (!mActor->mHealth) {
        if (getNoticedStatus() == MarioStatus_Swim) {
            mActor->forceGameOver();
        }
        return true;
    }
    return false;
}

MarioFireDamage::MarioFireDamage(MarioActor* actor) : MarioState(actor, MarioStatus_FireDamage) {
    _12 = 0;
}

void MarioFireDamage::decAfterTimer() {
    if (_12 && !isStatusActiveID(MarioStatus_FireDance) && !isStatusActiveID(MarioStatus_FireRun)) {
        _12--;
    }
}

bool Mario::doAbyssDamage() {
    if (getCurrentStatus() == MarioStatus_AbyssDamage) {
        return false;
    }
    stopWalk();
    mActor->damageDropThrowMemoSensor();
    MR::removeAllClingingKarikari();
    mActor->_A6E = 0;
    changeStatus(mAbyssDamage);
    return true;
}

MarioAbyssDamage::MarioAbyssDamage(MarioActor* actor) : MarioState(actor, MarioStatus_AbyssDamage) {
    _12 = 0;
    _14 = 0;
    _18.zero();
}

bool MarioAbyssDamage::start() {
    _12 = 0;
    _14 = 0;
    mActor->forceGameOverAbyss();
    return false;
}

bool MarioAbyssDamage::update() {
    addTrans(_18, "Module");
    switch (_14) {
    case 0:
        _14++;
        _12 = 120;
        MR::requestStartGameOverDemo();
        break;
    case 1:
        if (_12) {
            _12--;
        }
        if (!_12) {
            mActor->forceGameOverAbyss();
            return false;
        }
        break;
    }
    return true;
}

bool MarioAbyssDamage::close() {
    return true;
}

void Mario::connectToFireRun() {
    if (mActor->mHealth) {
        changeStatus(mFireRun);
        stopJump();
        mFireRun->_12 = 1;
    }
}

MarioFireRun::MarioFireRun(MarioActor* actor) : MarioState(actor, MarioStatus_FireRun) {
    _12 = 0;
    _14 = 0;
    _18 = 0.0f;
}

bool MarioFireRun::start() {
    _12 = mActor->getConst().getTable()->mFireRunTimer1;
    _14 = 0;
    stopAnimationUpper(nullptr, nullptr);
    changeAnimation("ファイアラン前兆", static_cast< const char* >(nullptr));
    if (!getPlayer()->mMovementStates._1) {
        _18 = -mActor->getConst().getTable()->mFireRunFirstJump;
    } else {
        _18 = 0.0f;
    }
    return true;
}

bool MarioFireRun::move() {
    if (getStickX() != 0.0f || getStickY() != 0.0f) {
        const TVec3f& padDir = getWorldPadDir();
        getPlayer()->setFrontVecKeepUp(padDir, mActor->getConst().getTable()->mFireRunTurnRatio);
    }
    if (getPlayer()->checkTrgA() || mActor->isRequestJump()) {
        Mario* player = getPlayer();
        player->mWalkSpeed = 1.0f;
        getPlayer()->tryJump();
        return false;
    }
    return true;
}

bool MarioFireRun::update() {
    switch (_14) {
    case 0:
        if (!getPlayer()->mMovementStates._1) {
            getPlayer()->mJumpVec = getAirGravityVec() * _18;
            addVelocity(getAirGravityVec() * _18);
            _18 += mActor->getConst().getTable()->mFireRunGravity;
            if (_18 > 50.0f) {
                _18 = 50.0f;
            }
            const TVec3f& velocity = mActor->_288;
            if (velocity.dot(getAirGravityVec()) < -10.0f) {
                _18 = 0.0f;
                addVelocity(getFrontVec() * 5.0f);
            }
        } else if (_12) {
            _12--;
        } else {
            _14++;
            _12 = mActor->getConst().getTable()->mFireRunTimer2;
            if (!mActor->mHealth) {
                _12 >>= 1;
            }
            _18 = 0.0f;
            changeAnimation("炎のランナー", static_cast< const char* >(nullptr));
        }
        break;
    case 1:
        playSound("炎ダメージ炎上中");
        if (!getPlayer()->mMovementStates._1) {
            _14 = 2;
            _12 += mActor->getConst().getTable()->mFireRunTimer3;
        }
        addVelocity(getFrontVec() * mActor->getConst().getTable()->mFireRunSpeed);
        if (_12) {
            _12--;
        }
        if (!_12) {
            _12 = mActor->getConst().getTable()->mFireRunTimer3;
            _14++;
        }
        return move();
    case 2:
        if (mActor->isEnableNerveChange() && getStickP() > 0.7f) {
            return false;
        }
        if (!getPlayer()->mMovementStates._1) {
            getPlayer()->mJumpVec = getAirGravityVec() * _18;
            addVelocity(getAirGravityVec() * _18);
            _18 += mActor->getConst().getTable()->mFireRunGravity;
            if (_18 > 50.0f) {
                _18 = 50.0f;
            }
            const TVec3f& velocity = mActor->_288;
            if (velocity.dot(getAirGravityVec()) < -10.0f) {
                _18 = 0.0f;
                addVelocity(getFrontVec() * 5.0f);
            }
            if (_12) {
                _12--;
            }
            break;
        }
        if (_12 > mActor->getConst().getTable()->mFireRunTimer3) {
            addVelocity(getFrontVec() * mActor->getConst().getTable()->mFireRunSpeed);
        } else {
            addVelocity(getFrontVec() * mActor->getConst().getTable()->mFireRunSpeed * _12 / mActor->getConst().getTable()->mFireRunTimer3);
        }
        if (mActor->isEnableNerveChange()) {
            if (_12) {
                _12--;
            }
            if (!_12) {
                return false;
            }
        }
        return move();
    }
    return true;
}

bool MarioFireRun::close() {
    if (!mActor->mHealth) {
        mActor->forceGameOver();
    }
    if (getPlayer()->mMovementStates.jumping) {
        stopAnimation("炎のランナー", "落下");
    } else {
        stopAnimation("炎のランナー", "基本");
        if (getStickP() < 0.1f) {
            playSound("声炎ダメージ終了");
        }
    }
    stopEffect("炎ダメージ煙");
    stopEffect("炎ダメージ青煙");
    mActor->_1B4 = 0;
    return true;
}

bool MarioFireRun::notice() {
    return false;
}

bool Mario::doFireDanceWithInitialDamage(u8 amount) {
    if (mMovementStates._1F) {
        return false;
    }
    bool started = doFireDance();
    if (started) {
        for (u32 i = 0; i < amount; i++) {
            mActor->decLife(0);
        }
        if (!mActor->mHealth) {
            mActor->forceGameOverNonStop();
        }
    }
    return started;
}

bool Mario::doFireObjHitWithInitialDamage() {
    bool result;
    if (isEnableAddDamage()) {
        result = doFireDanceWithInitialDamage(1);
    } else {
        result = false;
    }
    return result;
}

bool Mario::doNeedleWithInitialDamage(u8 amount) {
    if (mMovementStates._1F) {
        return false;
    }
    if (getPlayerMode() == PlayerMode_Teresa) {
        doTeresaReflection(getWallNorm(), false);
        return false;
    }
    bool started = doNeedle(nullptr);
    if (started) {
        for (u32 i = 0; i < amount; i++) {
            mActor->decLife(0);
        }
        if (!mActor->mHealth) {
            mActor->forceGameOverNonStop();
        }
    }
    return started;
}

bool Mario::doNeedleWithInitialDamage(const Triangle* triangle) {
    if (mMovementStates._1F) {
        return false;
    }
    if (getPlayerMode() == PlayerMode_Teresa) {
        doTeresaReflection(*MR::getNormal(triangle), false);
        return false;
    }
    bool started = doNeedle(triangle);
    if (started) {
        mActor->decLife(0);
        if (!mActor->mHealth) {
            mActor->forceGameOverNonStop();
        }
    }
    return started;
}

bool Mario::doNeedle(const Triangle* triangle) {
    if (getCurrentStatus() == MarioStatus_FireDamage) {
        return false;
    }
    if (getCurrentStatus() == MarioStatus_FireRun) {
        return false;
    }
    if (getCurrentStatus() == MarioStatus_FireDance) {
        return false;
    }
    if (mMovementStates._1B) {
        return false;
    }
    if (getPlayerMode() == PlayerMode_Teresa) {
        if (triangle) {
            doTeresaReflection(*MR::getNormal(triangle), false);
        }
        return false;
    }
    if (isInvincible()) {
        return false;
    }
    mActor->resetPlayerModeOnDamage();
    getPlayer()->mMovementStates._B = false;
    getPlayer()->mMovementStates._A = false;
    mActor->damageDropThrowMemoSensor();
    mFireDance->_29 = 1;
    changeStatus(mFireDance);
    return true;
}

bool Mario::doFireDance() {
    if (getCurrentStatus() == MarioStatus_Paralyze) {
        return false;
    }
    if (getCurrentStatus() == MarioStatus_FireDamage) {
        return false;
    }
    if (getCurrentStatus() == MarioStatus_FireRun) {
        return false;
    }
    if (getCurrentStatus() == MarioStatus_FireDance) {
        return false;
    }
    if (mMovementStates._1B) {
        return false;
    }
    if (isInvincible()) {
        return false;
    }
    if (getPlayerMode() == PlayerMode_Ice) {
        return false;
    }
    mActor->resetPlayerModeOnDamage();
    getPlayer()->mMovementStates._B = false;
    getPlayer()->mMovementStates._A = false;
    mActor->damageDropThrowMemoSensor();
    mFireDance->_29 = 0;
    changeStatus(mFireDance);
    mFireDamage->_12 = 120;
    return true;
}

MarioFireDance::MarioFireDance(MarioActor* actor) : MarioState(actor, MarioStatus_FireDance) {
    _14.zero();
    _24 = 0;
    _20 = 0.0f;
    _26 = 0;
    _28 = 0;
    _29 = 0;
}

bool MarioFireDance::start() {
    stopAnimationUpper(nullptr, nullptr);
    _20 = -mActor->getConst().getTable()->mFireDanceFirstJump;
    Mario* player = getPlayer();
    MR::vecKillElement(player->mJumpVec, getAirGravityVec(), &_14);
    _24 = 0;
    _26 = 0;
    if (getPlayer()->mDrawStates._10) {
        _20 *= 0.25f;
        _14 = getPlayer()->getWallNorm() * mActor->getConst().getTable()->mFireDanceFirstJump * 0.5f;
        getPlayer()->setFrontVecKeepUp(getPlayer()->getWallNorm());
        _24 = 1;
        _26 = 60;
    }
    _28 = 0;
    impact();
    impactEffect();
    startPadVib(3);
    getPlayer()->mJumpVec = getAirGravityVec() * _20;
    return true;
}

void MarioFireDance::impact() {
    changeAnimation("ファイアダンス", static_cast< const char* >(nullptr));
    if (!getPlayer()->mDrawStates._10) {
        if (_14.length() > 2.0f * mActor->getConst().getTable()->mFireDanceMoveSpeed) {
            _14.setLength(2.0f * mActor->getConst().getTable()->mFireDanceMoveSpeed);
        }
        _14.setLength(0.5f * _14.length());
    }
    getPlayer()->mMovementStates._1 = false;
    getPlayer()->mMovementStates.jumping = true;
}

void MarioFireDance::impactEffect() {
    playSound("ダメージ");
    playEffect("ダメージ");
    switch (_29) {
    case 0:
        playSound("声炎ダメージ");
        playSound("炎ダメージ");
        if (mActor->_1B4) {
            playEffect("炎ダメージ青煙");
        } else {
            playEffect("炎ダメージ煙");
        }
        break;
    case 1:
        playSound("声針ダメージ");
        playSound("針ダメージ");
        break;
    }
}

bool MarioFireDance::update() {
    if (getPlayer()->mMovementStates._1 && !getPlayer()->isRising()) {
        if (getPlayer()->_960 != 0x81 && getPlayer()->_960 != CollisionFloorCode_DamageFire && getPlayer()->_960 != CollisionFloorCode_Needle) {
            if (_28 == 1) {
                if (MR::getPlayerLeft() == 0 && !mActor->mHealth) {
                    mActor->changeGameOverAnimation();
                    return true;
                }
                getPlayer()->connectToFireRun();
                return false;
            }
            _28++;
            _20 = -mActor->getConst().getTable()->mFireDanceSecondJump;
            impact();
            startPadVib(0UL);
            playSound("炎ダメージ復帰バウンド");
            if (_29 == 1) {
                playSound("声針ダメージ中");
            } else {
                playSound("声炎ダメージ中");
            }
            changeAnimation("ファイアダンス", static_cast< const char* >(nullptr));
        } else {
            if (!_29) {
                mActor->decLifeLarge();
            } else {
                mActor->decLifeMiddle();
            }
            if (!mActor->mHealth) {
                mActor->forceGameOverNonStop();
            }
            _20 = -mActor->getConst().getTable()->mFireDanceFirstJump;
            impact();
            impactEffect();
            startPadVib(3);
        }
    }

    getPlayer()->mJumpVec = getAirGravityVec() * _20;
    addVelocity(getAirGravityVec() * _20);
    if (_20 < 0.0f) {
        _20 += mActor->getConst().getTable()->mFireDanceGravityRise;
    } else {
        _20 += mActor->getConst().getTable()->mFireDanceGravityDrop;
    }
    if (_20 > 50.0f) {
        _20 = 50.0f;
    }
    if (getStickP() != 0.0f) {
        if (_26) {
            _26--;
        } else {
            const TVec3f& padDir = getWorldPadDir();
            getPlayer()->setFrontVecKeepUp(padDir, mActor->getConst().getTable()->mFireDanceTurnRatio);
            _14 += getFrontVec() * mActor->getConst().getTable()->mFireDanceMoveAcc;
        }
    }
    MR::vecKillElement(_14, getAirGravityVec(), &_14);
    if (!_24 && _14.length() > mActor->getConst().getTable()->mFireDanceMoveSpeed) {
        _14.setLength(mActor->getConst().getTable()->mFireDanceMoveSpeed);
    }
    addVelocity(_14);
    return true;
}

bool MarioFireDance::close() {
    stopAnimation("ファイアダンス");
    stopEffect("炎ダメージ煙");
    stopEffect("炎ダメージ青煙");
    return true;
}

void Mario::checkKarikariDamage() {
    if (!_1C._5 || isDamaging()) {
        _7D0 = 120;
        return;
    }
    if (mActor->_934) {
        return;
    }
    if (mActor->_EA4) {
        return;
    }
    if (isStatusActive(MarioStatus_Talk)) {
        return;
    }
    if (_1C._5 && _7D0) {
        _7D0--;
        if (!_7D0) {
            if (mActor->mHealth == 1) {
                faint(mHeadVec);
                _7D0 = 120;
                return;
            }
            startPadVib(2);
            playSound("声小ダメージ");
            playSound("ダメージ");
            mActor->decLifeSmall();
            mActor->_BC4 = 16;
            _7D0 = 120;
            if (!mActor->mHealth) {
                mActor->forceGameOver();
            }
        }
    }
}

bool Mario::doDarkDamage() {
    if (getCurrentStatus() == MarioStatus_DarkDamage) {
        return false;
    }
    mActor->_3C0 = true;
    stopWalk();
    mActor->damageDropThrowMemoSensor();
    playSound("声沼沈み");
    playEffect("ダークマター死亡");
    setSeVersion(1);
    changeStatus(mDarkDamage);
    return true;
}

MarioDarkDamage::MarioDarkDamage(MarioActor* actor) : MarioState(actor, MarioStatus_DarkDamage) {
    _12 = 0;
    _14 = 0;
}

bool MarioDarkDamage::start() {
    _12 = 0;
    _14 = 0;
    return true;
}

bool MarioDarkDamage::update() {
    switch (_14) {
    case 0:
        _14++;
        _12 = 150;
        MR::requestStartGameOverDemo();
        break;
    case 1:
        if (_12) {
            _12--;
        }
        if (!_12) {
            mActor->forceKill(3);
            MarioActor* actor = mActor;
            actor->_481 = 1;
            actor->updateHand();
            actor->updateFace();
        }
        break;
    }
    if (_12) {
        playSound("ダークマター沈み");
    }
    return true;
}

bool MarioState::close() {
    return true;
}

bool MarioState::update() {
    return true;
}

bool MarioState::start() {
    return true;
}

bool MarioDarkDamage::notice() {
    return true;
}

bool MarioAbyssDamage::notice() {
    return true;
}
