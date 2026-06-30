#include "Game/Player/PlayerEventDown.hpp"
#include "Game/Player/MarioAccess.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

EventDown::EventDown() : EventSequence(16) {
    addEventOnTime("初期化", static_cast< EventFunc1 >(&EventDown::init), 0);
    addEventOnTime("MISSレイアウト開始", static_cast< EventFunc1 >(&EventDown::missLayoutOpen), 45);
    addEventOnTime("通常レイアウト消去", static_cast< EventFunc1 >(&EventDown::closeDefaultLayout), 30);
    addEventOnTime("サウンドA", static_cast< EventFunc1 >(&EventDown::sound), 30);
    addEventInTime("ワイプ開始", static_cast< EventFunc1 >(&EventDown::doCloseWipe), 100, 600);
    addEventOnTime("残機を引く", static_cast< EventFunc1 >(&EventDown::decLeft), 120);
    addEventInPhase("ワイプ終了後", static_cast< EventFunc1 >(&EventSequence::doWaitAfterWipe), 2);
}

void EventDown::init(u16 eventFrame, u16 sequenceFrame) {
    MR::setCubeBgmChangeInvalid();
    MR::clearBgmQueue();
    MR::stopStageBGM(10);
    MR::stopSubBGM(10);
    MarioAccess::getPlayerActor()->changeGameOverAnimation();
    playSound("声最終ダメージ");
    playSound("最後の一撃");
    MR::startPlayerDownWipe();
}
void EventDown::sound(u16 eventFrame, u16 sequenceFrame) {
    switch (sequenceFrame) {
    case 30:
        MR::setSoundVolumeSetting(2, 20);
        MR::startSubBGM("BGM_MISS", false);
        break;
    }
}
void EventDown::missLayoutOpen(u16 eventFrame, u16 sequenceFrame) {
    MR::startMissLayout();
    nextPhase();
}
