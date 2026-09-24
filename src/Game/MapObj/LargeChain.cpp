#include "Game/MapObj/LargeChain.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/LargeChainParts.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

void LargeChain_FORCE_MATCH_SDATA2() {
    (void)0.0f;
}

namespace {
    static const f32 sPartsLength = 200.0f;
    static const s32 sBreakInterval = 5;
};  // namespace

namespace NrvLargeChain {
    NEW_NERVE(LargeChainNrvWait, LargeChain, Wait);
    NEW_NERVE(LargeChainNrvBreak, LargeChain, Break);
};  // namespace NrvLargeChain

LargeChain::LargeChain(const char* pName) : LiveActor(pName), _8C(), _90(), mChainArray(), mChainCount(1) {
}

void LargeChain::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);

    s32 chainLen;
    MR::getJMapInfoArg0NoInit(rIter, &chainLen);
    mChainCount = static_cast< s32 >(chainLen / ::sPartsLength);

    createChainParts();

    MR::connectToSceneMapObjMovement(this);

    if (MR::tryRegisterDemoCast(this, rIter)) {
        MR::registerDemoActionNerve(this, GET_NERVE(LargeChain, LargeChainNrvBreak), nullptr);
    }

    MR::useStageSwitchSleep(this, rIter);

    initNerve(GET_NERVE(LargeChain, LargeChainNrvWait));

    makeActorAppeared();
}

void LargeChain::makeActorAppeared() {
    for (s32 i = 0; i < mChainCount; i++) {
        mChainArray[i].makeActorAppeared();
    }

    _8C->makeActorAppeared();
    _90->makeActorAppeared();
    LiveActor::makeActorAppeared();
}

void LargeChain::makeActorDead() {
    for (s32 i = 0; i < mChainCount; i++) {
        mChainArray[i].makeActorDead();
    }

    _8C->makeActorDead();
    _90->makeActorDead();
    LiveActor::makeActorDead();
}

void LargeChain::exeWait() {
}

void LargeChain::exeBreak() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(this);
        invalidateClippingAllChainParts();
    }

    if (getNerveStep() % ::sBreakInterval != 0) {
        return;
    }

    s32 step = getNerveStep() / ::sBreakInterval;

    if (step == 0) {
        _8C->breakChainParts();
    } else if (step == mChainCount - 1) {
        _90->breakChainParts();
    }

    mChainArray[step].breakChainParts();

    if (step == mChainCount - 1) {
        kill();
    }
}

namespace {
    inline LargeChainParts* createChainArray(u32 count) {
        return new LargeChainParts[count];
    }
}  // namespace

void LargeChain::createChainParts() {
    _8C = new LargeChainParts("固定鎖 下");
    _90 = new LargeChainParts("固定鎖 上");
    mChainArray = ::createChainArray(mChainCount);

    for (u32 i = 0; i < mChainCount; i++) {
        TVec3f pos(mPosition);
        TVec3f trans(0.0f, static_cast< f32 >(i), 0.0f);
        trans.y *= ::sPartsLength;
        pos.add(trans);

        mChainArray[i].initChainParts(&pos, &mRotation, &mScale, false);

        if (i == 0) {
            _8C->initChainParts(&pos, &mRotation, &mScale, true);
        } else if (i == mChainCount - 1) {
            TVec3f rot(mRotation);
            rot.y += 90.0f;
            rot.x += 180.0f;
            pos.y += 200.0f;
            _90->initChainParts(&pos, &rot, &mScale, true);
        }
    }
}

void LargeChain::invalidateClippingAllChainParts() {
    for (s32 i = 0; i < mChainCount; i++) {
        MR::invalidateClipping(&mChainArray[i]);
    }
}
