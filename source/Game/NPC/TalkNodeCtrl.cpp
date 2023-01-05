#include "Game/NPC/TalkNodeCtrl.h"

bool TalkNodeCtrl::isExistNextNode() const {
    return getNextNode() != NULL;
}

bool TalkNodeCtrl::isNextNodeMessage() const {
    TalkNode* next = getNextNode();

    if (next != NULL || next->mNodeType != 1) {
        return false;
    }

    return true;
}

bool TalkNodeCtrl::isCurrentNodeEvent() const {
    return getCurrentNodeEvent() != NULL;
}

// TalkNodeCtrl::getNextNode

TalkNode* TalkNodeCtrl::getNextNodeBranch() const {
    TalkNode* node = getNextNode();
    
    if (node == NULL) {
        return NULL;
    }

    return node->mNodeType != 2 ? NULL : node;
}

TalkNode* TalkNodeCtrl::getCurrentNodeBranch() const {
    if (mCurrentNode != NULL && mCurrentNode->mNodeType == 2) {
        return mCurrentNode;
    }

    return NULL;
}

TalkNode* TalkNodeCtrl::getCurrentNodeMessage() const {
    if (mCurrentNode != NULL && mCurrentNode->mNodeType == 1) {
        return mCurrentNode;
    }

    return NULL;
}

TalkNode* TalkNodeCtrl::getCurrentNodeEvent() const {
    if (mCurrentNode != NULL && mCurrentNode->mNodeType == 3) {
        return mCurrentNode;
    }

    return NULL;
}

TalkNode* TalkNodeCtrl::getNextNodeEvent() const {
    TalkNode* node = getNextNode();
    
    if (node == NULL) {
        return NULL;
    }

    return node->mNodeType != 3 ? NULL : node;
}