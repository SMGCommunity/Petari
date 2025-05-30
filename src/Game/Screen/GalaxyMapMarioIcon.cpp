#include "Game/Screen/GalaxyMapMarioIcon.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"

GalaxyMapMarioIcon::GalaxyMapMarioIcon(LayoutActor* pParam1, const char* pParam2) :
    LayoutActor("マリオ", true),
    _20(pParam1),
    _24(pParam2)
{}

void GalaxyMapMarioIcon::init(const JMapInfoIter& rIter) {
    f32 animFrame;

    initLayoutManager("IconMario", 2);
    MR::setInfluencedAlphaToChild(this);
    MR::startAnim(this, "Luigi", 1);

    if (MR::isPlayerLuigi()) {
        animFrame = 1.0f;
    }
    else {
        animFrame = 0.0f;
    }

    MR::setAnimFrameAndStop(this, animFrame, 1);
}

void GalaxyMapMarioIcon::showBlink() {
    appear();
    MR::startAnim(this, "Wait", 0);
}

void GalaxyMapMarioIcon::control() {
    MR::setLayoutScalePosAtPaneScaleTrans(this, _20, _24);
    MR::setLayoutAlpha(this, MR::getPaneAlpha(_20, _24));
}
