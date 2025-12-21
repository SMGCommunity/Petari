#include "Game/NPC/RosettaReading.hpp"
#include "Game/Demo/DemoFunction.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/PictureBookLayout.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    const s32 cOverlayStartStep = 122;
    // const s32 cOverlayFrame

    s32 getChapterNumberMin() {
        return MR::getPictureBookChapterAlreadyRead() + 1;
    }

    s32 getChapterNumberMax() {
        return MR::getPictureBookChapterCanRead();
    }
};  // namespace

namespace NrvRosettaReading {
    NEW_NERVE(RosettaReadingNrvPictureBookBefore, RosettaReading, PictureBookBefore);
    NEW_NERVE(RosettaReadingNrvPictureBook, RosettaReading, PictureBook);
    NEW_NERVE(RosettaReadingNrvPictureBookAfter, RosettaReading, PictureBookAfter);
};  // namespace NrvRosettaReading

RosettaReading::RosettaReading(const char* pName) : LiveActor(pName), mPictureBookLayout(nullptr) {}

void RosettaReading::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("RosettaReading", nullptr, false);
    initSound(2, false);
    MR::connectToSceneNpc(this);
    MR::invalidateClipping(this);
    MR::initLightCtrl(this);
    MR::tryRegisterDemoCast(this, rIter);
    DemoFunction::tryCreateDemoTalkAnimCtrlForScene(this, rIter, "DemoRosettaReading", nullptr, 0, 0);
    DemoFunction::registerDemoTalkMessageCtrl(
        this, MR::createTalkCtrlDirectOnRootNodeAutomatic(
                  this, rIter,
                  getChapterNumberMax() == PictureBookLayout::getChapterMax() ? "LibraryRoom_RosettaReading003" : "LibraryRoom_RosettaReading000",
                  // FIXME: Vector is not initialized correctly.
                  TVec3f(0.0f, 0.0f, 0.0f), MR::getJointMtx(this, "Chin")));

    if (getChapterNumberMin() <= getChapterNumberMax()) {
        mPictureBookLayout = new PictureBookLayout(getChapterNumberMin(), getChapterNumberMax(), true);
        mPictureBookLayout->initWithoutIter();
    }

    initNerve(&NrvRosettaReading::RosettaReadingNrvPictureBookBefore::sInstance);
    appear();
}

void RosettaReading::appear() {
    LiveActor::appear();
    setNerve(&NrvRosettaReading::RosettaReadingNrvPictureBookBefore::sInstance);
}

void RosettaReading::kill() {
    LiveActor::kill();
}

void RosettaReading::makeArchiveList(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter) {
    if (getChapterNumberMin() <= getChapterNumberMax()) {
        PictureBookLayout::makeArchiveList(pCollector, getChapterNumberMin(), getChapterNumberMax(), true);
    }
}

void RosettaReading::exePictureBookBefore() {
    if (MR::isDemoPartActive("朗読開始") || MR::isDemoPartActive("ロゼッタ会話[開始]") || MR::isDemoPartLessEqualStep("絵本デモ開始", 60)) {
        MR::startSystemLevelSE("SE_SM_LV_TICO_WAIT_LIBRARY", -1, -1);
    }

    if (MR::isDemoPartStep("絵本デモ開始", 100)) {
        MR::startSound(this, "SE_SM_ROSETTA_BOOK_OPEN", -1, -1);
    }

    if (MR::isDemoPartFirstStep("絵本デモ開始")) {
        MR::stopStageBGM(120);
    }

    if (MR::isDemoPartLastStep("絵本デモ開始")) {
        MR::pauseTimeKeepDemo(this);
        setNerve(&NrvRosettaReading::RosettaReadingNrvPictureBook::sInstance);
    }
}

void RosettaReading::exePictureBook() {
    if (MR::isFirstStep(this)) {
        mPictureBookLayout->appear();
        MR::forceToScreenCinemaFrame();
    }

    if (MR::isDead(mPictureBookLayout)) {
        for (int c = getChapterNumberMin(); c <= getChapterNumberMax(); c++) {
            MR::setPictureBookChapterAlreadyRead(c);
        }

        MR::forceToFrameCinemaFrame();
        MR::resumeTimeKeepDemo(this);
        setNerve(&NrvRosettaReading::RosettaReadingNrvPictureBookAfter::sInstance);
    }
}

void RosettaReading::exePictureBookAfter() {
    if (MR::isDemoPartGreaterStep("絵本デモ終了", 30) || MR::isDemoPartActive("ロゼッタ会話[終了]")) {
        MR::startSystemLevelSE("SE_SM_LV_TICO_WAIT_LIBRARY", -1, -1);
    }

    if (MR::isDemoPartStep("絵本デモ終了", cOverlayStartStep)) {
        MR::overlayWithPreviousScreen(1);
    }

    if (MR::isTimeKeepDemoActive()) {
        return;
    }

    kill();
}
