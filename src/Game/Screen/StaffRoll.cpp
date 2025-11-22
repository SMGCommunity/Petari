#include "Game/Camera/CameraContext.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Screen/InformationMessage.hpp"
#include "Game/Screen/StaffRoll.hpp"
#include "Game/System/GameSequenceFunction.hpp"
#include "Game/System/GameSystemFunction.hpp"
#include "Game/System/Language.hpp"
#include "Game/Util/DrawUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <cstdio>

#define PICTURE_TEXTURE_NUM 20
#define LINE_NUM 64

namespace {
    struct StaffRollParam {
        /* 0x00 */ f32 mPaneScale;
        /* 0x04 */ f32 mScrollSpeed;
        /* 0x08 */ f32 mLineLeading;
        /* 0x0C */ f32 mEmptyLineLeading;
    };

    struct PictureTiming {
        /* 0x00 */ s32 mAppearStep;
        /* 0x04 */ s32 mReplaceTextureId;
        /* 0x08 */ s32 mPaneId;
        /* 0x0C */ s32 mAnimId;
    };

    static const StaffRollParam sStaffRollParamTable[] = {
        {0.8f, 0.5f, 27.5f, 36.0f},
        {0.8f, 0.5f, 24.5f, 29.5f},
        {0.8f, 0.5f, 23.5f, 30.5f},
        {0.85f, 0.5f, 27.5f, 32.5f},
        {0.8f, 0.5f, 24.0f, 30.5f},
    };
    static const PictureTiming sPictureTimingTable[] = {
        {1100, 0, 2, 0},
        {1900, 0, 2, 1},
        {2000, 1, 1, 0},
        {2800, 1, 1, 1},
        {2900, 2, 0, 0},
        {3700, 2, 0, 1},
        {3800, 3, 3, 0},
        {4600, 3, 3, 1},
        {4700, 4, 2, 0},
        {5500, 4, 2, 1},
        {6100, 5, 0, 2},
        {6600, 5, 0, 3},
        {6100, 6, 3, 2},
        {6600, 6, 3, 3},
        {6700, 7, 1, 2},
        {7200, 7, 1, 3},
        {6700, 8, 2, 2},
        {7200, 8, 2, 3},
        {7300, 9, 0, 2},
        {7800, 9, 0, 3},
        {7300, 10, 3, 2},
        {7800, 10, 3, 3},
        {7900, 11, 1, 2},
        {8300, 11, 1, 3},
        {7900, 12, 2, 2},
        {8300, 12, 2, 3},
        {8500, 13, 0, 2},
        {8900, 13, 0, 3},
        {8500, 14, 3, 2},
        {8900, 14, 3, 3},
        {9400, 15, 3, 0},
        {10200, 15, 3, 1},
        {10300, 16, 2, 0},
        {11100, 16, 2, 1},
        {11200, 17, 1, 0},
        {12000, 17, 1, 1},
        {12090, 18, 0, 0},
        {12890, 18, 0, 1},
        {12990, 19, 3, 0},
        {13990, 19, 3, 1},
    };
    const char* cStaffRollDemoName = "StaffRoll";
    const char* cSaveAfterStaffRollDemoName = "SaveAfterStaffRoll";
    const char* cReplaceTextureArcName = "StaffRollTexture.arc";
    const char* cReplaceTexturePrefixHalf = "MStaffRollTexture";
    const char* cReplaceTexturePrefixComp = "LStaffRollTexture";
    const char* cStaffRollMsgId = "StaffList000";
    const char* cPaneNameText = "Text00";
    const char* cPaneNameTitleLogo = "TitleLogo";
    const char* cPaneNameTheEnd = "TheEnd";
    const char* cPaneNameTable[] = {
        "PictureLU",
        "PictureRU",
        "PictureLD",
        "PictureRD",
    };
    const char* cAnimNameTable[] = {
        "FadeIn",
        "FadeOut",
        "FrameIn",
        "FrameOut",
    };
    // static const s32 sStepForStartMargin = _;
    static const s32 sStepForPrepareInfo = 100;
    static const s32 sStepForTitleLogoAppear = 60;
    // static const s32 sStepToEndLogoStop = _;
    static const s32 sStepToThankYouVoice = 190;
    // static const _ sTitleLogoHeight = _;
    // static const _ sDoubleInterval = _;
    // static const _ sTripleInterval = _;
    static const f32 sEndLogoPosY = 220.0f;
}; // namespace

