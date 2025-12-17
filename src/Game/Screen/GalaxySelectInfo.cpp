#include "Game/Screen/GalaxySelectInfo.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Screen/GalaxyInfoLayoutSetter.hpp"
#include "Game/Screen/IconComet.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "Game/Util/ValueControl.hpp"

namespace {
    const s32 cFadeinoutFrame = 5;
};  // namespace

namespace NrvGalaxySelectInfo {
    NEW_NERVE(GalaxySelectInfoNrvWait, GalaxySelectInfo, Wait);
    NEW_NERVE(GalaxySelectInfoNrvDisappear, GalaxySelectInfo, Disappear);
};  // namespace NrvGalaxySelectInfo

GalaxySelectInfo::GalaxySelectInfo(bool param1)
    : LayoutActor("ギャラクシー選択情報", true), _20(param1), _24(nullptr), _28(false), mFadeinoutControl(nullptr), mInfoLayoutSetter(nullptr) {
    mFadeinoutControl = new ValueControl(cFadeinoutFrame);
    mFadeinoutControl->setZero();
}

void GalaxySelectInfo::init(const JMapInfoIter& rIter) {
    initLayoutManager("GalaxyInfo", GalaxyInfoLayoutSetter::getAnimLayerNum());
    MR::setInfluencedAlphaToChild(this);

    mInfoLayoutSetter = new GalaxyInfoLayoutSetter(this);

    if (_20) {
        MR::connectToSceneLayoutOnPause(this);
        MR::connectToSceneLayoutOnPauseMovementCalcAnim(mInfoLayoutSetter->mIconComet);
    } else {
        MR::connectToSceneLayout(this);
        MR::connectToSceneLayoutMovementCalcAnim(mInfoLayoutSetter->mIconComet);
    }

    initNerve(&NrvGalaxySelectInfo::GalaxySelectInfoNrvWait::sInstance);
}

void GalaxySelectInfo::draw() const {
    LayoutActor::draw();
    mInfoLayoutSetter->mIconComet->draw();
}

void GalaxySelectInfo::appear() {
    MR::startAnim(this, "Appear", 0);
    LayoutActor::appear();
    mInfoLayoutSetter->updateCometPos();
}

void GalaxySelectInfo::kill() {
    LayoutActor::kill();
    mInfoLayoutSetter->mIconComet->kill();
}

bool GalaxySelectInfo::show(const char* pParam1, u32 param2, int param3) {
    return show(pParam1, param2, true, param3);
}

void GalaxySelectInfo::makeArchiveList(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter) {
    pCollector->addArchive("GalaxyInfo");
    pCollector->addArchive("GalaxyInfoTexture");
    pCollector->addArchive("IconComet");
}

void GalaxySelectInfo::control() {
    _28 = false;

    mFadeinoutControl->update();

    if (mFadeinoutControl->mFrame == 0) {
        kill();
    }

    MR::setLayoutAlphaFloat(this, mFadeinoutControl->getValue());

    LayoutActor* pActor = mInfoLayoutSetter->mIconComet;
    MR::setLayoutAlphaFloat(pActor, mFadeinoutControl->getValue());

    mInfoLayoutSetter->updateCometPos();
}

bool GalaxySelectInfo::show(const char* pParam1, u32 param2, bool param3, int param4) {
    _28 = true;

    if (MR::isDead(this) || !MR::isEqualString(_24, pParam1)) {
        if (param3) {
            mInfoLayoutSetter->updateInfo(pParam1, GalaxyInfoLayoutSetter::GalaxyState(param2), param4);
        } else {
            mInfoLayoutSetter->updateInfo(pParam1, GalaxyInfoLayoutSetter::GalaxyState(param2));
        }

        _24 = pParam1;

        mFadeinoutControl->setDirToOne();
        setNerve(&NrvGalaxySelectInfo::GalaxySelectInfoNrvWait::sInstance);
        appear();

        return true;
    }

    return false;
}

void GalaxySelectInfo::exeWait() {
    if (_28) {
        return;
    }

    mFadeinoutControl->setDirToZero();
    setNerve(&NrvGalaxySelectInfo::GalaxySelectInfoNrvDisappear::sInstance);
}

void GalaxySelectInfo::exeDisappear() {
    if (mFadeinoutControl->mFrame == 0) {
        kill();
    }
}
