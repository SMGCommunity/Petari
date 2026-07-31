#include "Game/Screen/GalaxyMap.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/GalaxyMapCometIcon.hpp"
#include "Game/Screen/GalaxyMapDomeIcon.hpp"
#include "Game/Screen/GalaxyMapGalaxyDetail.hpp"
#include "Game/Screen/GalaxyMapGalaxyPlain.hpp"
#include "Game/Screen/GalaxyMapIcon.hpp"
#include "Game/Screen/GalaxyMapMarioIcon.hpp"
#include "Game/Screen/GalaxyMapTicoIcon.hpp"
#include "Game/Screen/GalaxyMapTitle.hpp"
#include "Game/Screen/IconAButton.hpp"
#include "Game/Screen/LayoutManager.hpp"
#include "Game/System/GameEventFlagTable.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <algorithm>
#include <cstdio>

namespace {
    static const s32 sFocusKeepFrame = 30;
    static const f32 sPointingRange = 20.0f;
};  // namespace

namespace {
    NEW_NERVE(GalaxyMapIdle, GalaxyMap, Idle);
    NEW_NERVE(GalaxyMapFocusIn, GalaxyMap, FocusIn);
    NEW_NERVE(GalaxyMapFocusOut, GalaxyMap, FocusOut);
    NEW_NERVE(GalaxyMapFadeinGalaxyMap, GalaxyMap, FadeinGalaxyMap);
    NEW_NERVE(GalaxyMapFadeinAstroMap, GalaxyMap, FadeinAstroMap);
    NEW_NERVE(GalaxyMapShowDetail, GalaxyMap, ShowDetail);
};  // namespace

GalaxyMap::GalaxyMap()
    : LayoutActor("ギャラクシー・天文台マップ", true), mMarioIcon1(), mMarioIcon2(), mGalaxyPlain(), mGalaxyDetail(), mPointingIcon(), mGalaxyName(),
      mIconAButton(), _70(), _71(true) {
}

void GalaxyMap::init(const JMapInfoIter& rIter) {
    initLayoutManager("MapGrandGalaxy", 1);
    initPaneCtrlPointing();
    initDomeIcon();
    initMarioIcon();
    initTicoIcon();
    initNerve(&::GalaxyMapIdle::sInstance);

    mGalaxyPlain = new GalaxyMapGalaxyPlain(this);
    mGalaxyPlain->initWithoutIter();

    mGalaxyDetail = new GalaxyMapGalaxyDetail();
    mGalaxyDetail->initWithoutIter();

    mTitle = new GalaxyMapTitle();
    mTitle->initWithoutIter();

    mIconAButton = new IconAButton(false, false);
    mIconAButton->initWithoutIter();
    mIconAButton->setFollowActorPane(this, "AButtonPosition");
    mIconAButton->kill();

    kill();
}

void GalaxyMap::appear() {
    LayoutActor::appear();

    std::for_each(mDomeIcon.begin(), mDomeIcon.end(), std::mem_func(&LayoutActor::appear));
    std::for_each(mIcon.begin(), mIcon.end(), std::mem_func(&LayoutActor::appear));
    std::for_each(mCometIcon.begin(), mCometIcon.end(), std::mem_func(&LayoutActor::appear));
    std::for_each(mTicoIcon.begin(), mTicoIcon.end(), std::mem_func(&LayoutActor::appear));

    mMarioIcon1->showBlink();

    if (mMarioIcon2 != nullptr) {
        mMarioIcon2->showBlink();
    }

    MR::setTextBoxNumberRecursive(this, "Star", MR::getPowerStarNum());
}

void GalaxyMap::kill() {
    LayoutActor::kill();

    std::for_each(mDomeIcon.begin(), mDomeIcon.end(), std::mem_func(&LayoutActor::kill));
    std::for_each(mIcon.begin(), mIcon.end(), std::mem_func(&LayoutActor::kill));
    std::for_each(mCometIcon.begin(), mCometIcon.end(), std::mem_func(&LayoutActor::kill));
    std::for_each(mTicoIcon.begin(), mTicoIcon.end(), std::mem_func(&LayoutActor::kill));

    mGalaxyPlain->kill();
    mGalaxyDetail->kill();
    mMarioIcon1->kill();
    mTitle->kill();
    mIconAButton->kill();

    if (mMarioIcon2 != nullptr) {
        mMarioIcon2->kill();
    }
}