namespace {
    const StaffRollParam* getStaffRollParam() {
        const char* pRegionPrefix = MR::getCurrentRegionPrefix();

        if (MR::isEqualString(pRegionPrefix, "Jp")) {
            return &sStaffRollParamTable[0];
        }

        if (MR::isEqualString(pRegionPrefix, "Us")) {
            return &sStaffRollParamTable[1];
        }

        if (MR::isEqualString(pRegionPrefix, "Eu")) {
            return &sStaffRollParamTable[2];
        }

        if (MR::isEqualString(pRegionPrefix, "Cn")) {
            return &sStaffRollParamTable[3];
        }

        if (MR::isEqualString(pRegionPrefix, "Kr")) {
            return &sStaffRollParamTable[4];
        }

        return nullptr;
    }
}; // namespace

namespace NrvStaffRollLine {
    NEW_NERVE(StaffRollLineNrvScroll, StaffRollLine, Scroll);
};

StaffRollLine::StaffRollLine(const char* pName)
    : LayoutActor(pName, true),
      mPosition(0.0f, 0.0f) {
}

void StaffRollLine::init(const JMapInfoIter& rIter) {
    initLayoutManager("StaffRoll", 1);
    MR::createAndAddPaneCtrl(this, cPaneNameText, 1);
    MR::setFollowPos(&mPosition, this, cPaneNameText);
    MR::setPaneScale(this, ::getStaffRollParam()->mPaneScale, ::getStaffRollParam()->mPaneScale, cPaneNameText);
    initNerve(&NrvStaffRollLine::StaffRollLineNrvScroll::sInstance);
    kill();
}

void StaffRollLine::appear() {
    LayoutActor::appear();
    setNerve(&NrvStaffRollLine::StaffRollLineNrvScroll::sInstance);
}

void StaffRollLine::exeScroll() {
    if (MR::isFirstStep(this)) {
        mPosition.y = MR::getScreenHeight();
    }

    mPosition.y -= ::getStaffRollParam()->mScrollSpeed;

    if (mPosition.y < -::getStaffRollParam()->mLineLeading) {
        kill();
    }
}

namespace NrvStaffRollPicture {
    NEW_NERVE(StaffRollPictureNrvWork, StaffRollPicture, Work);
};

StaffRollPicture::StaffRollPicture(const char* pName)
    : LayoutActor(pName, true),
      mReplaceTexture(nullptr) {
}

void StaffRollPicture::init(const JMapInfoIter& rIter) {
    initLayoutManager("StaffRollPicture", 1);
    MR::createAndAddPaneCtrl(this, "PictureLU", 1);
    MR::createAndAddPaneCtrl(this, "PictureLD", 1);
    MR::createAndAddPaneCtrl(this, "PictureRU", 1);
    MR::createAndAddPaneCtrl(this, "PictureRD", 1);
    disappearAllPane();
    initReplaceTexture();
    initNerve(&NrvStaffRollPicture::StaffRollPictureNrvWork::sInstance);
    kill();
}

void StaffRollPicture::exeWork() {
    for (s32 i = 0; i < sizeof(sPictureTimingTable) / sizeof(*sPictureTimingTable); i++) {
        const PictureTiming* pPictureTiming = &sPictureTimingTable[i];

        if (MR::isStep(this, pPictureTiming->mAppearStep)) {
            startPictureAction(
                pPictureTiming->mReplaceTextureId,
                cPaneNameTable[pPictureTiming->mPaneId],
                cAnimNameTable[pPictureTiming->mAnimId]);
        }
    }
}

void StaffRollPicture::initReplaceTexture() {
    char        replaceTextureName[64];
    const char* pReplaceTexturePrefix;

    mReplaceTexture = new nw4r::lyt::TexMap*[PICTURE_TEXTURE_NUM];

    if (MR::isStarCompleteAllGalaxy()) {
        pReplaceTexturePrefix = cReplaceTexturePrefixComp;
    } else {
        pReplaceTexturePrefix = cReplaceTexturePrefixHalf;
    }

    for (s32 i = 0; i < PICTURE_TEXTURE_NUM; i++) {
        snprintf(replaceTextureName, sizeof(replaceTextureName), "%s%03d.bti", pReplaceTexturePrefix, i);
        mReplaceTexture[i] = MR::createLytTexMap(cReplaceTextureArcName, replaceTextureName);
    }
}

