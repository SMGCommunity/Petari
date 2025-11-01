#include "Game/NPC/TalkState.hpp"
#include "Game/NPC/TalkMessageCtrl.hpp"

void TalkState::init(TalkMessageCtrl *pArg1, TalkBalloon *pArg2) {
    _04 = pArg1;
    _08 = pArg2;
    mMessageID = _04->getMessageID();
}