void GalaxyMap::movement() {
    LayoutActor::movement();

    if (!isNerve(&::GalaxyMapShowDetail::sInstance)) {
        std::for_each(mDomeIcon.begin(), mDomeIcon.end(), std::mem_func(&LayoutActor::movement));
        std::for_each(mIcon.begin(), mIcon.end(), std::mem_func(&LayoutActor::movement));
        std::for_each(mCometIcon.begin(), mCometIcon.end(), std::mem_func(&LayoutActor::movement));
        std::for_each(mTicoIcon.begin(), mTicoIcon.end(), std::mem_func(&LayoutActor::movement));

        mMarioIcon1->movement();

        if (mMarioIcon2 != nullptr) {
            mMarioIcon2->movement();
        }
    }

    mGalaxyDetail->movement();
    mGalaxyPlain->movement();
    mTitle->movement();
    mIconAButton->movement();
}

void GalaxyMap::calcAnim() {
    LayoutActor::calcAnim();

    std::for_each(mDomeIcon.begin(), mDomeIcon.end(), std::mem_func(&LayoutActor::calcAnim));
    std::for_each(mIcon.begin(), mIcon.end(), std::mem_func(&LayoutActor::calcAnim));
    std::for_each(mCometIcon.begin(), mCometIcon.end(), std::mem_func(&LayoutActor::calcAnim));
    std::for_each(mTicoIcon.begin(), mTicoIcon.end(), std::mem_func(&LayoutActor::calcAnim));

    mMarioIcon1->calcAnim();
    mGalaxyDetail->calcAnim();
    mGalaxyPlain->calcAnim();
    mTitle->calcAnim();
    mIconAButton->calcAnim();

    if (mMarioIcon2 != nullptr) {
        mMarioIcon2->calcAnim();
    }
}

void GalaxyMap::draw() const {
    if (!MR::isExecuteDrawLayout(this)) {
        return;
    }

    LayoutActor::draw();

    std::for_each(mDomeIcon.begin(), mDomeIcon.end(), std::mem_func(&GalaxyMapDomeIcon::draw));
    mMarioIcon1->draw();
    drawGalaxyIconWithOrder(1);
    mTitle->draw();
    std::for_each(mTicoIcon.begin(), mTicoIcon.end(), std::mem_func(&GalaxyMapTicoIcon::draw));

    if (mMarioIcon2 != nullptr) {
        mMarioIcon2->draw();
    }

    drawGalaxyIconWithOrder(3);
    std::for_each(mCometIcon.begin(), mCometIcon.end(), std::mem_func(&GalaxyMapCometIcon::draw));
    drawGalaxyIconWithOrder(2);
    mIconAButton->draw();
}

void GalaxyMap::movementForCapture() {
    if (mLayoutManager != nullptr && !MR::isStopAnimFrame(this)) {
        getLayoutManager()->movement();
    }

    if (isNerve(&::GalaxyMapShowDetail::sInstance)) {
        return;
    }

    std::for_each(mIcon.begin(), mIcon.end(), std::mem_func(&GalaxyMapIcon::movement));
    std::for_each(mCometIcon.begin(), mCometIcon.end(), std::mem_func(&GalaxyMapCometIcon::movement));
}

void GalaxyMap::calcAnimForCapture(const nw4r::lyt::DrawInfo& rDrawInfo) {
    MR::calcAnimLayoutWithDrawInfo(this, rDrawInfo);
    std::for_each(mDomeIcon.begin(), mDomeIcon.end(), std::bind2nd(std::mem_func(&GalaxyMapDomeIcon::calcAnimForCapture), rDrawInfo));
    std::for_each(mIcon.begin(), mIcon.end(), std::bind2nd(std::mem_func(&GalaxyMapIcon::calcAnimForCapture), rDrawInfo));
    std::for_each(mCometIcon.begin(), mCometIcon.end(), std::bind2nd(std::mem_func(&GalaxyMapCometIcon::calcAnimForCapture), rDrawInfo));
}

