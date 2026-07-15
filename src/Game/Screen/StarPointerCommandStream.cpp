#include "Game/Screen/StarPointerCommandStream.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/StarPointerDirector.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/Color.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Util/SystemUtil.hpp"

namespace {
    // static const _32 hFlashShiftNum =
    // static const _32 hTexSpeed =
    // static const _32 hTexRate =
    // static const _32 hDrawHalfWidth =
    static const f32 sMinWidth = 6.0f;
    static const f32 sMaxWidth = 32.0f;
    static const f32 sMinLineWidthDistance = 40.0f;
    static const f32 sMaxLineWidthDistance = 400.0f;
    static const f32 hDefaultLength = 80.0f;
    static const f32 hDefaultWidth = 30.0f;
    // static const _32 h16x9ScaleX =
};  // namespace

namespace NrvStarPointerCommandStream {
    NEW_NERVE(HostTypeNrvWait, StarPointerCommandStream, Wait);
    NEW_NERVE(HostTypeNrvSignal, StarPointerCommandStream, Signal);
};  // namespace NrvStarPointerCommandStream

namespace {
    static Color8 hOutColor = Color8((GXColor){0xFF, 0x00, 0x00, 0xFF});
};  // namespace

StarPointerCommandStream::StarPointerCommandStream(const TVec2f* pScreenPos)
    : LayoutActor("スターポインタ指示線", true), _20(false), mOffScreenTime(0), _28(0.0f), mScreenPos(pScreenPos), mWorldPos(nullptr),
      mPadChannel(-1), mColor(nullptr) {
}

void StarPointerCommandStream::start(const TVec3f* pPos, bool b) {
    mWorldPos = pPos;
    _20 = b;
    MR::showLayout(this);
    MR::startAnim(this, "Wait", 0);
    TVec2f screenPos;
    MR::calcScreenPosition(&screenPos, *mWorldPos);
    calcPose(screenPos, screenPos);
    calcAnim();
}

void StarPointerCommandStream::show() {
    MR::showLayout(this);
}

void StarPointerCommandStream::hide() {
    MR::setPaneScale(this, 0.0f, 0.0f, "PicRibbon");
    MR::hideLayout(this);
}

void StarPointerCommandStream::clear() {
    mWorldPos = nullptr;
    _20 = false;
}

void StarPointerCommandStream::init(const JMapInfoIter& rIter) {
    initLayoutManager("CommandStream", 1);
    MR::createAndAddPaneCtrl(this, "CommandStream", 1);
    MR::createAndAddPaneCtrl(this, "PicRibbon", 1);
    MR::createAndAddPaneCtrl(this, "PicArrow", 1);
    MR::createAndAddPaneCtrl(this, "Arrow", 1);
    MR::setFollowPos(&mFollowPos, this, "CommandStream");
    initNerve(&NrvStarPointerCommandStream::HostTypeNrvWait::sInstance);
    appear();
    MR::setPaneScale(this, 0.0f, 0.0f, "PicRibbon");
    MR::hideLayout(this);
}

void StarPointerCommandStream::control() {
    if (MR::isStarPointerInScreen(mPadChannel)) {
        mOffScreenTime = 0;
        _28 -= 0.05f;
    } else {
        mOffScreenTime++;
        if ((mOffScreenTime & 0x10) != 0 && (mOffScreenTime & 0x0F) == 0) {
            MR::startSystemSE("SE_SY_DPD_DRAG_RANGE_OUT");
        }
    }

    TVec2f screenPos;
    MR::calcScreenPosition(&screenPos, *mWorldPos);
    TVec2f insidePos = *mScreenPos;
    StarPointerFunction::forceInsideScreenEdge(&insidePos);
    calcPose(screenPos, insidePos);
}

void StarPointerCommandStream::calcPose(const TVec2f& rPos, const TVec2f& rInsidePos) {
    mFollowPos = rPos;
    TVec2f diff;
    diff.x = rInsidePos.x - rPos.x;
    diff.y = rInsidePos.y - rPos.y;

    if (MR::isScreen16Per9()) {
    }

    if (!MR::isNearZero(diff)) {
        f32 length;
        MR::separateScalarAndDirection(&length, &diff, diff);
        f32 lineWidth = calcLineWidth(length);
        length /= ::hDefaultLength;
        lineWidth /= ::hDefaultWidth;
        MR::setPaneScale(this, lineWidth, length, "PicRibbon");
        MR::setPaneRotate(this, 0.0f, 0.0f, MR::toDegree(MR::atan2(diff.x, diff.y)), "Arrow");
        return;
    }

    MR::setPaneScale(this, 0.0f, 0.0f, "PicRibbon");
}

void StarPointerCommandStream::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Wait", 0);
    }

    if (!MR::isStarPointerInScreen(mPadChannel)) {
        setNerve(&NrvStarPointerCommandStream::HostTypeNrvSignal::sInstance);
    }
}

void StarPointerCommandStream::exeSignal() {
    if (MR::isFirstStep(this)) {
    }

    if (MR::isStarPointerInScreen(mPadChannel)) {
        setNerve(&NrvStarPointerCommandStream::HostTypeNrvWait::sInstance);
    }
}

f32 StarPointerCommandStream::calcLineWidth(f32 length) const {
    f32 norm = MR::normalize(length, ::sMinLineWidthDistance, ::sMaxLineWidthDistance);
    return norm * ::sMinWidth + (1.0f - norm) * ::sMaxWidth;
}
