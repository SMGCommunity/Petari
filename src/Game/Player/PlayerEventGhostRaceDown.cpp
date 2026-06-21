#include "Game/Player/PlayerEventGhostRaceDown.hpp"
#include "Game/Util/SoundUtil.hpp"

EventGhostRaceDown::EventGhostRaceDown() : EventSequence(16) {
    addEventOnTime("初期化", static_cast< EventFunc1 >(&EventGhostRaceDown::init), 0);
    addEventOnTime("通常レイアウト消去", static_cast< EventFunc1 >(&EventGhostRaceDown::closeDefaultLayout), 30);
    addEventOnTime("サエンドA", static_cast< EventFunc1 >(&EventGhostRaceDown::sound), 30);
    addEventOnTime("サエンドB", static_cast< EventFunc1 >(&EventGhostRaceDown::sound), 120);
    addEventOnTime("サエンドC", static_cast< EventFunc1 >(&EventGhostRaceDown::sound), 140);
    addEventInTime("ワイプ開始", static_cast< EventFunc1 >(&EventGhostRaceDown::doCloseWipe), 92, 200);
    addEventOnTime("演出を引き", static_cast< EventFunc1 >(&EventGhostRaceDown::decLeft), 120);
    addEventInPhase("ワイプ終了後", static_cast< EventFunc1 >(&EventGhostRaceDown::doWaitAfterWipe), 2);
}

void EventGhostRaceDown::init(u16 eventFrame, u16 sequenceFrame) {
    MR::stopStageBGM(10);
    MR::stopSubBGM(10);
    MR::setCubeBgmChangeInvalid();
    playAnimation("レース負け");
    playSound("声最終ダメージ");
    playSound("最後の一撃");
}
void EventGhostRaceDown::sound(u16 eventFrame, u16 sequenceFrame) {
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