void StaffRollPicture::startPictureAction(s32 replaceTextureId, const char* pPaneName, const char* pAnimName) {
    if (replaceTextureId >= 0) {
        const char* pChildPaneName = nullptr;

        if (MR::isEqualString(pPaneName, "PictureLU")) {
            pChildPaneName = "PicLU";
        } else if (MR::isEqualString(pPaneName, "PictureLD")) {
            pChildPaneName = "PicLD";
        } else if (MR::isEqualString(pPaneName, "PictureRU")) {
            pChildPaneName = "PicRU";
        } else if (MR::isEqualString(pPaneName, "PictureRD")) {
            pChildPaneName = "PicRD";
        }

        MR::replacePaneTexture(this, pChildPaneName, mReplaceTexture[replaceTextureId], 0);
    }

    MR::startPaneAnim(this, pPaneName, pAnimName, 0);
}

void StaffRollPicture::disappearAllPane() {
    MR::startPaneAnim(this, "PictureLU", "FadeIn", 0);
    MR::startPaneAnim(this, "PictureLD", "FadeIn", 0);
    MR::startPaneAnim(this, "PictureRU", "FadeIn", 0);
    MR::startPaneAnim(this, "PictureRD", "FadeIn", 0);
    MR::setPaneAnimFrameAndStop(this, "PictureLU", 0.0f, 0);
    MR::setPaneAnimFrameAndStop(this, "PictureLD", 0.0f, 0);
    MR::setPaneAnimFrameAndStop(this, "PictureRU", 0.0f, 0);
    MR::setPaneAnimFrameAndStop(this, "PictureRD", 0.0f, 0);
}

namespace NrvStaffRoll {
    NEW_NERVE(StaffRollNrvPrepareDemo, StaffRoll, PrepareDemo);
    NEW_NERVE(StaffRollNrvPrepareBgm, StaffRoll, PrepareBgm);
    NEW_NERVE(StaffRollNrvTitleLogoAppear, StaffRoll, TitleLogoAppear);
    NEW_NERVE(StaffRollNrvTitleLogoScroll, StaffRoll, TitleLogoScroll);
    NEW_NERVE(StaffRollNrvLineScroll, StaffRoll, LineScroll);
    NEW_NERVE(StaffRollNrvEndLogoScroll, StaffRoll, EndLogoScroll);
    NEW_NERVE(StaffRollNrvAllRights, StaffRoll, AllRights);
    NEW_NERVE(StaffRollNrvPrepareInfo, StaffRoll, PrepareInfo);
    NEW_NERVE(StaffRollNrvInfoPictureBookAppear, StaffRoll, InfoAppear);
    NEW_NERVE(StaffRollNrvInfoCometAppear, StaffRoll, InfoAppear);
    NEW_NERVE(StaffRollNrvInfoLuigiAppear, StaffRoll, InfoAppear);
    NEW_NERVE(StaffRollNrvInfoPictureBookDisappear, StaffRoll, InfoDisappear);
    NEW_NERVE(StaffRollNrvInfoCometDisappear, StaffRoll, InfoDisappear);
    NEW_NERVE(StaffRollNrvInfoLuigiDisappear, StaffRoll, InfoDisappear);
    NEW_NERVE(StaffRollNrvPauseForEndingMovie, StaffRoll, Pause);
    NEW_NERVE(StaffRollNrvRestart, StaffRoll, Restart);
    NEW_NERVE(StaffRollNrvPrepareSave, StaffRoll, PrepareSave);
    NEW_NERVE(StaffRollNrvSave, StaffRoll, Save);
    NEW_NERVE(StaffRollNrvEnd, StaffRoll, End);
}; // namespace NrvStaffRoll

StaffRoll::StaffRoll(const char* pName)
    : LayoutActor(pName, true),
      mInfo(nullptr),
      mLine(nullptr),
      mPicture(nullptr),
      mMsg(nullptr),
      mMsgLineNum(0),
      mMsgLine(0),
      mTitleLogoPos(0.0f, 0.0f),
      mEndLogoPos(0.0f, 0.0f),
      _48(-1),
      _4C(0) {
}

