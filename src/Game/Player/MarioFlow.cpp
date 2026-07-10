#include "Game/Player/MarioFlow.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Util/MathUtil.hpp"

bool Mario::doFlow() {
    if (isStatusActive(MarioStatus_Flow)) {
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

    getPlayer()->mMovementStates._1 = false;
    getPlayer()->mMovementStates.jumping = true;
    getPlayer()->mMovementStates._B = true;
    getPlayer()->mMovementStates._2B = true;

    _18 = getFrontVec() * mActor->mConst->getTable()->mJumpDistFlow;
    _18 += mActor->_240.negateInline() * mActor->mConst->getTable()->mJumpHeightFlow;
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

        _18 += mActor->_240 * mActor->getConst().getTable()->mGravityFlow;

        if (_12 == 20) {
            changeAnimation("中ダメージ空中", static_cast< const char* >(nullptr));
        }

        if (_12 > 30 && mActor->isRequestRush()) {
            getPlayer()->trySpinJump(0);

            return false;
        }

        if (getPlayer()->getMovementStates()._1) {
            getPlayer()->mMovementStates.jumping = false;

            changeAnimation("中ダメージ着地", static_cast< const char* >(nullptr));

            playEffect("共通ダメージ着地");
            playSound("吹っ飛び倒れ", -1);

            MR::vecKillElement(_18, mActor->_240, &_18);

            _12 = 0;
            _14++;
        }
        break;
    case 1:
        if (getPlayer()->getMovementStates()._1 == false) {
            getPlayer()->mMovementStates.jumping = true;
            _14 = 0;
        } else {
            addVelocity(_18);

            _18.x *= 0.95f;
            _18.y *= 0.95f;
            _18.z *= 0.95f;

            if (!isAnimationRun("中ダメージ着地")) {
                return false;
            }

            if (_12 > 15 && checkTrgA()) {
                getPlayer()->tryJump();

                return false;
            }
        }
        break;
    }

    getPlayer()->mJumpVec = _18;

    return true;
}

bool MarioFlow::close() {
    stopAnimation("ダメージ", static_cast< const char* >(nullptr));
    stopAnimation("ダメージ着地", "基本");

    return true;
}

MarioFlow::MarioFlow(MarioActor* pActor) : MarioState(pActor, MarioStatus_Flow), _12(), _14() {
    _18.zeroInline();
}
