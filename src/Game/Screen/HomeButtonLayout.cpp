#include "Game/Screen/HomeButtonLayout.hpp"
#include "Game/AudioLib/AudSystem.hpp"
#include "Game/AudioLib/AudWrap.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Speaker/SpkSystem.hpp"
#include "Game/System/GameSystemFunction.hpp"
#include "Game/System/HomeButtonMenuWrapper.hpp"
#include "Game/System/Language.hpp"
#include "Game/System/WPadHolder.hpp"
#include "Game/Util/FileUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Util/SystemUtil.hpp"

namespace {
    int SoundCallback(int evt, int num) {
        switch (evt) {
        case HBMSEV_PLAY_SOUND:
            MR::startSystemSE(JAISoundID(num + 0xD0000));
            return HBMSEV_RET_PLAY_SOUND;
        default:
            return HBMSEV_RET_NONE;
        }
    }
};  // namespace

namespace NrvHomeButtonLayout {
    NEW_NERVE(HomeButtonLayoutDeactive, HomeButtonLayout, Deactive);
    NEW_NERVE(HomeButtonLayoutActive, HomeButtonLayout, Active);
};  // namespace NrvHomeButtonLayout

HomeButtonMenuContext::HomeButtonMenuContext() : mHomeButtonInfo(), mControllerData() {
    mHomeButtonInfo = new HBMDataInfo;
    mControllerData = new HBMControllerData;

    initHomeButtonInfo();
    initControllerData();

    mHomeButtonInfo->mem = nullptr;
    mHomeButtonInfo->memSize = 0;
    mHomeButtonInfo->pAllocator = MR::getHomeButtonLayoutAllocator();
    mHomeButtonInfo->messageFlag = HBMMSG_NOSAVE_DEFAULT;
}

void HomeButtonMenuContext::initControllerData() {
    for (s32 i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        mControllerData->wiiCon[i].kpad = nullptr;
        mControllerData->wiiCon[i].pos.x = 0.0f;
        mControllerData->wiiCon[i].pos.y = 0.0f;
        mControllerData->wiiCon[i].use_devtype = WPAD_DEV_FREESTYLE;
    }
}

void HomeButtonMenuContext::initHomeButtonInfo() {
    mHomeButtonInfo->region = MR::getLanguageFromIPL();
    mHomeButtonInfo->layoutBuf = MR::receiveFile("/LayoutData/HomeButton.arc");
    mHomeButtonInfo->spkSeBuf = MR::receiveFile("/HomeButton2/SpeakerSe.arc");
    mHomeButtonInfo->msgBuf = MR::receiveFile("/HomeButton2/home.csv");
    mHomeButtonInfo->configBuf = MR::receiveFile("/HomeButton2/config.txt");
    mHomeButtonInfo->configBufSize = MR::getFileSize("/HomeButton2/config.txt", false);
    mHomeButtonInfo->sound_callback = &::SoundCallback;
    mHomeButtonInfo->backFlag = 0;
    mHomeButtonInfo->cursor = 0;
    mHomeButtonInfo->adjust.x = 832.0f / 608.0f;
    mHomeButtonInfo->adjust.y = 1.0f;
    mHomeButtonInfo->frameDelta = 1.0f;
}

HomeButtonLayout::HomeButtonLayout() : LayoutActor("HOMEボタン", false), mMenuContext(), _24(), _25() {
}

void HomeButtonLayout::init(const JMapInfoIter& rIter) {
    RSO::setupRsoHomeButtonMenu();

    mMenuContext = new HomeButtonMenuContext();

    RSO::HBMCreate(mMenuContext->mHomeButtonInfo);
    MR::setLayoutDefaultAllocator();
    RSO::HBMSetAdjustFlag(MR::isScreen16Per9());
    initNerve(&NrvHomeButtonLayout::HomeButtonLayoutDeactive::sInstance);
    appear();

    mFlag.mIsHidden = true;
    mFlag.mIsOffCalcAnim = true;
}

void HomeButtonLayout::movement() {
    if (MR::isDead(this)) {
        return;
    }

    LayoutActor::movement();
}

void HomeButtonLayout::exeDeactive() {
    if (MR::isFirstStep(this)) {
        mFlag.mIsHidden = true;
        mFlag.mIsOffCalcAnim = true;
        _24 = false;
    }

    if (GameSystemFunction::isResetProcessing()) {
        return;
    }

    if (GameSystemFunction::isDisplayStrapRemineder()) {
        return;
    }

    if (tryCorePadTriggerHome()) {
        setNerve(&NrvHomeButtonLayout::HomeButtonLayoutActive::sInstance);
    }
}

