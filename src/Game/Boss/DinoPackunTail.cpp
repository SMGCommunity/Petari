#include "Game/Boss/DinoPackunTail.hpp"
#include "Game/Boss/DinoPackunTailNode.hpp"
#include "JSystem/JGeometry/TVec.hpp"

DinoPackunTail::DinoPackunTail(u32 nodeCount) {
    mNodes = nullptr;
    mMaxNodes = 0;
    mNumNodes = 0;
    _C = 1.0f;
    _10 = 1.0f;
    _14 = 0.0f;
    _18 = 0.0f;
    _1C = 1;
    mNodes = new DinoPackunTailNode*[nodeCount];
    mMaxNodes = nodeCount;
}

void DinoPackunTail::addTailNode(DinoPackunTailNode* pNode) {
    if (mNumNodes != 0) {
        u32 prev = mNumNodes - 1;
        DinoPackunTailNode* prevNode = mNodes[prev];
        prevNode->_B8 = pNode;
        pNode->_B4 = prevNode;
    }

    u32 num = mNumNodes;
    mNumNodes = num + 1;
    mNodes[num] = pNode;
}

void DinoPackunTail::activate() {
    _1C = 1;
    u32 count = mNumNodes;

    for (u32 i = 0; i < count; i++) {
        mNodes[i]->resetJoint();
        mNodes[i]->makeActorAppeared();
    }
}

void DinoPackunTail::deactivate() {
    _1C = 0;
    u32 count = mNumNodes;

    for (u32 i = 0; i < count; i++) {
        mNodes[i]->makeActorDead();
    }
}

void DinoPackunTail::onMovement() {
    u32 count = mNumNodes;

    for (u32 i = 0; i < count; i++) {
        MR::requestMovementOn(mNodes[i]);
    }
}

void DinoPackunTail::lockNodePosition(u32 idx) {
    mNodes[idx]->lockPosition();
}

void DinoPackunTail::unlockNodePosition(u32 idx) {
    mNodes[idx]->unLockPosition();
}

void DinoPackunTail::lockEndNodePosition() {
    getNode(mNumNodes - 1)->lockPosition();
}

void DinoPackunTail::unlockEndNodePosition() {
    getNode(mNumNodes - 1)->unLockPosition();
}

void DinoPackunTail::registerPreCalcJointCallBack() {
    if (_1C) {
        u32 count = mNumNodes;

        for (u32 i = 0; i < count; i++) {
            mNodes[i]->registerPreCalcJointCallBack();
        }
    }
}

void DinoPackunTail::registerJointCallBack() {
    if (_1C) {
        u32 count = mNumNodes;

        for (u32 i = 0; i < count; i++) {
            mNodes[i]->registerJointCallBack();
        }
    }
}

// https://decomp.me/scratch/PqcAa
void DinoPackunTail::updateJoint() {
    if (_1C) {
        addAccelKeepDistance();
        addAccelKeepBend();
        addAccelToBck();

        u32 count = mNumNodes;

        for (u32 i = 1; i < count; i++) {
            DinoPackunTailNode* node = mNodes[i];
            MR::addVelocity(mNodes[i], (node->_9C - (node->mPosition + node->mVelocity)) * _14);
        }
    }
}

// DinoPackunTail::addAccelKeepBend

// https://decomp.me/scratch/aWhPK
void DinoPackunTail::addAccelKeepDistance() {
    TVec3f v19;
    v19.set< f32 >(mNodes[0]->mPosition);
    u32 nodeCount = mNumNodes;

    for (u32 i = 1; i < nodeCount; i++) {
        TVec3f v18;
        v18.set< f32 >(mNodes[i]->mPosition);
        TVec3f v17;
        v17.set< f32 >(v18 - v19);

        if (MR::isNearZero(v17)) {
            MR::getRandomVector(&v17, 0.1f);
        }

        f32 scalar;
        MR::separateScalarAndDirection(&scalar, &v17, v17);
        f32 linkLength = mNodes[i]->getLinkLength();
        DinoPackunTailNode** nodes = mNodes;
        f32 v7 = ((0.89999998f * (_10 * (linkLength - scalar))) * 0.5f);
        nodes[i]->addNodeVelocityHost(v17 * v7);

        if (i != 0) {
            TVec3f v13;
            JMathInlineVEC::PSVECNegate(&v17, &v13);
            mNodes[i - 1]->addNodeVelocityHost(v13 * v7);
        }

        v19.setPS2(v18);
    }
}

void DinoPackunTail::addAccelToBck() {
    if (_18 <= 0.0f) {
        return;
    }

    for (u32 i = 1; i < mNumNodes; i++) {
        TVec3f v8;
        v8.set< f32 >(mNodes[i]->_9C - mNodes[i]->mPosition);
        mNodes[i]->addNodeVelocityHost(v8 * _18);
    }
}

void DinoPackunTail::getTailNodePosition(TVec3f* pPos, s32 index) const {
    pPos->set< f32 >(mNodes[index]->mPosition);
}

void DinoPackunTail::getEndTailPosition(TVec3f* pPos) const {
    s32 last = mNumNodes - 1;
    pPos->set< f32 >(mNodes[last]->mPosition);
}

DinoPackunTailNode* DinoPackunTail::getNode(u32 index) const {
    return mNodes[index];
}
