#include "Game/MapObj/CoconutTreeLeaf.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

void CoconutTreeLeaf::init(const JMapInfoIter& rIter) {
    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);
    MR::initCollisionPartsFromResourceHolder(this, "CoconutTreeLeaf", getSensor("body"), MR::getResourceHolder(mGroup), _D8);
    MR::invalidateClipping(this);
    makeActorAppeared();
}

// CoconutTreeLeaf::update

bool CoconutTreeLeaf::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerHipDropFloor(msg)) {
        TVec3f stack_8(_A8);
        stack_8.scale(-0.1f);
        mVelocity.add(stack_8);
        return true;
    }

    return false;
}

CoconutTreeLeafGroup::CoconutTreeLeafGroup(const char* pName) : LiveActor(pName), mNumLeaves(), mLeaves(), _94(0.0f, 0.0f, 1.0f), _A0(0.0f) {
}

void CoconutTreeLeafGroup::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("CoconutTreeLeaf", nullptr, false);
    MR::connectToSceneMapObjNoCalcAnim(this);
    mNumLeaves = MR::getJointNum(this) - 1;
    mLeaves = new CoconutTreeLeaf*[mNumLeaves];

    for (s32 i = 0; i < mNumLeaves; i++) {
        mLeaves[i] = new CoconutTreeLeaf(this, MR::getJointMtx(this, i + 1), &_94);
        mLeaves[i]->initWithoutIter();
    }

    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);
    MR::setClippingTypeSphere(this, 500.0f);
    makeActorAppeared();
}

void CoconutTreeLeafGroup::startClipped() {
    LiveActor::startClipped();

    for (s32 i = 0; i < mNumLeaves; i++) {
        mLeaves[i]->startClipped();
    }
}

void CoconutTreeLeafGroup::endClipped() {
    LiveActor::endClipped();

    for (s32 i = 0; i < mNumLeaves; i++) {
        mLeaves[i]->endClipped();
    }
}

// control

CoconutTreeLeaf::~CoconutTreeLeaf() {
}

CoconutTreeLeafGroup::~CoconutTreeLeafGroup() {
}

MtxPtr CoconutTreeLeaf::getBaseMtx() const {
    return _D8;
}