void HomeButtonLayout::exeActive() {
    if (MR::isFirstStep(this)) {
        AudWrap::getSystem()->enterHomeButtonMenu();
        MR::startStarPointerModeHomeButton(this);
        GameSystemFunction::onHomeButtonMenuBeginAllRumble();
        MR::setWPadHolderModeHomeButton();

        if (_24) {
            GameSystemFunction::stopControllerLeaveWatcher();
        }

        mFlag.mIsHidden = false;
        mFlag.mIsOffCalcAnim = false;

        RSO::HBMInit();
        RSO::HBMSetAdjustFlag(MR::isScreen16Per9());
    }

    updateController();
    RSO::HBMCalc(mMenuContext->mControllerData);

    if (RSO::HBMGetSelectBtnNum() == HBM_SELECT_NULL) {
        return;
    }

    bool bVar1 = false;

    switch (RSO::HBMGetSelectBtnNum()) {
    case HBM_SELECT_HOMEBTN:
        GameSystemFunction::onHomeButtonMenuCloseAllRumble();
        break;
    case HBM_SELECT_BTN1:
        AudWrap::getSystem()->preProcessToReset();
        bVar1 = true;
        GameSystemFunction::requestGoWiiMenu(true);
        GameSystemFunction::onHomeButtonMenuEndAllRumble();
        MR::setStarPointerModeBase();
        break;
    case HBM_SELECT_BTN2:
        if (!_25) {
            AudWrap::getSystem()->preProcessToReset();
        }

        bVar1 = true;
        GameSystemFunction::requestResetGameSystem(true);
        GameSystemFunction::onHomeButtonMenuEndAllRumble();
        MR::setStarPointerModeBase();
        break;
    }

    if (!bVar1) {
        AudWrap::getSystem()->exitHomeButtonMenu();
        SpkSystem::reconnect(-1);
    }

    if (MR::isStarPointerModeHomeButton()) {
        MR::endStarPointerMode(this);
    }

    MR::setWPadHolderModeGame();
    GameSystemFunction::setPadConnectCallback();

    if (_24) {
        GameSystemFunction::startControllerLeaveWatcher();
    }

    _25 = false;

    setNerve(&NrvHomeButtonLayout::HomeButtonLayoutDeactive::sInstance);
}

void HomeButtonLayout::draw() const {
    if (MR::isDead(this)) {
        return;
    }

    switch (MR::isHiddenLayout(this)) {
    case false:
        GXClearVtxDesc();
        GXSetVtxAttrFmt(GX_VTXFMT4, GX_VA_POS, GX_POS_XY, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT4, GX_VA_CLR0, GX_CLR_RGB, GX_S8, 0);
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GXSetNumChans(1);
        GXSetNumTexGens(0);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);

        Mtx44 projectionMtx;
        f32 screenHalfHeight = MR::getScreenHeight() / 2.0f;
        f32 screenHalfWidth = MR::getScreenWidth() / 2.0f;
        C_MTXOrtho(projectionMtx, screenHalfHeight, -screenHalfHeight, -screenHalfWidth, screenHalfWidth, 0.0f, 500.0f);
        GXSetProjection(projectionMtx, GX_ORTHOGRAPHIC);

        GXSetCullMode(GX_CULL_NONE);
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
        RSO::HBMDraw();
        break;
    }
}

void HomeButtonLayout::forceToDeactive() {
    if (mMenuContext == nullptr) {
        return;
    }

    RSO::HBMStartBlackOut();
    _25 = true;
}

bool HomeButtonLayout::isActive() const {
    if (MR::isDead(this)) {
        return false;
    }

    return isNerve(&NrvHomeButtonLayout::HomeButtonLayoutActive::sInstance) && !MR::isNewNerve(this);
}

void HomeButtonLayout::updateController() {
    for (s32 chan = WPAD_CHAN0; chan < WPAD_MAX_CONTROLLERS; chan++) {
        MR::getHBMKPadData(&mMenuContext->mControllerData->wiiCon[chan], chan);
    }
}

bool HomeButtonLayout::tryCorePadTriggerHome() {
    for (s32 i = 0; i < MR::getWPadMaxCount(); i++) {
        if (MR::testCorePadTriggerHome(i)) {
            return true;
        }
    }

    return false;
}