void StaffRoll::init(const JMapInfoIter& rIter) {
    MR::connectToSceneLayout(this);
    initLayoutManager("StaffRollTitle", 1);

    mInfo = new InformationMessage();
    mInfo->initWithoutIter();

    MR::createAndAddPaneCtrl(this, cPaneNameTitleLogo, 1);
    MR::copyPaneTrans(&mTitleLogoPos, this, cPaneNameTitleLogo);
    MR::setFollowPos(&mTitleLogoPos, this, cPaneNameTitleLogo);
    MR::hidePane(this, cPaneNameTitleLogo);

    MR::createAndAddPaneCtrl(this, cPaneNameTheEnd, 1);
    MR::copyPaneTrans(&mEndLogoPos, this, cPaneNameTheEnd);
    MR::setFollowPos(&mEndLogoPos, this, cPaneNameTheEnd);
    MR::hidePane(this, cPaneNameTheEnd);

    mLine = new StaffRollLine*[LINE_NUM];

    for (s32 i = 0; i < LINE_NUM; i++) {
        mLine[i] = new StaffRollLine("スタッフロール一行");
        mLine[i]->initWithoutIter();
    }

    mMsg = MR::getGameMessageDirect(cStaffRollMsgId);
    mMsgLineNum = MR::countMessageLine(mMsg);

    mPicture = new StaffRollPicture("スタッフロール写真");
    mPicture->initWithoutIter();

    initNerve(&NrvStaffRoll::StaffRollNrvPrepareDemo::sInstance);
    kill();
}

void StaffRoll::appear() {
    GameSystemFunction::setPermissionToCheckWiiRemoteConnectAndScreenDimming(false);
    MR::deactivateGameSceneDraw3D();
    LayoutActor::appear();
    setNerve(&NrvStaffRoll::StaffRollNrvPrepareDemo::sInstance);
}

void StaffRoll::kill() {
    if (!MR::isDead(mPicture)) {
        mPicture->kill();
    }

    for (s32 i = 0; i < LINE_NUM; i++) {
        if (!MR::isDead(mLine[i])) {
            mLine[i]->kill();
        }
    }

    LayoutActor::kill();
}

void StaffRoll::startInfo() {
    setNerve(&NrvStaffRoll::StaffRollNrvRestart::sInstance);
}

bool StaffRoll::isPauseOrEnd() const {
    return isNerve(&NrvStaffRoll::StaffRollNrvPauseForEndingMovie::sInstance) || isNerve(&NrvStaffRoll::StaffRollNrvEnd::sInstance);
}

bool StaffRoll::isNextLineEmpty() const {
    if (mMsgLine >= mMsgLineNum) {
        return false;
    }

    wchar_t message[256];
    MR::getMessageLine(message, sizeof(message) / sizeof(*message), mMsg, mMsgLine);

    return message[0] == '\0';
}

void StaffRoll::exePrepareDemo() {
    if (MR::tryStartDemo(this, cStaffRollDemoName)) {
        setNerve(&NrvStaffRoll::StaffRollNrvPrepareBgm::sInstance);
    }
}

void StaffRoll::exePrepareBgm() {
    if (MR::isFirstStep(this)) {
        MR::startStageBGM("STM_STAFF_ROLL", true);
    }

    if (!MR::isPreparedStageBgm()) {
        return;
    }

    if (MR::isGreaterStep(this, sStepForTitleLogoAppear)) {
        MR::unlockStageBGM();
        _4C = 0;
        setNerve(&NrvStaffRoll::StaffRollNrvTitleLogoAppear::sInstance);
    }
}

void StaffRoll::exeTitleLogoAppear() {
    if (MR::isFirstStep(this)) {
        MR::showPane(this, cPaneNameTitleLogo);
        MR::startPaneAnim(this, cPaneNameTitleLogo, "TitleLogoAppear", 0);
    }

    if (MR::isStep(this, 240)) {
        setNerve(&NrvStaffRoll::StaffRollNrvTitleLogoScroll::sInstance);
    }
}

void StaffRoll::exeTitleLogoScroll() {
    if (MR::isFirstStep(this)) {
        MR::startPaneAnim(this, cPaneNameTitleLogo, "TitleLogoWait", 0);
        mPicture->appear();
        _48 = 0;
    }

    mTitleLogoPos.y -= ::getStaffRollParam()->mScrollSpeed;

    if (mTitleLogoPos.y < -210.0f) {
        MR::hidePane(this, cPaneNameTitleLogo);
        setNerve(&NrvStaffRoll::StaffRollNrvLineScroll::sInstance);
    }
}

/*
void StaffRoll::exeLineScroll() {
    float v1 = MR::mod(MR::getScreenHeight() - 220.0f, ::getStaffRollParam()->mScrollSpeed);
    float v2 = 14211 - (v1 == 0.0f) + (MR::getScreenHeight() - 220.0f) / ::getStaffRollParam()->mScrollSpeed;

    if (v2 > _4C) {
        return;
    }

    setNerve(&NrvStaffRoll::StaffRollNrvEndLogoScroll::sInstance);
}
*/

