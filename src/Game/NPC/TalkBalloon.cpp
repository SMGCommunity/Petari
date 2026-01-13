#include "Game/NPC/TalkBalloon.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/NPC/TalkMessageCtrl.hpp"
#include "Game/NPC/TalkMessageInfo.hpp"
#include "Game/NPC/TalkState.hpp"
#include "Game/NPC/TalkTextFormer.hpp"
#include "Game/Screen/IconAButton.hpp"
#include "Game/Screen/LayoutActor.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "JSystem/JUtility/JUTVideo.hpp"

namespace NrvTalkBalloonShort {
    NEW_NERVE(TalkBalloonShortNrvOpen, TalkBalloonShort, Open);
    NEW_NERVE(TalkBalloonShortNrvTalk, TalkBalloonShort, Talk);
    NEW_NERVE(TalkBalloonShortNrvClose, TalkBalloonShort, Close);
}  // namespace NrvTalkBalloonShort

namespace NrvTalkBalloonEvent {
    NEW_NERVE(TalkBalloonEventNrvWait, TalkBalloonEvent, Wait);
    NEW_NERVE(TalkBalloonEventNrvOpen, TalkBalloonEvent, Open);
    NEW_NERVE(TalkBalloonEventNrvTalk, TalkBalloonEvent, Talk);
    NEW_NERVE(TalkBalloonEventNrvClose, TalkBalloonEvent, Close);
}  // namespace NrvTalkBalloonEvent

TalkBalloon::TalkBalloon(const char* pName) : LayoutActor(pName, true), mMessageCtrl(nullptr), mTextFormer(nullptr), _28(false), _29(false) {}

void TalkBalloon::create(const char* pLayoutName, bool arg2, bool isTalkLayout) {
    _28 = arg2;

    if (isTalkLayout) {
        MR::connectToSceneTalkLayout(this);
    } else {
        MR::connectToSceneLayout(this);
    }

    if (pLayoutName != nullptr) {
        initLayoutManager(pLayoutName, 2);

        if (_28) {
            MR::createAndAddPaneCtrl(this, "Balloon", 1);
        }

        mTextFormer = new TalkTextFormer(this, "Text00");
        MR::hideScreen(this);
    }
}

void TalkBalloon::init(const JMapInfoIter& rIter) {
    kill();
}

void TalkBalloon::kill() {
    LayoutActor::kill();
    mMessageCtrl = nullptr;
}

void TalkBalloon::open(TalkMessageCtrl* pCtrl) {
    mMessageCtrl = pCtrl;
    MR::startAnim(this, "Appear", 0);
    if (_28) {
        MR::startPaneAnim(this, "Balloon", "Beak", 0);
    }
}

void TalkBalloon::close() {
    MR::startAnim(this, "End", 0);
}

void TalkBalloon::updateTalking() {
    updateBalloon();
    mTextFormer->updateTalking();
}

bool TalkBalloon::isTextAppearedAll() {
    return mTextFormer->isTextAppearedAll();
}

bool TalkBalloon::turnPage() {
    return false;
}

bool TalkBalloon::hasNextPage() {
    return mTextFormer->hasNextPage();
}

void TalkBalloon::skipMessage() {}

void TalkBalloon::pauseOff() {
    MR::requestMovementOn(this);
}

inline f32 fmin(f32 a, f32 b) {
    return b >= a ? a : b;
}

inline f32 fmax(f32 a, f32 b) {
    return b >= a ? b : a;
}

void TalkBalloon::updateBalloon() {
    mMessageCtrl->updateBalloonPos();
    if (_28) {
        TVec2f paneTrans;
        MR::copyPaneTrans(&paneTrans, this, "PicBeak");

        TVec2f v2(mMessageCtrl->_1C.x - paneTrans.x, mMessageCtrl->_1C.y - paneTrans.y);
        TVec2f v3(1.0f, 0.0f);

        MR::normalizeOrZero(&v2);

        f32 cosine = JMAAcosRadian(v2.x * v3.x + v2.y * v3.y) * 57.29578f;

        f32 paneAnimFrameMax = MR::getPaneAnimFrameMax(this, "Balloon", 0);
        f32 paneAnimFrame = MR::getPaneAnimFrame(this, "Balloon", 0);

        if (v2.x * v3.y - v2.y * v3.x > 0.0f) {
            f32 halfMax = paneAnimFrameMax;
            halfMax *= 0.5f;
            paneAnimFrameMax = fmin(paneAnimFrameMax, halfMax + cosine);
        } else {
            f32 half = 0.5f;
            paneAnimFrameMax = fmax(0.0f, paneAnimFrameMax * half - cosine);
        }

        if (!_29) {
            if ((f32)__fabs(paneAnimFrame - paneAnimFrameMax) < 10.0f) {
                return;
            }
            _29 = true;
        }

        if (paneAnimFrame > paneAnimFrameMax) {
            paneAnimFrameMax += 1.0f;
        } else {
            paneAnimFrameMax -= 1.0f;
        }

        if (__fabsf(paneAnimFrame - paneAnimFrameMax) < 2.0f) {
            _29 = false;
        }

        MR::setPaneAnimFrameAndStop(this, "Balloon", paneAnimFrameMax, 0);
    }
}

