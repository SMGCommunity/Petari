#include "Game/Player/MarioFlow.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioConst.hpp"

MarioFlow::MarioFlow(MarioActor *pActor) : MarioState(pActor, 0x19), _12(0) {
    _14 = 0;
    _18.z = 0.0f;
    _18.y = 0.0f;
    _18.x = 0.0f;
}

bool Mario::doFlow() {
    if (isStatusActive(0x19)) {
        return false;
    }
    if (isAnimationRun("空中ひねり")) {
        return false;
    }
    changeStatus(mFlow);
    return true;
}

bool MarioFlow::start() {
    _12 = 0;
    _14 = 0;
    changeAnimationNonStop("中ダメージ");
    startPadVib(2);
    getPlayer()->mMovementStates._1 = 0;
    getPlayer()->mMovementStates._0 = 1;
    getPlayer()->mMovementStates._B = 0;
    getPlayer()->mMovementStates._2B = 1;
    _18 = getFrontVec() * mActor->mConst->mTable[mActor->mConst->mCurrentTable]->mJumpDistFlow;
    _18 += mActor->_240.negateInline() * mActor->mConst->mTable[mActor->mConst->mCurrentTable]->mJumpHeightFlow;
    getPlayer()->mJumpVec = _18;
    addVelocity(_18);
    getPlayer()->stopWalk();
    return true;
}

bool MarioFlow::update() {
    _12++;
    switch (_14) {
        case 0:
            addVelocity(_18);
            TVec3f stack_8 = mActor->_240 * mActor->mConst->mTable[mActor->mConst->mCurrentTable]->mGravityFlow;
            _18 += stack_8;
            if (_12 == 0x14) {
                changeAnimation("中ダメージ空中", (char*)nullptr);
            }
            if (_12 > 0x1e && mActor->isRequestRush()) {
                getPlayer()->trySpinJump(0);
                return false;
            }
            if (getPlayer()->mMovementStates._1) {
                getPlayer()->mMovementStates._0 = 0;
                changeAnimation("中ダメージ着地", (char*)nullptr);
                playEffect("共通ダメージ着地");
                playSound("吹っ飛び倒れ", -1);
                MR::vecKillElement(_18, mActor->_240, &_18);
                _12 = 0;
                _14++;
            }
            break;
        case 1:
            if (!getPlayer()->mMovementStates._1) {
                getPlayer()->mMovementStates._0 = 1;
                _14 = 0;
            }
            else {
                addVelocity(_18);
                _18.mult(0.95f);
                if (!isAnimationRun("中ダメージ着地")) {
                    return false;
                }
                if (_12 > 0xf && checkTrgA()) {
                    getPlayer()->tryJump();
                    return false;
                }
            }
            break;
        default:
            break;
    }
    getPlayer()->mJumpVec = _18;
    return true;
}

bool MarioFlow::close() {
    stopAnimation("ダメージ", (char*)nullptr);
    stopAnimation("ダメージ着地", "基本");
    return true;
}
