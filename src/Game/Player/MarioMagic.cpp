#include "Game/Player/MarioMagic.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioModule.hpp"

void Mario::stopPunch() {
    if (isStatusActive(17)) {
        closeStatus(mMagic);
    }

    MarioActor* actor = mActor;

    if (!actor->_944) {
        actor->_945 = 0;
        actor->_974 = 0;
    }

    actor->_944 = 0;
}

void Mario::startMagic() {
    if (!mMovementStates.jumping) {
        if (!mActor->_468) {
            if (!mMovementStates._23) {
                if (!isStatusActive(7)) {
                    if (isSkatableFloor()) {
                        doSkate();
                    } else {
                        clearSlope();
                        changeStatus(mMagic);
                        stopAnimationUpper(static_cast< const char* >(nullptr), static_cast< const char* >(nullptr));
                        _10._1 = 1;
                    }
                }
            }
        }
    }
}

MarioMagic::MarioMagic(MarioActor* pActor) : MarioState(pActor, 0x11) {
    _12 = 0;
}

bool MarioMagic::close() {
    stopEffect("スピンライト");

    if (_12 < 0x1A) {
        playEffect("スピンライト消去");
    }

    return true;
}

bool MarioMagic::start() {
    changeAnimation("地上ひねり", static_cast< const char* >(nullptr));
    stopEffect("パンチブラー左");
    stopEffect("パンチブラー右");
    playEffect("共通地上スピン");
    playSound("声スピン", -1);
    playSound("スピンジャンプ", -1);
    startPadVib(2);
    _12 = 0;
    return true;
}

bool MarioMagic::update() {
    if (mActor->isRequestJump()) {
        getPlayer()->tryJump();
        return false;
    } else if (!isAnimationRun("地上ひねり")) {
        return false;
    }

    _12++;

    if (_12 == 25) {
        stopEffect("スピンライト");
        playEffect("スピンライト消去");
    }

    if (getPlayer()->mMovementStates.jumping) {
        getPlayer()->procJump(false);
    } else {
        getPlayer()->mainMove();
    }

    getPlayer()->updateWalkSpeed();
    return true;
}
