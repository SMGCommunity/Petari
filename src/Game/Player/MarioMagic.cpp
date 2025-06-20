#include "Game/Player/MarioMagic.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"

void Mario::stopPunch() {
    if (isStatusActive(0x11)) {
        closeStatus(mMagic);
    }
    MarioActor* pActor = mActor;
    if (!pActor->_944) {
        pActor->_945 = 0;
        pActor->_974 = 0;
    }
    pActor->_944 = 0;
}

void Mario::startMagic() {
    if (!mMovementStates._0 && (mActor->_468.x == 0.0f) && !mMovementStates._23 && !isStatusActive(0x7)) {
        if (isSkatableFloor()) {
            doSkate();
        }
        else {
            clearSlope();
            changeStatus(mMagic);
            stopAnimationUpper(nullptr, (char*)nullptr);
            _10._1 = 1;
        }
    }
}

MarioMagic::MarioMagic(MarioActor *pActor) : MarioState(pActor, 0x11), _12(0){

}

bool MarioMagic::start() {
    changeAnimation("地上ひねり", (char*)nullptr);
    stopEffect("パンチブラー左");
    stopEffect("パンチブラー右");
    playEffect("共通地上スピン");
    playSound("声スピン", -1);
    playSound("スピンジャンプ", -1);
    startPadVib(2);
    _12 = 0;
    return true;
}

bool MarioMagic::close() {
    stopEffect("スピンライト");
    if (_12 < 0x1a) {
        playEffect("スピンライト消去");
    }
    return true;
}

bool MarioMagic::update() {
    if (mActor->isRequestJump()) {
        getPlayer()->tryJump();
        return false;
    }
    if (!isAnimationRun("地上ひねり")) {
        return false;
    }
    _12++;
    if (_12 == 0x19) {
        stopEffect("スピンライト");
        playEffect("スピンライト消去");
    }
    if (getPlayer()->mMovementStates._0) {
        getPlayer()->procJump(false);
    }
    else {
        getPlayer()->mainMove();
    }
    getPlayer()->updateWalkSpeed();
    return true;
}
