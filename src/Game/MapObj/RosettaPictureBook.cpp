#include "Game/MapObj/RosettaPictureBook.hpp"
#include "Game/Screen/IconAButton.hpp"
#include "Game/Screen/PictureBookLayout.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    const s32 hFadeOutFrame = 60;
    const s32 hFadeInFrame = 60;
};

namespace NrvRosettaPictureBook {
    NEW_NERVE(HostTypeNrvWait, RosettaPictureBook, Wait);
    NEW_NERVE(HostTypeNrvDemoWait, RosettaPictureBook, DemoWait);
    NEW_NERVE(HostTypeNrvFadeOut, RosettaPictureBook, FadeOut);
    NEW_NERVE(HostTypeNrvReading, RosettaPictureBook, Reading);
    NEW_NERVE(HostTypeNrvFadeIn, RosettaPictureBook, FadeIn);
};

RosettaPictureBook::RosettaPictureBook(const char* pName) :
    LiveActor(pName),
    mLayout(nullptr),
    mIconAButton(nullptr),
    mIsValidOpenIconAButton(false)
{
    
}

void RosettaPictureBook::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("RosettaPictureBook", nullptr, false);
    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    MR::addHitSensorMapObjSimple(this, "body", 8, 150.0f, TVec3f(0.0f, 0.0f, 0.0f));

    mLayout = new PictureBookLayout(1, MR::getPictureBookChapterCanRead(), false);
    mLayout->initWithoutIter();

    mIconAButton = new IconAButton(true, false);
    mIconAButton->initWithoutIter();

    initNerve(&NrvRosettaPictureBook::HostTypeNrvWait::sInstance);
    MR::tryRegisterDemoCast(this, rIter);
    makeActorAppeared();
}

void RosettaPictureBook::appear() {
    mIsValidOpenIconAButton = false;

    setNerve(&NrvRosettaPictureBook::HostTypeNrvWait::sInstance);
    LiveActor::appear();
}

void RosettaPictureBook::kill() {
    LiveActor::kill();
    mIconAButton->kill();
}

void RosettaPictureBook::makeArchiveList(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter) {
    PictureBookLayout::makeArchiveList(pCollector, 1, MR::getPictureBookChapterCanRead(), false);
}

void RosettaPictureBook::control() {
    mIsValidOpenIconAButton = false;
}

void RosettaPictureBook::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isOnGroundPlayer()) {
        mIsValidOpenIconAButton = true;
    }
}

void RosettaPictureBook::exeWait() {
    if (mIsValidOpenIconAButton) {
        if (!mIconAButton->isOpen()) {
            MR::startSystemSE("SE_SY_TALK_BUTTON_APPEAR", -1, -1);
            mIconAButton->openWithRead();
        }
    }
    else if (mIconAButton->isOpen()) {
        mIconAButton->term();
    }

    if (!mIsValidOpenIconAButton) {
        return;
    }

    if (!mIconAButton->isOpen()) {
        return;
    }

    if (!MR::testCorePadTriggerA(WPAD_CHAN0)) {
        return;
    }

    MR::requestStartDemoMarioPuppetableWithoutCinemaFrame(
        this,
        "ロゼッタ絵本デモ",
        &NrvRosettaPictureBook::HostTypeNrvFadeOut::sInstance,
        &NrvRosettaPictureBook::HostTypeNrvDemoWait::sInstance);
}

void RosettaPictureBook::exeDemoWait() {
    
}

void RosettaPictureBook::exeFadeOut() {
    if (MR::isFirstStep(this)) {
        MR::startSystemSE("SE_SY_TALK_START", -1, -1);
        MR::requestMovementOn(mIconAButton);
        mIconAButton->term();
        MR::closeWipeCircle(hFadeOutFrame);
        MR::stopStageBGM(hFadeOutFrame);
        MR::startBckPlayer("Wait", (const char*)nullptr);
    }

    if (MR::isGreaterStep(this, hFadeOutFrame)) {
        mLayout->appear();
        setNerve(&NrvRosettaPictureBook::HostTypeNrvReading::sInstance);
    }
}

void RosettaPictureBook::exeReading() {
    if (MR::isFirstStep(this)) {
        mLayout->appear();
    }

    if (MR::isDead(mLayout)) {
        MR::endDemo(this, "ロゼッタ絵本デモ");
        setNerve(&NrvRosettaPictureBook::HostTypeNrvFadeIn::sInstance);
    }
}

void RosettaPictureBook::exeFadeIn() {
    if (MR::isFirstStep(this)) {
        MR::openWipeCircle(hFadeInFrame);
    }

    if (MR::isGreaterStep(this, hFadeInFrame)) {
        setNerve(&NrvRosettaPictureBook::HostTypeNrvWait::sInstance);
    }
}