TalkBalloonShort::TalkBalloonShort(const char* pName) : TalkBalloon(pName), _2C(0) {
    initNerve(&NrvTalkBalloonShort::TalkBalloonShortNrvOpen::sInstance);
}

void TalkBalloonShort::init(const JMapInfoIter& rIter) {
    TalkBalloon::create("TalkBalloonStretch", true, false);
}

void TalkBalloonShort::close() {
    TalkBalloon::close();
    setNerve(&NrvTalkBalloonShort::TalkBalloonShortNrvClose::sInstance);
}

void TalkBalloonShort::updateBalloon() {
    TalkBalloon::updateBalloon();
    if (mMessageCtrl->_1C.z > 0.0f) {
        MR::hideScreen(this);
    } else {
        MR::showScreen(this);
    }
    setTrans(TVec2f(mMessageCtrl->_1C.x, mMessageCtrl->_1C.y));
}

void TalkBalloonShort::initInterval() {
    _2C = 120;
}

void TalkBalloonShort::exeTalk() {
    updateTalking();
    if (_2C != 0) {
        if (MR::isStep(this, _2C)) {
            MR::startAnim(this, "End", 0);
        } else if (MR::isGreaterStep(this, _2C + 30)) {
            open(mMessageCtrl);
        }
    }
}

void TalkBalloonShort::exeClose() {
    updateBalloon();
    if (MR::isAnimStopped(this, 0)) {
        kill();
    }
}

void TalkBalloonShort::exeOpen() {
    updateTalking();
    if (MR::isAnimStopped(this, 0)) {
        setNerve(&NrvTalkBalloonShort::TalkBalloonShortNrvTalk::sInstance);
    }
}

void TalkBalloonShort::open(TalkMessageCtrl* pCtrl) {
    TalkBalloon::open(pCtrl);
    MR::showScreen(this);
    LayoutActor::appear();
    TalkTextFormer* tempFormer = mTextFormer;

    const wchar_t* message;
    if (TalkFunction::isComposeTalk(pCtrl)) {
        message = TalkFunction::getSubMessage(pCtrl);
    } else {
        message = TalkFunction::getMessage(pCtrl);
    }

    tempFormer->formMessage(message, 0);
    tempFormer->setArg(pCtrl->mTagArg, 0);

    s32 numLine = MR::countMessageLine(message);

    switch (numLine) {
    case 1:
        MR::startAnim(this, "OneLine", 1);
        break;

    case 2:
        MR::startAnim(this, "TwoLine", 1);
        break;

    default:
        MR::startAnim(this, "TwoLine", 1);
        break;
    }

    MR::setAnimFrameAndStopAdjustTextWidth(this, "TxtText", 1);
    LayoutActor::setNerve(&NrvTalkBalloonShort::TalkBalloonShortNrvOpen::sInstance);
}

TalkBalloonEvent::TalkBalloonEvent(const char* pName) : TalkBalloon(pName), _2C(1), mAButton(nullptr) {
    initNerve(&NrvTalkBalloonEvent::TalkBalloonEventNrvWait::sInstance);
}

void TalkBalloonEvent::init(const JMapInfoIter& rIter) {
    create("TalkBalloonEvent", 1, 1);
    MR::createAndAddPaneCtrl(this, "AButtonPosition", 1);
}

void TalkBalloonEvent::open(TalkMessageCtrl* pCtrl) {
    mMessageCtrl = pCtrl;
    LayoutActor::appear();
    TalkMessageInfo* info = TalkFunction::getMessageInfo(pCtrl);
    mTextFormer->formMessage((const wchar_t*)info->_0, _2C);
    mTextFormer->setArg(pCtrl->mTagArg, 0);
    mAButton->kill();
    mAButton->setFollowActorPane(this, "AButtonPosition");
    updateBalloon();
    LayoutActor::setNerve(&NrvTalkBalloonEvent::TalkBalloonEventNrvWait::sInstance);
}

