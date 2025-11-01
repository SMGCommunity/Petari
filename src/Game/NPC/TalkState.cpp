#include "Game/NPC/TalkState.hpp"
#include "Game/NPC/TalkMessageCtrl.hpp"

void TalkState::init(TalkMessageCtrl *pArg1, TalkBalloon *pArg2) {
    this->_04 = pArg1;
    this->_08 = pArg2;
    this->mMessageID = this->_04->getMessageID();
}
