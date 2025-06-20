#include "Game/Player/MarioSukekiyo.hpp"
#include "Game/Player/MarioBury.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Map/HitInfo.hpp"

MarioSukekiyo::MarioSukekiyo(MarioActor *pActor) : MarioState(pActor, 0x1a) {
    _44 = new Triangle();
    _14.zero();
    _20.zero();
    _2C.zero();
    _38.zero();
    _48 = 0;
    _4A = 0;
}

MarioBury::MarioBury(MarioActor *pActor) : MarioSukekiyo(pActor) {
    mStatusId = 0x1b;
}

bool MarioSukekiyo::start() {
    playSound("スケキヨ開始", -1);
    Mario* pMario = getPlayer();
    playEffectRT("属性尻ドロップ", pMario->_368, getTrans());
    startPadVib("最強");
    startCamVib(0x3);
    mActor->_F44 = false;
    _14 = getPlayer()->_368;
    _20 = getPlayer()->mSideVec;
    getPlayer()->forceSetHeadVecKeepSide(_14);
    _48 = 0;
    _4A = 0;
    getPlayer()->stopJump();
    getPlayer()->stopWalk();
    if (mStatusId == 0x1a) {
        changeAnimation("スケキヨ", (char*)nullptr);
    }
    else {
        playSound("声足埋まり開始", -1);
        changeAnimation("埋まり", (char*)nullptr);
    }
    return true;
}

bool MarioSukekiyo::close() {
    getPlayer()->stopWalk();
    mActor->_F44 = true;
    stopAnimation(nullptr, "基本");
    return true;
}

bool MarioSukekiyo::update() {
    if (!getPlayer()->isCurrentFloorSand()) {
        return false;
    }
    if (_4A) {
        return isAnimationRun(nullptr) != false;
    }
    if (mActor->isRequestRush()) {
        _4A = 1;
    }
    if (checkTrgA()) {
        _4A = 1;
    }
    if (_4A) {
        if (mStatusId == 0x1a) {
            changeAnimation("スケキヨ脱出", "基本"); // for some reason these strings don't want to order themselves correctly in the .data
            playSound("声スケキヨ終了", -1);
        }
        else {
            changeAnimation("埋まり脱出", "基本");
            playSound("声足埋まり終了", -1);
        }
        playSound("スケキヨ終了", -1);
    }
    return true;
}

bool MarioSukekiyo::notice() {
    return false;
}

bool MarioSukekiyo::postureCtrl(MtxPtr mtx) {
    MR::makeMtxUpSide((TPos3f *)mtx, _14, _20);
    return true;
}
