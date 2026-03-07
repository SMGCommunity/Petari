#include "Game/Screen/GalaxyMapGalaxyDetail.hpp"
#include "Game/Screen/GalaxyInfoLayoutSetter.hpp"
#include "Game/Screen/IconAButton.hpp"
#include "Game/Screen/IconComet.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include <cstddef>

namespace {
    NEW_NERVE(GalaxyMapGalaxyDetailAppear, GalaxyMapGalaxyDetail, Appear);
    NEW_NERVE(GalaxyMapGalaxyDetailDisplay, GalaxyMapGalaxyDetail, Display);
    NEW_NERVE(GalaxyMapGalaxyDetailDisappear, GalaxyMapGalaxyDetail, Disappear);
};  // namespace

GalaxyMapGalaxyDetail::GalaxyMapGalaxyDetail() : LayoutActor("GalaxyDetail", true) {
    mInfoLayoutSetter = nullptr;
}

void GalaxyMapGalaxyDetail::init(const JMapInfoIter& rIter) {
    u32 layerNum = GalaxyInfoLayoutSetter::getAnimLayerNum();
    initLayoutManager("GalaxyDetail", layerNum);
    mInfoLayoutSetter = new GalaxyInfoLayoutSetter(this);
    mAButtonIcon = MR::createAndSetupIconAButton(this, false, false);
    initNerve(&GalaxyMapGalaxyDetailAppear::sInstance);
    kill();
}

void GalaxyMapGalaxyDetail::appear() {
    LayoutActor::appear();
    mInfoLayoutSetter->updateCometPos();
    setNerve(&GalaxyMapGalaxyDetailAppear::sInstance);
}

void GalaxyMapGalaxyDetail::kill() {
    LayoutActor::kill();
    mInfoLayoutSetter->mIconComet->kill();
    mAButtonIcon->kill();
}

void GalaxyMapGalaxyDetail::movement() {
    LayoutActor::movement();
    mInfoLayoutSetter->mIconComet->movement();
    mAButtonIcon->movement();
}

void GalaxyMapGalaxyDetail::calcAnim() {
    LayoutActor::calcAnim();
    mInfoLayoutSetter->updateCometPos();
    mInfoLayoutSetter->mIconComet->calcAnim();
    mAButtonIcon->calcAnim();
}

void GalaxyMapGalaxyDetail::draw() const {
    LayoutActor::draw();
    mInfoLayoutSetter->mIconComet->draw();
    mAButtonIcon->draw();
}

void GalaxyMapGalaxyDetail::startDisplay(const char* a1, bool a2) {
    enum {
        STATE2 = 2,
        STATE3 = 3,
    };

    u32 state = (a2) ? STATE2 : STATE3; //the enum is needed to prevent the ternary from optimizing itself 
    mInfoLayoutSetter->updateInfo(a1, GalaxyInfoLayoutSetter::GalaxyState(state));
    MR::setTextBoxNumberRecursive(this, "ShaCoinNum", MR::getCoinBestScore(a1));
    appear();
}

void GalaxyMapGalaxyDetail::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Appear", 0);
        MR::startSystemSE("SE_SY_GALAMAP_WINDOW_OPEN", -1, -1);
    }

    MR::setNerveAtAnimStopped(this, &GalaxyMapGalaxyDetailDisplay::sInstance, 0);
}

void GalaxyMapGalaxyDetail::exeDisplay() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Wait", 0);
        mAButtonIcon->openWithoutMessage();
    }

    if (MR::testCorePadTriggerA(WPAD_CHAN0)) {
        setNerve(&GalaxyMapGalaxyDetailDisappear::sInstance);
    }
}

void GalaxyMapGalaxyDetail::exeDisappear() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "End", 0);
        mAButtonIcon->term();
        MR::startSystemSE("SE_SY_GALAMAP_WINDOW_CLOSE", -1, -1);
    }
    
    if (MR::isAnimStopped(this, 0)) {
        kill();
    }
}

void GalaxyMapGalaxyDetail::control() {

}
