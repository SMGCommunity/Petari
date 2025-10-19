#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/OneUpBoard.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"

namespace NrvOneUpBoard {
    NEW_NERVE(HostTypeWait, OneUpBoard, Wait);
    NEW_NERVE(HostTypeAppear, OneUpBoard, Appear);
};

OneUpBoard::OneUpBoard() :
    LayoutActor("1UPボード", true)
{
    
}

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

    pos.y -= 60.0f;
    pos.y = pos.y >= 160.0f
        ? pos.y
        : 160.0f;

    setTrans(pos);
}

void OneUpBoard::control() {
    
}

void OneUpBoard::exeWait() {
    
}

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
