#include "Game/Screen/OneUpBoard.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"

namespace {
    static const f32 sOffsetY = 60.0f;
    static const f32 sOffsetYMin = 160.0f;
};  // namespace

namespace NrvOneUpBoard {
    NEW_NERVE(HostTypeWait, OneUpBoard, Wait);
    NEW_NERVE(HostTypeAppear, OneUpBoard, Appear);
};  // namespace NrvOneUpBoard

OneUpBoard::OneUpBoard() : LayoutActor("1UPボード", true) {}

void OneUpBoard::init(const JMapInfoIter& rIter) {
    initLayoutManager("OneUp", 2);
    initNerve(&NrvOneUpBoard::HostTypeWait::sInstance);
    MR::connectToSceneLayout(this);
    MR::registerDemoSimpleCastAll(this);
    initEffectKeeper(1, "OneUp", nullptr);
    kill();
}

void OneUpBoard::popup(s32 oneUpNum) {
    MR::setTextBoxArgNumberRecursive(this, "OneUp", oneUpNum, 0);
    appear();
}

void OneUpBoard::appear() {
    LayoutActor::appear();
    setNerve(&NrvOneUpBoard::HostTypeAppear::sInstance);
}

void OneUpBoard::calcPos() {
    TVec2f pos;

    MR::calcScreenPosition(&pos, *MR::getPlayerCenterPos());

    pos.y -= sOffsetY;
    pos.y = pos.y >= sOffsetYMin ? pos.y : sOffsetYMin;

    setTrans(pos);
}

void OneUpBoard::control() {}

void OneUpBoard::exeWait() {}

void OneUpBoard::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Appear", 0);
        MR::emitEffect(this, "OneUpGet");
    }

    calcPos();

    if (MR::isAnimStopped(this, 0)) {
        kill();
    }
}
