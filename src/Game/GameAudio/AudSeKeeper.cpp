#include "Game/GameAudio/AudSeKeeper.hpp"
#include "Game/AudioLib/AudSoundObject.hpp"
#include "Game/Util/HashUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <JSystem/J3DGraphAnimator/J3DJoint.hpp>

AudSeKeeperBox::AudSeKeeperBox() : mSoundObj(nullptr), mJoint(nullptr), mHash(0) {
}

AudSeKeeperBox::~AudSeKeeperBox() {
    if (mSoundObj != nullptr) {
        mSoundObj->dispose();
        delete mSoundObj;
    }
}

void AudSeKeeperBox::update() {
    updateJointPos();
    if (mSoundObj != nullptr) {
        mSoundObj->process();
    }
}

void AudSeKeeperBox::add(J3DJoint* pJoint, u16 hash) {
    mJoint = pJoint;
    mHash = hash;
    updateJointPos();
    mSoundObj = new AudSoundObject(&mPos, 8, MR::getCurrentHeap());
}

void AudSeKeeperBox::updateJointPos() {
    mPos.x = MR::getJointTransX(mJoint);
    mPos.y = MR::getJointTransY(mJoint);
    mPos.z = MR::getJointTransZ(mJoint);
}

AudSeKeeper::AudSeKeeper(LiveActor* pActor, int maxBoxes) : JKRDisposer(), mActor(pActor), mMaxBoxes(maxBoxes), mNumBoxes(0) {
    mBoxes = new AudSeKeeperBox[mMaxBoxes];
}

AudSeKeeper::~AudSeKeeper() {
    delete[] mBoxes;
}

void AudSeKeeper::update() {
    for (int idx = 0; idx < mNumBoxes; idx++) {
        mBoxes[idx].update();
    }
}

void AudSeKeeper::makeBox(const char* pJointName, const char* pJointName2) {
    if (mNumBoxes >= mMaxBoxes) {
        return;
    }

    J3DJoint* joint = MR::getJoint(mActor, pJointName);

    u16 hash;
    if (pJointName2 != nullptr) {
        hash = MR::getHashCode(pJointName2);
    } else {
        hash = MR::getHashCode(pJointName);
    }

    for (int idx = 0; idx < mNumBoxes; idx++) {
        if (hash == mBoxes[idx].mHash) {
            return;
        }
    }

    mBoxes[mNumBoxes].add(joint, hash);
    mNumBoxes++;
}

bool AudSeKeeper::playLevel(const char* pJointName, const char* pSoundName) {
    int idx = getBoxEqualName(pJointName);
    if (idx < 0) {
        return false;
    }
    MR::startSoundObjectLevel(mBoxes[idx].mSoundObj, pSoundName);
    return true;
}

int AudSeKeeper::getBoxEqualName(const char* pJointName) {
    u16 hash = MR::getHashCode(pJointName);
    for (int idx = 0; idx < mNumBoxes; idx++) {
        if (hash == mBoxes[idx].mHash) {
            return idx;
        }
    }
    return -1;
}