void StaffRoll::exeEndLogoScroll() {
    if (MR::isFirstStep(this)) {
        MR::showPane(this, cPaneNameTheEnd);
        MR::startPaneAnim(this, cPaneNameTheEnd, "AllRights", 0);
        MR::setPaneAnimFrameAndStop(this, cPaneNameTheEnd, 0.0f, 0);

        mEndLogoPos.y = MR::getScreenHeight();
    }

    mEndLogoPos.y -= ::getStaffRollParam()->mScrollSpeed;

    if (mEndLogoPos.y < sEndLogoPosY) {
        mEndLogoPos.y = sEndLogoPosY;

        setNerve(&NrvStaffRoll::StaffRollNrvAllRights::sInstance);
    }
}

void StaffRoll::exeAllRights() {
    if (MR::isFirstStep(this)) {
        MR::startPaneAnim(this, cPaneNameTheEnd, "AllRights", 0);
    }

    if (MR::isStep(this, sStepToThankYouVoice)) {
        if (MR::isPlayerLuigi()) {
            MR::startSystemSE("SE_SY_THANK_YOU_LUIGI", -1, -1);
        } else {
            MR::startSystemSE("SE_SY_E3_THANK_YOU", -1, -1);
        }
    }

    if (MR::isPaneAnimStopped(this, cPaneNameTheEnd, 0)) {
        MR::stopStageBGM(0);
        mPicture->kill();
        MR::hidePane(this, cPaneNameTheEnd);
        setNerve(&NrvStaffRoll::StaffRollNrvPrepareInfo::sInstance);
    }
}

void StaffRoll::exePrepareInfo() {
    if (MR::isStep(this, sStepForPrepareInfo)) {
        setNrvNextInfo();
    }
}

void StaffRoll::exeInfoAppear() {
    const char* pMessageId;

    if (MR::isFirstStep(this)) {
        if (isNerve(&NrvStaffRoll::StaffRollNrvInfoPictureBookAppear::sInstance)) {
            pMessageId = "System_Result005";
        } else if (isNerve(&NrvStaffRoll::StaffRollNrvInfoCometAppear::sInstance)) {
            pMessageId = "StaffRollInfo_Comet";
        } else if (isNerve(&NrvStaffRoll::StaffRollNrvInfoLuigiAppear::sInstance)) {
            if (MR::isPlayerLuigi()) {
                pMessageId = "StaffRollInfo_FinalGalaxyOpen";
            } else {
                pMessageId = "StaffRollInfo_Luigi";
            }
        }

        MR::startSubBGM("BGM_NEW_GALAXY", false);

        if (isNerve(&NrvStaffRoll::StaffRollNrvInfoLuigiAppear::sInstance) && !MR::isPlayerLuigi()) {
            MR::startSystemSE("SE_SY_LUIGI_MODE_APPEAR", -1, -1);
        }

        mInfo->setMessage(pMessageId);
        mInfo->setCenter(true);
        mInfo->appearWithButtonLayout();
    }

    if (!MR::testSystemPadTriggerDecide()) {
        return;
    }

    MR::startSystemSE("SE_SY_TALK_OK", -1, -1);

    if (isNerve(&NrvStaffRoll::StaffRollNrvInfoPictureBookAppear::sInstance)) {
        setNerve(&NrvStaffRoll::StaffRollNrvInfoPictureBookDisappear::sInstance);
    } else if (isNerve(&NrvStaffRoll::StaffRollNrvInfoCometAppear::sInstance)) {
        setNerve(&NrvStaffRoll::StaffRollNrvInfoCometDisappear::sInstance);
    } else if (isNerve(&NrvStaffRoll::StaffRollNrvInfoLuigiAppear::sInstance)) {
        setNerve(&NrvStaffRoll::StaffRollNrvInfoLuigiDisappear::sInstance);
    }
}

void StaffRoll::exeInfoDisappear() {
    if (MR::isFirstStep(this)) {
        mInfo->disappear();
    }

    if (MR::isDead(mInfo)) {
        setNrvNextInfo();
    }
}

void StaffRoll::exePause() {
    if (MR::isFirstStep(this)) {
        MR::endDemo(this, cStaffRollDemoName);
        MR::offPlayerControl();
        MR::deactivateDefaultGameLayout();
    }
}

void StaffRoll::exeRestart() {
    if (MR::tryStartDemo(this, cStaffRollDemoName)) {
        setNrvNextInfo();
    }
}

