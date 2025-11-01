#include "Game/NPC/TalkState.hpp"
#include "Game/NPC/TalkMessageCtrl.hpp"

void TalkState::init(TalkMessageCtrl *t1, TalkBalloon *t2) {
    this->_04 = t1;
    this->_08 = t2;
    this->mMessageID = this->_04->getMessageID();
}