void GalaxyMap::drawForCapture(const nw4r::lyt::DrawInfo& rDrawInfo) {
    // FIXME: bind2nd is inlining here very stubbornly.
    MR::drawLayoutWithDrawInfoWithoutProjectionSetup(this, rDrawInfo);
    std::for_each(mDomeIcon.begin(), mDomeIcon.end(), std::bind2nd(std::ptr_fun(MR::drawLayoutWithDrawInfoWithoutProjectionSetup), rDrawInfo));
    std::for_each(mIcon.begin(), mIcon.end(), std::bind2nd(std::ptr_fun(MR::drawLayoutWithDrawInfoWithoutProjectionSetup), rDrawInfo));
    std::for_each(mCometIcon.begin(), mCometIcon.end(), std::bind2nd(std::ptr_fun(MR::drawLayoutWithDrawInfoWithoutProjectionSetup), rDrawInfo));
}

void GalaxyMap::setModeNormal() {
    std::for_each(mIcon.begin(), mIcon.end(), std::mem_func(&GalaxyMapIcon::setModeNormal));
    std::for_each(mTicoIcon.begin(), mTicoIcon.end(), std::mem_func(&GalaxyMapTicoIcon::setModeNormal));
    std::for_each(mDomeIcon.begin(), mDomeIcon.end(), std::mem_func(&GalaxyMapDomeIcon::setModeNormal));
    mTitle->setModeNormal();

    _71 = true;
}

void GalaxyMap::setModeNewGalaxyDiscover() {
    std::for_each(mIcon.begin(), mIcon.end(), std::mem_func(&GalaxyMapIcon::setModeNewGalaxyDiscover));
    std::for_each(mTicoIcon.begin(), mTicoIcon.end(), std::mem_func(&GalaxyMapTicoIcon::setModeNormal));
    std::for_each(mDomeIcon.begin(), mDomeIcon.end(), std::mem_func(&GalaxyMapDomeIcon::setModeNormal));
    mTitle->setModeHideInformation();

    _71 = false;
}

void GalaxyMap::setModeNewTicoGalaxyDiscover() {
    std::for_each(mIcon.begin(), mIcon.end(), std::mem_func(&GalaxyMapIcon::setModeNormal));
    std::for_each(mTicoIcon.begin(), mTicoIcon.end(), std::mem_func(&GalaxyMapTicoIcon::setModeDiscover));
    std::for_each(mDomeIcon.begin(), mDomeIcon.end(), std::mem_func(&GalaxyMapDomeIcon::setModeNormal));
    mTitle->setModeHideInformation();

    _71 = false;
}

void GalaxyMap::setModeNewDomeDiscover() {
    std::for_each(mIcon.begin(), mIcon.end(), std::mem_func(&GalaxyMapIcon::setModeNormal));
    std::for_each(mTicoIcon.begin(), mTicoIcon.end(), std::mem_func(&GalaxyMapTicoIcon::setModeNormal));
    std::for_each(mDomeIcon.begin(), mDomeIcon.end(), std::mem_func(&GalaxyMapDomeIcon::setModeNewDomeDiscover));
    mTitle->setModeHideInformation();

    _71 = false;
}

void GalaxyMap::setModeTicoCometPray() {
    std::for_each(mIcon.begin(), mIcon.end(), std::mem_func(&GalaxyMapIcon::setModeNormal));
    std::for_each(mTicoIcon.begin(), mTicoIcon.end(), std::mem_func(&GalaxyMapTicoIcon::setModeNormal));
    std::for_each(mDomeIcon.begin(), mDomeIcon.end(), std::mem_func(&GalaxyMapDomeIcon::setModeNormal));
    mTitle->setModeHideInformation();

    _71 = false;
}

void GalaxyMap::setModeCapture() {
    appear();
    forceToGalaxyMap();
    mTitle->kill();
    mMarioIcon1->kill();
    mIconAButton->kill();

    if (mMarioIcon2 != nullptr) {
        mMarioIcon2->kill();
    }
}

void GalaxyMap::drawGalaxyInfo() const {
    if (!MR::isExecuteDrawLayout(this)) {
        return;
    }

    mGalaxyPlain->draw();
    mGalaxyDetail->draw();
}