void StaffRoll::exePrepareSave() {
    if (MR::isFirstStep(this)) {
        MR::endDemo(this, cStaffRollDemoName);
        MR::offPlayerControl();
    }

    if (MR::tryStartDemoWithoutCinemaFrameValidHandPointerFinger(this, cSaveAfterStaffRollDemoName)) {
        setNerve(&NrvStaffRoll::StaffRollNrvSave::sInstance);
    }
}

void StaffRoll::exeSave() {
    if (MR::isFirstStep(this)) {
        GameSequenceFunction::startGameDataSaveSequence(true, false);
    }

    if (GameSequenceFunction::isActiveSaveDataHandleSequence()) {
        return;
    }

    MR::endDemo(this, cSaveAfterStaffRollDemoName);
    MR::offPlayerControl();
    MR::deactivateDefaultGameLayout();
    setNerve(&NrvStaffRoll::StaffRollNrvEnd::sInstance);
}

void StaffRoll::exeEnd() {
}

void StaffRoll::movement() {
    if (!MR::isExecuteCalcAnimLayout(this)) {
        return;
    }

    mInfo->movement();
    mPicture->movement();

    for (s32 i = 0; i < LINE_NUM; i++) {
        mLine[i]->movement();
    }

    LayoutActor::movement();
    tryGenerateLine();

    if (_4C == 0) {
        if (MR::isPlayingStageBgm()) {
            _4C++;
        }
    } else {
        _4C++;
    }
}

void StaffRoll::calcAnim() {
    mInfo->calcAnim();
    mPicture->calcAnim();

    for (s32 i = 0; i < LINE_NUM; i++) {
        mLine[i]->calcAnim();
    }

    LayoutActor::calcAnim();
}

void StaffRoll::draw() const {
    if (MR::isDead(this)) {
        return;
    }

    GXColor fillColor = {0, 0, 0, 0};

    MR::fillScreen(fillColor);
    mInfo->draw();
    mPicture->draw();

    for (s32 i = 0; i < LINE_NUM; i++) {
        mLine[i]->draw();
    }

    LayoutActor::draw();
}

void StaffRoll::setNrvNextInfo() {
    if (isNerve(&NrvStaffRoll::StaffRollNrvPrepareInfo::sInstance)) {
        setNerve(&NrvStaffRoll::StaffRollNrvPauseForEndingMovie::sInstance);
    } else if (!MR::isOnGameEventFlagViewNormalEnding() && isNerve(&NrvStaffRoll::StaffRollNrvRestart::sInstance)) {
        setNerve(&NrvStaffRoll::StaffRollNrvInfoPictureBookAppear::sInstance);
    } else if (isNerve(&NrvStaffRoll::StaffRollNrvInfoPictureBookDisappear::sInstance)) {
        MR::onGameEventFlagViewNormalEnding();
        setNerve(&NrvStaffRoll::StaffRollNrvInfoCometAppear::sInstance);
    } else if (!MR::isOnGameEventFlagViewCompleteEnding() && MR::isStarCompleteAllGalaxy()) {
        setNerve(&NrvStaffRoll::StaffRollNrvInfoLuigiAppear::sInstance);
        MR::onGameEventFlagViewCompleteEnding();
    } else {
        setNerve(&NrvStaffRoll::StaffRollNrvPrepareSave::sInstance);
    }
}

bool StaffRoll::tryGenerateLine() {
    f32 lineLeading;

    if (_48 == -1) {
        return false;
    }

    if (_48 == 0) {
        makeLine();

        mMsgLine++;
    }

    _48++;

    if (isNextLineEmpty()) {
        lineLeading = ::getStaffRollParam()->mEmptyLineLeading;
    } else {
        lineLeading = ::getStaffRollParam()->mLineLeading;
    }

    if (_48 * ::getStaffRollParam()->mScrollSpeed >= lineLeading) {
        if (mMsgLine < mMsgLineNum) {
            _48 = 0;
        } else {
            _48 = -1;
        }
    }

    return _48 - 1 == 0;
}

// StaffRoll::makeLine

StaffRollLine* StaffRoll::getDeadLineLayout() const {
    for (s32 i = 0; i < LINE_NUM; i++) {
        if (MR::isDead(mLine[i])) {
            return mLine[i];
        }
    }

    return nullptr;
}

namespace MR {
    StaffRoll* getStaffRoll() {
        return MR::getSceneObj< StaffRoll >(SceneObj_StaffRoll);
    }
}; // namespace MR