void TalkBalloonEvent::close() {
    MR::startAnim(this, "End", 0);

    if (!TalkFunction::isSelectTalk(mMessageCtrl)) {
        MR::startSystemSE("SE_SY_TALK_OK", -1, -1);
    }

    if (!MR::isDead(mAButton)) {
        mAButton->term();
    }

    MR::recoverSoundVolumeSetting(30);
    MR::moveVolumeStageBGM(1.0f, 60);
    MR::moveVolumeSubBGM(1.0f, 60);
    LayoutActor::setNerve(&NrvTalkBalloonEvent::TalkBalloonEventNrvClose::sInstance);
}

bool TalkBalloonEvent::turnPage() {
    if (mTextFormer->nextPage()) {
        mTextFormer->setArg(mMessageCtrl->mTagArg, 0);
        MR::startSystemSE("SE_SY_TALK_FOCUS_ITEM", -1, -1);
        mAButton->term();

        return true;
    }

    return false;
}

void TalkBalloonEvent::skipMessage() {
    if (LayoutActor::isNerve(&NrvTalkBalloonEvent::TalkBalloonEventNrvTalk::sInstance)) {
        mTextFormer->updateTalking();
    }
}

bool TalkBalloonEvent::isTextAppearedAll() {
    return mTextFormer->isTextAppearedAll() && LayoutActor::isNerve(&NrvTalkBalloonEvent::TalkBalloonEventNrvTalk::sInstance);
}

void TalkBalloonEvent::exeWait() {
    if (MR::isTimeKeepDemoActive()) {
        LayoutActor::setNerve(&NrvTalkBalloonEvent::TalkBalloonEventNrvOpen::sInstance);
    } else if (!MR::isFirstStep(this) && MR::isCameraInterpolatingNearlyEnd()) {
        LayoutActor::setNerve(&NrvTalkBalloonEvent::TalkBalloonEventNrvOpen::sInstance);
    }
}

void TalkBalloonEvent::exeOpen() {
    if (MR::isFirstStep(this)) {
        updateBeak();
        MR::showScreen(this);
        TalkBalloon::open(mMessageCtrl);
        MR::startSystemSE("SE_SM_TALKBLN_OPEN", -1, -1);
        MR::setSoundVolumeSetting(3, 30);

        if (!MR::isPlayingStageBgmName("STM_KINOPIO_TANKEN_B")) {
            MR::moveVolumeStageBGM(0.5f, 30);
            MR::moveVolumeSubBGM(0.5f, 30);
        }
    }

    updateBalloon();

    if (MR::isAnimStopped(this, 0)) {
        MR::startAnim(this, "Wait", 0);
        LayoutActor::setNerve(&NrvTalkBalloonEvent::TalkBalloonEventNrvTalk::sInstance);
    }
}

void TalkBalloonEvent::exeTalk() {
    updateTalking();
    if (mTextFormer->isTextAppearedAll() && MR::isDead(mAButton) && !TalkFunction::getMessageInfo(mMessageCtrl)->isBalloonFix()) {
        if (!TalkFunction::isSelectTalk(mMessageCtrl) || hasNextPage()) {
            mAButton->openWithoutMessage();
        }
    }
}

void TalkBalloonEvent::exeClose() {
    if (MR::isAnimStopped(this, 0)) {
        kill();
    }
}

void TalkBalloonEvent::updateBeak() {
    if (!_28) {
        return;
    }

    mMessageCtrl->updateBalloonPos();
    TalkMessageCtrl* messageCtrl = mMessageCtrl;
    TVec2f v1(messageCtrl->_1C.x, messageCtrl->_1C.y);

    if (0.0f <= v1.x && v1.x < MR::getScreenWidth() && 120.0f <= v1.y && v1.y < (s32)JUTVideo::sManager->mRenderModeObj->efbHeight) {
        MR::showPane(this, "ShaBeak");
        MR::showPane(this, "PicBeak");
    } else {
        MR::hidePane(this, "ShaBeak");
        MR::hidePane(this, "PicBeak");
    }
}

TalkBalloonSign::TalkBalloonSign(const char* pName) : TalkBalloonEvent(pName) {
    _2C = 2;
}

void TalkBalloonSign::init(const JMapInfoIter& rIter) {
    TalkBalloon::create("SignBoardBalloon", false, true);
    MR::createAndAddPaneCtrl(this, "AButtonPosition", 1);
}

TalkBalloonInfo::TalkBalloonInfo(const char* pName) : TalkBalloon(pName) {}

void TalkBalloonInfo::open(TalkMessageCtrl* pCtrl) {
    TalkMessageInfo* info = TalkFunction::getMessageInfo(pCtrl);
    MR::appearInformationMessage((const wchar_t*)info->_0, true);
}

void TalkBalloonInfo::close() {
    MR::startSystemSE("SE_SY_TALK_OK", -1, -1);
    MR::disappearInformationMessage();
}

