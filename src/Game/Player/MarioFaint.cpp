#include "Game/Player/MarioFaint.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioSwim.hpp"

MarioFaint::MarioFaint(MarioActor *pActor) : MarioState(pActor, 3), _12(0) {
    _14 = 0;
    _16 = 0;
    _18.zero();
    _24 = false;
    _25 = false;
}

bool MarioFaint::start() {
    _12 = 0;
    _16 = 0;
    getPlayer()->mMovementStates._B = 0;
    getPlayer()->mMovementStates._0 = 0;
    if (_18.dot(getPlayer()->mFrontVec) > 0.0f) {
        getPlayer()->setFrontVecKeepUp(_18);
        changeAnimation("後方小ダメージ", (char*)nullptr);
    }
    else {
        TVec3f stack_8;
        getPlayer()->setFrontVecKeepUp(_18.negateInline());
        changeAnimation("前方小ダメージ", (char*)nullptr);
    }
    if (_24) {
        changeAnimation("ノーダメージ", (char*)nullptr);
    }
    playSound("声小ダメージ", -1);
    playSound("ダメージ", -1);
    playEffect("ダメージ");
    startPadVib(2);
    addVelocity(_18);
    _25 = !_24;
    if (!_24) {
        mActor->decLife(0);
        mActor->resetPlayerModeOnDamage();
        if (mActor->mHealth == 0) {
            if (!getPlayer()->mMovementStates._1) {
                mActor->forceGameOverNonStop();
            }
            else {
                mActor->forceGameOver();
            }
        }
        return true;
    }
    _24 = 0;
    mActor->resetPlayerModeOnNoDamage();
    return true;
}

bool MarioFaint::close() {
    if (getPlayer()->mMovementStates._1) {
        stopAnimation("後方小ダメージ", (char*)nullptr);
        stopAnimation("前方小ダメージ", "基本");
    }
    if (_25) {
        _14 = 0x78;
    }
    return true;
}

// regswap
bool MarioFaint::update() {
    _12++;
    switch (_16) {
        case 0:
            addVelocity(_18);
            //MarioConst* pConst = mActor->mConst;
            _18.scale(mActor->mConst->mTable[mActor->mConst->mCurrentTable]->mFaintFriction1);
            if (_12 == mActor->mConst->mTable[mActor->mConst->mCurrentTable]->mFaintTimer1) {
                _16++;
            }
            break;
        case 1:
            if (!getPlayer()->mMovementStates._1) {
                return false;
            }
            addVelocity(_18);
            MarioConst* pConst2 = mActor->mConst;
            _18.scale(pConst2->mTable[pConst2->mCurrentTable]->mFaintFriction2);
            if (checkTrgA()) {
                getPlayer()->tryJump();
                return false;
            }
            MarioConst* pConst3 = mActor->mConst;
            MarioConstTable* pTable = pConst3->mTable[pConst3->mCurrentTable];
            if (_12 == pTable->mFaintTimer1 + pTable->mFaintTimer2) {
                return false;
            }
            break;
        default:
            break;
    }
    return true;
}

void MarioFaint::setVec(const TVec3f &rVec) {
    MR::vecKillElement(rVec, mActor->_240, &_18);
    _18.setLength(mActor->mConst->mTable[mActor->mConst->mCurrentTable]->mSlideDistFaint);
    _12 = 0;
    _16 = 0;
    if (getPlayer()->isStatusActive(5)) {
        _18.zero();
    }
}

bool Mario::doFlipWeak(const TVec3f &rVec) {
    if (mMovementStates._1B) {
        return false;
    }
    u8 var = 1;
    mFaint->_24 = var;
    if (faint(rVec)) {
        if (isStatusActive(6)) {
            mSwim->_AC = var;
            mFaint->_24 = false;
        }
        return true;
    }
    mFaint->_24 = false;
    forceStopTornado();
    return false;
}

bool Mario::faint(const TVec3f &rVec) {
    _7C4.setPS2(rVec);
    if (!isEnableAddDamage()) {
        return false;
    }
    if (getCurrentStatus() == 2) {
        return false;
    }
    mFaint->setVec(rVec);
    stopWalk();
    forceStopTornado();
    mActor->damageDropThrowMemoSensor();
    if (getCurrentStatus() != 3) {
        mMovementStates._27 = 1;
        return true;
    }
    return mMovementStates._27;
}
