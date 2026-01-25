#include "Game/Player/MarioSukekiyo.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"

MarioSukekiyo::MarioSukekiyo(MarioActor* pActor) : MarioState(pActor, 0x1A) {
    _44 = new Triangle();
    _14.zero();
    _20.zero();
    _2C.zero();
    _38.zero();
    _48 = 0;
    _4A = 0;
}

MarioBury::MarioBury(MarioActor* pActor) : MarioSukekiyo(pActor) {
    mStatusId = 27;
}

bool MarioSukekiyo::close() {
    getPlayer()->stopWalk();
    mActor->_F44 = 1;
    stopAnimation(nullptr, "基本");
    return true;
}

bool MarioSukekiyo::notice() {
    return false;
}

bool MarioSukekiyo::postureCtrl(MtxPtr mtx) {
    MR::makeMtxUpSide((TPos3f*)mtx, _14, _20);
    return true;
}

bool MarioSukekiyo::start() {
    playSound("スケキヨ開始", -1);
    Mario* player = getPlayer();
    playEffectRT("属性尻ドロップ", player->_368, getTrans());
    startPadVib("最強");
    startCamVib(3);
    mActor->_F44 = 0;
    _14 = getPlayer()->_368;
    _20 = getPlayer()->mSideVec;
    getPlayer()->forceSetHeadVecKeepSide(_14);
    _48 = 0;
    _4A = 0;
    getPlayer()->stopJump();
    getPlayer()->stopWalk();

    if (mStatusId == 26) {
        changeAnimation("スケキヨ", (const char*)nullptr);
    } else {
        playSound("声足埋まり開始", -1);
        changeAnimation("埋まり", (const char*)nullptr);
    }
    return true;
}

bool MarioSukekiyo::update() {
    if (!getPlayer()->isCurrentFloorSand()) {
        return false;
    }

    if (_4A) {
        return isAnimationRun(nullptr) != 0;
    }

    if (mActor->isRequestRush()) {
        _4A = 1;
    }

    if (checkTrgA()) {
        _4A = 1;
    }

    if (_4A) {
        if (mStatusId == 26) {
            changeAnimation("スケキヨ脱出", "基本");
            playSound("声スケキヨ終了", -1);
        } else {
            changeAnimation("埋まり脱出", "基本");
            playSound("声足埋まり終了", -1);
        }

        playSound("スケキヨ終了", -1);
    }

    return true;
}