void TalkBalloonInfo::updateTalking() {}

void TalkBalloonInfo::updateBalloon() {}

bool TalkBalloonInfo::isTextAppearedAll() {
    return true;
}

TalkBalloonIcon::TalkBalloonIcon(const char* pName) : TalkBalloonShort(pName) {}

void TalkBalloonIcon::init(const JMapInfoIter& rIter) {
    TalkBalloon::create("TalkBalloonLetter", false, false);
}

void TalkBalloonIcon::open(TalkMessageCtrl* pCtrl) {
    TalkBalloon::open(pCtrl);
    MR::showScreen(this);
    LayoutActor::appear();

    const wchar_t* message = TalkFunction::getMessage(pCtrl);
    TalkTextFormer* former = mTextFormer;

    former->formMessage(message, 2);
    former->setArg(pCtrl->mTagArg, 0);
    LayoutActor::setNerve(&NrvTalkBalloonShort::TalkBalloonShortNrvOpen::sInstance);
}

TalkBalloonHolder::TalkBalloonHolder() : _14(0) {
    mBalloonShortArray = new TalkBalloonShort*[4];

    for (u32 i = 0; i < 4; i++) {
        // "Speech bubble [plain discussion]"
        TalkBalloonShort* temp = new TalkBalloonShort("会話吹き出し[簡易会話]");
        mBalloonShortArray[i] = temp;
        mBalloonShortArray[i]->initWithoutIter();
        mBalloonShortArray[i]->kill();
    }

    // "Speech bubble [event]"
    mBalloonEvent = new TalkBalloonEvent("会話吹き出し[イベント]");
    mBalloonEvent->initWithoutIter();
    mBalloonEvent->kill();

    // "Speech bubble [information]"
    mBalloonInfo = new TalkBalloonInfo("会話吹き出し[インフォメーション]");
    mBalloonInfo->initWithoutIter();
    mBalloonInfo->kill();

    // "Speech bubble [signboard]"
    mBalloonSign = new TalkBalloonSign("会話吹き出し[看板]");
    mBalloonSign->initWithoutIter();
    mBalloonSign->kill();

    // "Speech bubble [icon]"
    mBalloonIcon = new TalkBalloonIcon("会話吹き出し[アイコン]");
    mBalloonIcon->initWithoutIter();
    mBalloonIcon->kill();

    mAButton = new IconAButton(true, false);
    mAButton->initWithoutIter();

    mBalloonEvent->mAButton = mAButton;
    mBalloonSign->mAButton = mAButton;
}

void TalkBalloonHolder::balloonOff() {
    mAButton->kill();
}

void TalkBalloonHolder::update() {}

bool TalkBalloonHolder::isActiveBalloonShort() const {
    for (int i = 0; i < 4; i++) {
        if (!MR::isDead(mBalloonShortArray[i])) {
            return true;
        }
    }

    return false;
}

void TalkBalloonHolder::pauseOff() {
    mBalloonEvent->pauseOff();
    mBalloonInfo->pauseOff();
    mBalloonSign->pauseOff();
    mBalloonIcon->pauseOff();

    for (int i = 0; i < 4; i++) {
        mBalloonShortArray[i]->pauseOff();
    }

    MR::requestMovementOn(mAButton);
}

TalkBalloon* TalkBalloonHolder::getBalloon(const TalkMessageCtrl* pArg) {
    TalkBalloon* balloon;
    const TalkMessageInfo* info = TalkFunction::getMessageInfo(pArg);

    if (info->isNormalTalk()) {
        balloon = mBalloonEvent;
    } else if (info->isShortTalk()) {
        mBalloonShortArray[_14]->_2C = 0;
        balloon = mBalloonShortArray[_14];
    } else if (info->isEventTalk()) {
        balloon = mBalloonEvent;
    } else if (info->isComposeTalk()) {
        mBalloonShortArray[_14]->_2C = 0;
        balloon = mBalloonEvent;
    } else {
        balloon = nullptr;
    }

    if (info->isBalloonInfo()) {
        balloon = mBalloonInfo;
    } else if (info->isBalloonSign()) {
        balloon = mBalloonSign;
    } else if (info->isBalloonFix()) {
        balloon = mBalloonEvent;
    } else if (info->isBalloonCall()) {
        mBalloonShortArray[_14]->_2C = 120;
        balloon = mBalloonShortArray[_14];
    } else if (info->isBalloonIcon()) {
        balloon = mBalloonIcon;
    }

    if (balloon == mBalloonShortArray[_14]) {
        _14 = (_14 + 1) % 4;
    }

    return balloon;
}
