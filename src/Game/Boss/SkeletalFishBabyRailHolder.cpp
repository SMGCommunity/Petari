#include "Game/Boss/SkeletalFishBabyRailHolder.hpp"
#include "Game/Boss/SkeletalFishBabyRail.hpp"

SkeletalFishBabyRailGroupNode::SkeletalFishBabyRailGroupNode(s32 a1) {
    _0 = a1;
    mNumNodes = 0;
    _10 = 0;
    _14 = 0;
    _18 = nullptr;
    _1C = nullptr;
    _20 = nullptr;
    _24 = nullptr;

    SkeletalFishBabyRailSetLinkNode* node = new SkeletalFishBabyRailSetLinkNode();
    _18 = node;
    _1C = node;
    _20 = node;
    _24 = node;
}

void SkeletalFishBabyRailGroupNode::addChild(SkeletalFishBabyRail *pRail) {
    if (MR::isLoopRail(pRail)) {
        if (_24->_10 == nullptr) {
            createChild();
        }

        SkeletalFishBabyRailSetLinkNode* node = _24->_10;
        _24 = node;
        node->_4 = pRail;
    }
    else {
        if (_20->_10 == nullptr) {
            createChild();
        }

        SkeletalFishBabyRailSetLinkNode* node = _20->_10;
        _20 = node;
        node->_0 = pRail;
    }
}

void SkeletalFishBabyRailGroupNode::tidy() {
    for (SkeletalFishBabyRailSetLinkNode* curLink = _18->_10; curLink != nullptr; curLink = curLink->_10) {
        TVec3f endPointPos;
        MR::calcRailEndPointPos(&endPointPos, curLink->_0);
        SkeletalFishBabyRailSetLinkNode* v2 = curLink;
        TVec3f nearestRailPos;
        MR::calcNearestRailPos(&nearestRailPos, curLink->_4, endPointPos);
        f32 totalDist = PSVECDistance(endPointPos.toCVec(), nearestRailPos.toCVec());

        for (SkeletalFishBabyRailSetLinkNode* child = curLink->_10; child != nullptr; child = child->_10) {
            MR::calcNearestRailPos(&nearestRailPos, child->_4, endPointPos);
            f32 dist = PSVECDistance(endPointPos.toCVec(), nearestRailPos.toCVec());

            if (dist < totalDist) {
                totalDist = dist;
                v2 = child;
            }
        }

        if (curLink != v2) {
            SkeletalFishBabyRail* rail = curLink->_4;
            curLink->_4 = v2->_4;
            v2->_4 = rail;
        }
    }
}

void SkeletalFishBabyRailGroupNode::createChild() {
    SkeletalFishBabyRailSetLinkNode* node = new SkeletalFishBabyRailSetLinkNode();
    node->_C = _1C;
    _1C->_10 = node;
    _1C = node;
    mNumNodes++;
}

SkeletalFishBabyRailHolder::SkeletalFishBabyRailHolder(const char *pName) : NameObj(pName) {
    mNodes = nullptr;
}

void SkeletalFishBabyRailHolder::initAfterPlacement() {
    SkeletalFishBabyRailGroupNode* nodes = mNodes;
    SkeletalFishBabyRailGroupNode* curNode;

    for (curNode = nodes; curNode != nullptr; curNode = curNode->_14) {
        curNode->tidy();
    }   
}

void SkeletalFishBabyRailHolder::add(SkeletalFishBabyRail *pRail) {
    SkeletalFishBabyRailGroupNode* nextNode = mNodes;

    for (; nextNode != nullptr; nextNode = nextNode->_14) { 
        if (nextNode->_0 == pRail->_8C) {
            break;
        }
    }

    if (nextNode == nullptr) {
        nextNode = createGroup(pRail->_8C);
    }

    nextNode->addChild(pRail);
}

SkeletalFishBabyRailGroupNode* SkeletalFishBabyRailHolder::createGroup(s32 a1) {
    SkeletalFishBabyRailGroupNode* groupNode = new SkeletalFishBabyRailGroupNode(a1);
    SkeletalFishBabyRailGroupNode* cur = mNodes;
    mNodes = groupNode;
    groupNode->_14 = cur;

    if (cur != nullptr) {
        cur->_10 = groupNode;
    }

    return groupNode;
}

namespace MR {
    SkeletalFishBabyRailHolder* getSkeletalFishBabyRailHolder() {
        return MR::getSceneObj<SkeletalFishBabyRailHolder*>(SceneObj_SkeletalFishBabyRailHolder);
    }

    void createSkeletalFishBabyRailHolder() {
        MR::createSceneObj(SceneObj_SkeletalFishBabyRailHolder);
    }
};

SkeletalFishBabyRailHolder::~SkeletalFishBabyRailHolder() {

}

SkeletalFishBabyRailSetLinkNode::SkeletalFishBabyRailSetLinkNode() {
    _0 = 0;
    _4 = nullptr;
    _8 = 0;
    _C = nullptr;
    _10 = nullptr;   
}