bool GalaxyMap::isTransition() const {
    return isNerve(&::GalaxyMapFadeinAstroMap::sInstance) || isNerve(&::GalaxyMapFadeinGalaxyMap::sInstance);
}

bool GalaxyMap::isPointingAnything() const {
    return mPointingIcon != nullptr;
}

bool GalaxyMap::isShowDetail() const {
    return !MR::isDead(this) && isNerve(&::GalaxyMapShowDetail::sInstance);
}

void GalaxyMap::dispIconAButton() {
    mIconAButton->openWithoutMessage();
}

void GalaxyMap::changeToGalaxyMap() {
    appear();
    setNerve(&::GalaxyMapFadeinGalaxyMap::sInstance);
}

void GalaxyMap::changeToAstroMap() {
    appear();
    setNerve(&::GalaxyMapFadeinAstroMap::sInstance);
}

void GalaxyMap::forceToGalaxyMap() {
    appear();
    MR::startAnim(this, "DomeIn", 0);
    MR::setAnimFrameAndStopAtEnd(this, 0);
    mTitle->startGalaxyMap();
    setNerve(&::GalaxyMapIdle::sInstance);
}

void GalaxyMap::forceToAstroMap() {
    appear();
    MR::startAnim(this, "DomeOut", 0);
    MR::setAnimFrameAndStopAtEnd(this, 0);
    mTitle->startAstroMap();
    setNerve(&::GalaxyMapIdle::sInstance);
}

void GalaxyMap::exeIdle() {
    tryFocusIn();
}

void GalaxyMap::exeFocusIn() {
    mPointingIcon->onPointing();
    updateGalaxyName();

    if (tryFocusIn()) {
        return;
    }
}

void GalaxyMap::exeFocusOut() {
    updateGalaxyName();

    if (tryFocusIn()) {
        return;
    }

    if (MR::isGreaterStep(this, ::sFocusKeepFrame)) {
        mGalaxyName = nullptr;

        setNerve(&::GalaxyMapIdle::sInstance);
    }
}

void GalaxyMap::exeFadeinGalaxyMap() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "DomeIn", 0);
        mTitle->changeToGalaxyMap();
    }

    if (MR::isAnimStopped(this, 0)) {
        mGalaxyName = nullptr;
        mPointingIcon = nullptr;

        setNerve(&::GalaxyMapIdle::sInstance);
    }
}

void GalaxyMap::exeFadeinAstroMap() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "DomeOut", 0);
        mTitle->changeToAstroMap();
    }

    if (MR::isAnimStopped(this, 0)) {
        mGalaxyName = nullptr;
        mPointingIcon = nullptr;

        setNerve(&::GalaxyMapIdle::sInstance);
    }
}

void GalaxyMap::exeShowDetail() {
    if (MR::isFirstStep(this)) {
        mGalaxyDetail->startDisplay(mGalaxyName, _70);
        std::for_each(mIcon.begin(), mIcon.end(), std::mem_func(&GalaxyMapIcon::deactivatePointing));
    }

    if (MR::isDead(mGalaxyDetail)) {
        std::for_each(mIcon.begin(), mIcon.end(), std::mem_func(&GalaxyMapIcon::activatePointing));
        setNerve(&::GalaxyMapIdle::sInstance);
    }
}

// GalaxyMap::initPaneCtrlPointing

namespace {
    const char* const cDomes[] = {
        "Dome1", "Dome2", "Dome3", "Dome4", "Dome5", "Dome6",
    };
    const char* const cDomeShowHidePanes[] = {
        "Base1", "Base2", "Base3", "Base4", "Base5", "Base6",
    };
    const char* const cMarioIconPositions[] = {
        "Mario7", "Mario1", "Mario2", "Mario3", "Mario4", "Mario5", "Mario6",
    };
    const char* const cMarioIconPositionsInDome[] = {
        nullptr, "GMario1", "GMario2", "GMario3", "GMario4", "GMario5", "GMario6",
    };
};  // namespace

