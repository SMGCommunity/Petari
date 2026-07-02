#include "Game/Player/PlayerEventRaceDown.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

EventRaceDown::EventRaceDown() : EventSequence(16) {
    addEventOnTime("初期化", static_cast< EventFunc1 >(&EventRaceDown::init), 0);
    addEventOnTime("MISSレイアウト開始", static_cast< EventFunc1 >(&EventRaceDown::missLayoutOpen), 45);
    addEventOnTime("通常レイアウト消去", (&EventRaceDown::closeDefaultLayout), 30);
    addEventOnTime("サウンドA", static_cast< EventFunc1 >(&EventRaceDown::sound), 30);
    addEventOnTime("サウンドB", static_cast< EventFunc1 >(&EventRaceDown::sound), 100);
    addEventOnTime("サウンドC", static_cast< EventFunc1 >(&EventRaceDown::sound), 120);
    addEventInStatus("ワイプ開始", static_cast< EventFunc1 >(&EventRaceDown::doCloseWipe),
                     static_cast< EventFunc2 >(&EventRaceDown::checkCloseWipeStart));
    addEventOnTime("残機を引く", static_cast< EventFunc1 >(&EventRaceDown::decLeft), 120);
    addEventInPhase("ワイプ終了後", static_cast< EventFunc1 >(&EventRaceDown::doWaitAfterWipe), 2);
}

void EventRaceDown::init(u16, u16) {
    MR::setCubeBgmChangeInvalid();
    MR::clearBgmQueue();
    MR::stopStageBGM(10);
    MR::stopSubBGM(10);
    playAnimation("レース負け");
    playSound("声最終ダメージ");
    playSound("最後の一撃");
    MR::startPlayerDownWipe();
}

void EventRaceDown::sound(u16 eventFrame, u16 sequenceFrame) {
    switch (sequenceFrame) {
    case 30:
        MR::setSoundVolumeSetting(2, 20);
        MR::startSubBGM("BGM_MISS", false);
        break;
    case 100:
    case 120:
        break;
    }
}

bool EventRaceDown::checkCloseWipeStart(u16 sequenceFrame) {
    if (isMissLayoutClosed(sequenceFrame) && getPhase() == 1) {
        return true;
    } else {
        return false;
    }
}

void EventRaceDown::missLayoutOpen(u16 eventFrame, u16 sequenceFrame) {
    MR::startMissLayout();
    nextPhase();
}
