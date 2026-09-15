#include "Game/MapObj/ChipCounter.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/ChipBase.hpp"
#include "Game/MapObj/ChipHolder.hpp"
#include "Game/MapObj/CollectCounter.hpp"
#include "Game/NPC/TalkDirector.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

void ChipCounter_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

namespace {
    static const char* sChipPainName[] = {"Chip1", "Chip2", "Chip3", "Chip4", "Chip5"};
    static const s32 sShowTime = 0;
    static s32 sChipPainCount = ARRAY_SIZE(sChipPainName);
};  // namespace

namespace NrvChipCounter {
    NEW_NERVE(ChipCounterNrvHide, ChipCounter, Hide);
    NEW_NERVE(ChipCounterNrvFrameIn, ChipCounter, FrameIn);
    NEW_NERVE(ChipCounterNrvShow, ChipCounter, Show);
    NEW_NERVE(ChipCounterNrvFrameOut, ChipCounter, FrameOut);
    NEW_NERVE(ChipCounterNrvTryDemo, ChipCounter, TryDemo);
    NEW_NERVE(ChipCounterNrvComplete, ChipCounter, Complete);
    NEW_NERVE(ChipCounterNrvCompleteOut, ChipCounter, CompleteOut);
};  // namespace NrvChipCounter

ChipCounter::ChipCounter(const char* pName, s32 type) : LayoutActor(pName, true), mCollectCounter(), mCount(), mType(type), mGroupId(-1), _30(1.0f) {
}

void ChipCounter::init(const JMapInfoIter& rIter) {
    MR::connectToSceneLayout(this);

    switch (mType) {
    case ChipBase::Type_Blue:
        initLayoutManager("BlueChipCounter", 2);

        break;
    case ChipBase::Type_Yellow:
        initLayoutManager("YellowChipCounter", 2);

        break;
    }

    initNerve(GET_NERVE(ChipCounter, ChipCounterNrvHide));

    for (s32 i = 0; i < ::sChipPainCount; i++) {
        MR::createAndAddPaneCtrl(this, ::sChipPainName[i], 2);
        MR::startPaneAnim(this, ::sChipPainName[i], "ChipGet", 0);
    }

    MR::startAnim(this, "ShowHide", 1);
    MR::setAnimFrameAndStop(this, _30 * 20.0f, 1);

    mCollectCounter = new CollectCounter("集め数字");
    mCollectCounter->initWithoutIter();

    kill();
}

void ChipCounter::control() {
    if (MR::isActiveTalkBalloonShort()) {
        _30 -= 0.05f;

        if (_30 < 0.0f) {
            _30 = 0.0f;
        }
    } else {
        _30 += 0.05f;

        if (_30 > 1.0f) {
            _30 = 1.0f;
        }
    }

    MR::setAnimFrameAndStop(this, _30 * 20.0f, 1);
}

void ChipCounter::setCount(s32 count) {
    mCollectCounter->setCount(count);
    mCount = count;

    for (s32 i = 0; i < ::sChipPainCount; i++) {
        if (i < mCount - 1) {
            MR::setPaneAnimFrameAndStop(this, ::sChipPainName[i], 1.0f, 0);
            continue;
        }

        if (i == mCount - 1) {
            if (i == ::sChipPainCount - 1) {
                MR::setPaneAnimFrameAndStop(this, ::sChipPainName[i], 1.0f, 0);
                continue;
            }

            MR::startPaneAnim(this, ::sChipPainName[i], "ChipGet", 0);

            continue;
        }

        MR::setPaneAnimFrameAndStop(this, ::sChipPainName[i], 0.0f, 0);
    }
}

inline bool ChipCounter::isHidden() {
    return isNerve(GET_NERVE(ChipCounter, ChipCounterNrvHide)) || isNerve(GET_NERVE(ChipCounter, ChipCounterNrvFrameOut));
}

inline bool ChipCounter::isComplete() {
    return isNerve(GET_NERVE(ChipCounter, ChipCounterNrvTryDemo)) || isNerve(GET_NERVE(ChipCounter, ChipCounterNrvComplete)) ||
           isNerve(GET_NERVE(ChipCounter, ChipCounterNrvCompleteOut));
}

void ChipCounter::requestShow(s32 a1, s32 a2) {
    if (isHidden()) {
        appear();

        mCount = a2;

        for (s32 i = 0; i < ::sChipPainCount; i++) {
            if (i < mCount) {
                MR::setPaneAnimFrameAndStop(this, ::sChipPainName[i], 1.0f, 0);
            } else {
                MR::setPaneAnimFrameAndStop(this, ::sChipPainName[i], 0.0f, 0);
            }
        }

        setNerve(GET_NERVE(ChipCounter, ChipCounterNrvFrameIn));
    }

    mGroupId = a1;
}

void ChipCounter::requestComplete(s32 a1) {
    if (isHidden()) {
        appear();
    }

    mGroupId = a1;

    MR::requestStartDemoWithoutCinemaFrame(this, "チップコンプリート", GET_NERVE(ChipCounter, ChipCounterNrvComplete),
                                           GET_NERVE(ChipCounter, ChipCounterNrvTryDemo));
}

void ChipCounter::requestHide(s32 a1) {
    if (mGroupId != a1) {
        return;
    }

    if (!isHidden()) {
        setNerve(GET_NERVE(ChipCounter, ChipCounterNrvFrameOut));
    }
}

void ChipCounter::requestActive() {
    if (!isHidden()) {
        appear();
    }
}

void ChipCounter::requestDeactive() {
    if (!isComplete()) {
        kill();
        mCollectCounter->kill();
    }
}

bool ChipCounter::tryEndFrameIn() {
    if (MR::isAnimStopped(this, 0)) {
        setNerve(GET_NERVE(ChipCounter, ChipCounterNrvShow));
        return true;
    }

    return false;
}

bool ChipCounter::tryEndFrameOut() {
    if (MR::isAnimStopped(this, 0)) {
        setNerve(GET_NERVE(ChipCounter, ChipCounterNrvHide));

        kill();

        return true;
    }

    return false;
}

bool ChipCounter::tryEndComplete() {
    if (MR::isAnimStopped(this, 0)) {
        setNerve(GET_NERVE(ChipCounter, ChipCounterNrvCompleteOut));
        return true;
    }

    return false;
}

void ChipCounter::exeHide() {
}

void ChipCounter::exeFrameIn() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Appear", 0);
    }

    tryEndFrameIn();
}

void ChipCounter::exeShow() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Wait", 0);
    }
}

void ChipCounter::exeFrameOut() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "End", 0);
    }

    tryEndFrameOut();
}

void ChipCounter::exeTryDemo() {
    if (MR::isFirstStep(this)) {
        MR::hideLayout(this);
    }
}

void ChipCounter::exeComplete() {
    if (MR::isFirstStep(this)) {
        for (s32 i = 0; i < ::sChipPainCount; i++) {
            MR::setPaneAnimFrameAndStop(this, ::sChipPainName[i], 1.0f, 0);
        }

        MR::showLayout(this);
        MR::startAnim(this, "Complete", 0);

        MR::requestMovementOn(mCollectCounter);
    }

    tryEndComplete();
}

void ChipCounter::exeCompleteOut() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "End", 0);
    }

    if (tryEndFrameOut()) {
        setNerve(GET_NERVE(ChipCounter, ChipCounterNrvHide));

        mCollectCounter->kill();

        MR::endDemo(this, "チップコンプリート");

        MR::noticeEndChipCompleteDemo(mType, mGroupId);
    }
}