void GalaxyMap::initDomeIcon() {
    GalaxyMapDomeIcon* domeIcon;

    mDomeIcon.init(6);

    for (u32 i = 0; i < 6; i++) {
        domeIcon = new GalaxyMapDomeIcon(i + 1, this, ::cDomes[i], ::cDomeShowHidePanes[i]);
        domeIcon->initWithoutIter();

        mDomeIcon.push_back(domeIcon);
    }
}

void GalaxyMap::initMarioIcon() {
    GalaxyMapMarioIcon* marioIcon1;
    GalaxyMapMarioIcon* marioIcon2;
    s32 scenarioNo = 0;

    if (MR::isEqualStageName("AstroDome")) {
        scenarioNo = MR::getCurrentScenarioNo();
    }

    marioIcon1 = new GalaxyMapMarioIcon(this, ::cMarioIconPositions[scenarioNo]);
    marioIcon1->initWithoutIter();

    mMarioIcon1 = marioIcon1;

    if (::cMarioIconPositionsInDome[scenarioNo] != nullptr) {
        marioIcon2 = new GalaxyMapMarioIcon(this, ::cMarioIconPositionsInDome[scenarioNo]);
        marioIcon2->initWithoutIter();

        mMarioIcon2 = marioIcon2;
    }
}

void GalaxyMap::initTicoIcon() {
    GalaxyMapTicoIcon* ticoIcon;
    char exclamationGalaxyName[16];
    s32 exclamationGalaxyNum = GameEventFlagTable::calcExclamationGalaxyNum();

    mTicoIcon.init(exclamationGalaxyNum);

    for (s32 i = 0; i < exclamationGalaxyNum; i++) {
        snprintf(exclamationGalaxyName, sizeof(exclamationGalaxyName), "Tico%1d", i + 1);

        ticoIcon = new GalaxyMapTicoIcon(i, GameEventFlagTable::getExclamationGalaxyNameFromIndex(i), this, exclamationGalaxyName);
        ticoIcon->initWithoutIter();

        mTicoIcon.push_back(ticoIcon);
    }
}

bool GalaxyMap::tryFocusIn() {
    if (!_71) {
        mGalaxyName = nullptr;
        mPointingIcon = nullptr;

        setNerve(&::GalaxyMapIdle::sInstance);

        return true;
    }

    GalaxyMapIcon* pointingGalaxyIcon = getPointingGalaxyIcon();

    if (pointingGalaxyIcon != mPointingIcon) {
        const Nerve* nerve = &::GalaxyMapFocusOut::sInstance;

        if (pointingGalaxyIcon != nullptr) {
            nerve = &::GalaxyMapFocusIn::sInstance;
            mGalaxyName = pointingGalaxyIcon->mGalaxyName;
        }

        if (pointingGalaxyIcon != nullptr) {
            _70 = pointingGalaxyIcon->isStatusNew();

            MR::startSystemSE("SE_SY_GALAMAP_ICON_POINT");
        }

        mPointingIcon = pointingGalaxyIcon;

        setNerve(nerve);

        return true;
    }

    if (pointingGalaxyIcon != nullptr && MR::testCorePadTriggerA(WPAD_CHAN0)) {
        setNerve(&::GalaxyMapShowDetail::sInstance);

        return true;
    }

    return false;
}

void GalaxyMap::updateGalaxyName() {
    if (mPointingIcon != nullptr) {
        mGalaxyPlain->show(mGalaxyName, mPointingIcon->mPaneName);
    }
}

void GalaxyMap::drawGalaxyIconWithOrder(int param1) const {
    for (s32 i = 0; i < mIcon.size(); i++) {
        s32 value = 1;

        if (mPointingIcon != nullptr && mPointingIcon == mIcon[i]) {
            value = 3;
        }

        if (mIcon[i]->isNewOnDiscoverMode()) {
            value = 2;
        }

        if (value == param1) {
            mIcon[i]->draw();
        }
    }
}

GalaxyMapIcon* GalaxyMap::getPointingGalaxyIcon() const {
    for (s32 i = 0; i < mIcon.size(); i++) {
        GalaxyMapIcon* icon = mIcon[i];

        if (icon->isPointing()) {
            return icon;
        }
    }

    return nullptr;
}
