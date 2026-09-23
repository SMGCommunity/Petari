#include "Game/Map/FileSelector.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/FileSelectCameraController.hpp"
#include "Game/Map/FileSelectEffect.hpp"
#include "Game/Map/FileSelectFunc.hpp"
#include "Game/Map/FileSelectItem.hpp"
#include "Game/Map/FileSelectItemDelegator.hpp"
#include "Game/Map/FileSelectSky.hpp"
#include "Game/NPC/MiiFacePartsHolder.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Screen/BackButton.hpp"
#include "Game/Screen/BrosButton.hpp"
#include "Game/Screen/FileSelectButton.hpp"
#include "Game/Screen/FileSelectInfo.hpp"
#include "Game/Screen/InformationMessage.hpp"
#include "Game/Screen/Manual2P.hpp"
#include "Game/Screen/MiiConfirmIcon.hpp"
#include "Game/Screen/MiiSelect.hpp"
#include "Game/Screen/SysInfoWindow.hpp"
#include "Game/Screen/TitleSequenceProduct.hpp"
#include "Game/System/GameSequenceFunction.hpp"
#include "Game/System/UserFile.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/FileUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SequenceUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <JSystem/JKernel/JKRMemArchive.hpp>
#include <nw4r/ut/ResFont.h>

#define USER_FILE_NUM 6

namespace {
    static const f32 sSlopeDegree = 20.0f;
    static const f32 sItemPosRadius = 5000.0f;
    static const f32 sSelectEffectOffset = 1000.0f;
    const char* cMarioNameMessageID = "System_FileSelect_Icon000";
    const char* cLuigiNameMessageID = "System_FileSelect_Icon001";
    static s32 sBgmNearState = 6;
    static u32 sBgmNearStateChangeFrames = 60;
    static s32 sBgmFarState = 5;
    static u32 sBgmFarStateChangeFrames = 60;
    // static _ sThetaOffset = _;
    // static _ sCreatorOffset = _;
    static const f32 sItemThetaOffset[USER_FILE_NUM] = {10.0f, -10.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    static const s32 sIndexOrder[USER_FILE_NUM] = {1, 2, 4, 6, 5, 3};
};  // namespace

namespace {
    inline int getItemArrayIndex(s32 id) {
        const s32* pIndexOrder = sIndexOrder;

        for (int i = 0; i < ARRAY_SIZE(::sIndexOrder); i++) {
            if (id == pIndexOrder[i]) {
                return i;
            }
        }

        return -1;
    }

    MiiFacePartsHolder* getMiiFacePartsHolder() {
        return MR::getSceneObj< MiiFacePartsHolder >(SceneObj_MiiFacePartsHolder);
    }
};  // namespace

namespace NrvFileSelector {
    NEW_NERVE(FileSelectorNrvWaitBind, FileSelector, WaitBind);
    NEW_NERVE(FileSelectorNrvTitle, FileSelector, Title);
    NEW_NERVE(FileSelectorNrvTitleEnd, FileSelector, TitleEnd);
    NEW_NERVE(FileSelectorNrvRFLError, FileSelector, RFLError);
    NEW_NERVE(FileSelectorNrvRFLWait, FileSelector, RFLWait);
    NEW_NERVE(FileSelectorNrvRFLWaitEnd, FileSelector, RFLWaitEnd);
    NEW_NERVE(FileSelectorNrvFileSelectStart, FileSelector, FileSelectStart);
    NEW_NERVE(FileSelectorNrvFileSelect, FileSelector, FileSelect);
    NEW_NERVE(FileSelectorNrvFileConfirmStart, FileSelector, FileConfirmStart);
    NEW_NERVE(FileSelectorNrvFileConfirmMiiDeleteWarningStart, FileSelector, FileConfirmMiiDeleteWarningStart);
    NEW_NERVE(FileSelectorNrvFileConfirmMiiDeleteWarning, FileSelector, FileConfirmMiiDeleteWarning);
    NEW_NERVE(FileSelectorNrvFileConfirmMiiDeleteSave, FileSelector, FileConfirmMiiDeleteSave);
    NEW_NERVE(FileSelectorNrvFileConfirm, FileSelector, FileConfirm);
    NEW_NERVE(FileSelectorNrvDemoStartWait, FileSelector, DemoStartWait);
    NEW_NERVE(FileSelectorNrvDemo, FileSelector, Demo);
    NEW_NERVE(FileSelectorNrvCreateConfirmStart, FileSelector, CreateConfirmStart);
    NEW_NERVE(FileSelectorNrvCreateConfirm, FileSelector, CreateConfirm);
    NEW_NERVE(FileSelectorNrvCreate, FileSelector, Create);
    NEW_NERVE(FileSelectorNrvCopyWait, FileSelector, CopyWait);
    NEW_NERVE(FileSelectorNrvCopySelect, FileSelector, CopySelect);
    NEW_NERVE(FileSelectorNrvCopyConfirmStart, FileSelector, CopyConfirmStart);
    NEW_NERVE(FileSelectorNrvCopyConfirm, FileSelector, CopyConfirm);
    NEW_NERVE(FileSelectorNrvCopySave, FileSelector, CopySave);
    NEW_NERVE(FileSelectorNrvCopySaveMii, FileSelector, CopySaveMii);
    NEW_NERVE(FileSelectorNrvCopyDemo, FileSelector, CopyDemo);
    NEW_NERVE(FileSelectorNrvCopyRejectStart, FileSelector, CopyRejectStart);
    NEW_NERVE(FileSelectorNrvCopyReject, FileSelector, CopyReject);
    NEW_NERVE(FileSelectorNrvMiiWait, FileSelector, MiiWait);
    NEW_NERVE(FileSelectorNrvMiiTip, FileSelector, MiiTip);
    NEW_NERVE(FileSelectorNrvMiiSelectStartFirst, FileSelector, MiiSelectStart);
    NEW_NERVE(FileSelectorNrvMiiSelectStart, FileSelector, MiiSelectStart);
    NEW_NERVE(FileSelectorNrvMiiSelect, FileSelector, MiiSelect);
    NEW_NERVE(FileSelectorNrvMiiCancel, FileSelector, MiiCancel);
    NEW_NERVE(FileSelectorNrvMiiConfirmWait, FileSelector, MiiConfirmWait);
    NEW_NERVE(FileSelectorNrvMiiConfirm, FileSelector, MiiConfirm);
    NEW_NERVE(FileSelectorNrvMiiCreateWait, FileSelector, MiiCreateWait);
    NEW_NERVE(FileSelectorNrvMiiCreateDemo, FileSelector, MiiCreateDemo);
    NEW_NERVE(FileSelectorNrvMiiCaution, FileSelector, MiiCaution);
    NEW_NERVE(FileSelectorNrvMiiInfoStart, FileSelector, MiiInfoStart);
    NEW_NERVE(FileSelectorNrvMiiInfo, FileSelector, MiiInfo);
    NEW_NERVE(FileSelectorNrvDeleteConfirmStart, FileSelector, DeleteConfirmStart);
    NEW_NERVE(FileSelectorNrvDeleteConfirm, FileSelector, DeleteConfirm);
    NEW_NERVE(FileSelectorNrvDelete, FileSelector, Delete);
    NEW_NERVE(FileSelectorNrvDeleteDemo, FileSelector, DeleteDemo);
    NEW_NERVE(FileSelectorNrvFileBroken, FileSelector, FileBroken);
    NEW_NERVE(FileSelectorNrvManualStart, FileSelector, ManualStart);
    NEW_NERVE(FileSelectorNrvManual, FileSelector, Manual);
};  // namespace NrvFileSelector

FileSelector::FileSelector(const char* pName)
    : LiveActor(pName), mCameraController(), mSky(), mItems(), mOperationButton(), mBackButton(), mBrosButton(), mInfoMessage(), mSysInfoWindow(),
      _B4(), _B8(), _BC(), _C0(), mUserFile(), _CC(), mTitle(), mMiiSelect(), mMiiId(new RFLCreateID()), mManual(), mIsMiiSelectStartFirst(),
      mBgmState(), mSelectEffect() {
}

void FileSelector::init(const JMapInfoIter& rIter) {
    MR::connectToScene(this, MR::MovementType_Environment, MR::CalcAnimType_None, MR::DrawBufferType_None, MR::DrawType_None);
    initHitSensor(1);
    MR::addHitSensorPriorBinder(this, "body", 8, 500.0f, TVec3f(0.0f, 0.0f, 0.0f));
    initUserFileArray();
    MR::invalidateClipping(this);
    createCameraController();
    createSky();
    createFileItems();
    createOperationButton();
    createBackButton();
    createBrosButton();
    createInfoMessage();
    createSysInfoWindow();
    createFileInfo();
    createTitle();
    createMiiSelect();
    createMiiConfirmIcon();
    createManual();
    createSelectEffect();
    initNerve(GET_NERVE(FileSelector, FileSelectorNrvWaitBind));
    MR::tryRegisterDemoCast(this, rIter);
    createMiiFont();
    appear();
}

void FileSelector::appear() {
    LiveActor::appear();
    mCameraController->appear();
    setNerve(GET_NERVE(FileSelector, FileSelectorNrvWaitBind));
}

void FileSelector::kill() {
    mBackButton->kill();
    LiveActor::kill();
}

void FileSelector::callbackStart() {
    setNerve(GET_NERVE(FileSelector, FileSelectorNrvDemoStartWait));
}

void FileSelector::callbackCopy() {
    setNerve(GET_NERVE(FileSelector, FileSelectorNrvCopyWait));
}

void FileSelector::callbackMii() {
    disappearAllLayout();
    invalidateSelectAll();
    setNerve(GET_NERVE(FileSelector, FileSelectorNrvMiiWait));
}

void FileSelector::callbackDelete() {
    setNerve(GET_NERVE(FileSelector, FileSelectorNrvDeleteConfirmStart));
}

void FileSelector::callbackManual() {
    MR::startSystemSE("SE_SY_FILE_SEL_TIPS_OPEN");
    setNerve(GET_NERVE(FileSelector, FileSelectorNrvManualStart));
}

void FileSelector::notifyItem(FileSelectItem* pItem, s32 msg) {
    switch (msg) {
    case 0:
        onPoint(pItem);
        break;
    case 1:
        onSelect(pItem);
        break;
    case 2:
        if (isNerve(GET_NERVE(FileSelector, FileSelectorNrvCopySelect))) {
            onPoint(pItem);
        }
        break;
    default:
        break;
    }
}

void FileSelector::control() {
    _C0 = nullptr;

    for (int i = 0; i < mItems->getObjNum(); i++) {
        LiveActor* pActor = mItems->getActor(i);
        TVec3f& rPosition = pActor->mPosition;
        rPosition = _98[i] * 0.05f + rPosition * 0.95f;
    }

    for (int i = 0; i < mItems->getObjNum(); i++) {
        LiveActor* pActor = mItems->getActor(i);
        TVec3f& rEffectPosition = mSelectEffect[i].mPosition;
        rEffectPosition.set(pActor->mPosition + TVec3f(0.0f, ::sSelectEffectOffset, 0.0f));
    }

    updateBgm();
}

bool FileSelector::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgAutoRushBegin(msg) && isNerve(GET_NERVE(FileSelector, FileSelectorNrvWaitBind))) {
        MR::hidePlayer();
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvTitle));

        return true;
    }

    if (MR::isMsgUpdateBaseMtx(msg)) {
        TMtx34f baseMtx;
        baseMtx.identity();

        MR::setPlayerBaseMtx(baseMtx.toMtxPtr());

        return true;
    }

    return false;
}

void FileSelector::initUserFileArray() {
    mUserFile = new UserFile[USER_FILE_NUM]();
    _CC = new bool[USER_FILE_NUM];

    initUserFile();
}

void FileSelector::createCameraController() {
    mCameraController = new FileSelectCameraController("ファイルセレクトカメラ制御");
    mCameraController->initWithoutIter();
}

void FileSelector::createSky() {
    mSky = new FileSelectSky("ファイルセレクト画面の空");
    mSky->initWithoutIter();
    mSky->appear();
}

void FileSelector::createFileItems() {
    mItems = new DeriveActorGroup< FileSelectItem >("全ファイルアイテム保持", USER_FILE_NUM);
    _98 = new TVec3f[USER_FILE_NUM]();

    calcBasePos(0.0f);
    MR::createSceneObj(SceneObj_MiiFacePartsHolder);

    FileSelectItemDelegator< FileSelector >* pDelegator = new FileSelectItemDelegator< FileSelector >(this, &FileSelector::notifyItem);

    s32 id;

    for (int i = 0; i < USER_FILE_NUM; i++) {
        id = ::sIndexOrder[i];
        FileSelectIconID iconId = FileSelectIconID();
        FileSelectItem* pItem = new FileSelectItem(id, true, iconId, "ファイルセレクトアイテム");

        pItem->initWithoutIter();
        pItem->setSelectDelegator(pDelegator);
        pItem->mPosition.set(_98[i]);
        mItems->registerActor(pItem);
    }
}

void FileSelector::createOperationButton() {
    mOperationButton = new FileSelectButton("FileSelectButton");
    mOperationButton->initWithoutIter();
    mOperationButton->setCallbackFunctor(MR::Functor(this, &FileSelector::callbackStart), MR::Functor(this, &FileSelector::callbackCopy),
                                         MR::Functor(this, &FileSelector::callbackMii), MR::Functor(this, &FileSelector::callbackDelete),
                                         MR::Functor(this, &FileSelector::callbackManual));
}

void FileSelector::createBackButton() {
    mBackButton = new BackButton("戻るボタン", false);
    mBackButton->initWithoutIter();

    MR::connectToScene(mBackButton, MR::MovementType_Layout, MR::CalcAnimType_Layout, MR::DrawBufferType_None, MR::DrawType_LayoutDecoration);
}

void FileSelector::createBrosButton() {
    mBrosButton = new BrosButton("ルイージ切り替えボタン");
    mBrosButton->initWithoutIter();
}

void FileSelector::createInfoMessage() {
    mInfoMessage = new InformationMessage();
    mInfoMessage->initWithoutIter();
}

void FileSelector::createSysInfoWindow() {
    mSysInfoWindow = MR::createSysInfoWindow();
    MR::connectToSceneLayout(mSysInfoWindow);

    mSysInfoWindowMini = MR::createSysInfoWindowMiniExecuteWithChildren();
    MR::connectToSceneLayout(mSysInfoWindowMini);
}

void FileSelector::createFileInfo() {
    mFileInfo = new FileSelectInfo(RFL_NAME_LEN + 1, "ファイル情報");
    mFileInfo->initWithoutIter();
}

void FileSelector::createTitle() {
    mTitle = new TitleSequenceProduct();
    mTitle->kill();
}

void FileSelector::createMiiSelect() {
    mMiiSelect = new MiiSelect("MiiSelect");
    mMiiSelect->initWithoutIter();
}

void FileSelector::createMiiConfirmIcon() {
    mMiiConfirmIcon = new MiiConfirmIcon("Mii確認用アイコン");
    mMiiConfirmIcon->initWithoutIter();
    MR::connectToScene(mMiiConfirmIcon, MR::MovementType_Layout, MR::CalcAnimType_Layout, MR::DrawBufferType_None, MR::DrawType_LayoutDecoration);
}

void FileSelector::createMiiFont() {
    JKRMemArchive* pArchive = MR::receiveArchive("/LayoutData/MiiFont.arc");
    mFont = new nw4r::ut::ResFont();
    mFont->SetResource(pArchive->getResource("/MiiFont26.brfnt"));
    mFont->SetAlternateChar('?');
    MR::setTextBoxFontRecursive(mFileInfo, "FileName", mFont);
    MR::setTextBoxFontRecursive(mMiiSelect, "TxtName", mFont);
    MR::setTextBoxFontRecursive(mMiiConfirmIcon, "MiiName", mFont);
}

void FileSelector::createManual() {
    mManual = new Manual2P("２Ｐマニュアル");
    mManual->initWithoutIter();
}

void FileSelector::createSelectEffect() {
    mSelectEffect = new FileSelectEffect[USER_FILE_NUM]();

    for (int i = 0; i < USER_FILE_NUM; i++) {
        mSelectEffect[i].initWithoutIter();
        mSelectEffect[i].mPosition.set(_98[i]);
        mSelectEffect[i].mScale.set(0.4f);
    }
}

void FileSelector::appearAllItems() {
    for (int i = 0; i < mItems->getObjNum(); i++) {
        mItems->getActor(i)->appear();
    }
}

void FileSelector::disappearAllLayout() {
    if (!MR::isDead(mOperationButton)) {
        mOperationButton->disappear();

        if (!MR::isDead(mFileInfo)) {
            mFileInfo->slideBack();
        }
    }

    if (!MR::isDead(mFileInfo)) {
        mFileInfo->disappear();
    }

    if (!mBackButton->isHidden() && mBackButton->_24 == 0) {
        mBackButton->disappear();
    }

    if (!MR::isDead(mBrosButton)) {
        mBrosButton->disappear();
    }
}

bool FileSelector::isHiddenAllLayout() const {
    return MR::isDead(mOperationButton) && MR::isDead(mFileInfo) && mBackButton->isHidden() && MR::isDead(mBrosButton);
}

void FileSelector::updateFileInfo() {
    if (_C0 != nullptr) {
        if (_BC != _C0) {
            if (_BC != nullptr) {
                clearPointing();
                mFileInfo->disappear();
            }

            if (_C0->isExist()) {
                setFileInfo(_C0->_140);
                mFileInfo->appear();
                mFileInfo->forceChange();
            }

            _BC = _C0;
            _BC->onPointing();
        }
    } else if (_BC != nullptr) {
        clearPointing();
        mFileInfo->disappear();
    }
}

void FileSelector::appearAllIndex() {
    for (int i = 0; i < mItems->getObjNum(); i++) {
        static_cast< FileSelectItem* >(mItems->getActor(i))->appearIndex();
    }
}

void FileSelector::disappearAllIndex() {
    for (int i = 0; i < mItems->getObjNum(); i++) {
        static_cast< FileSelectItem* >(mItems->getActor(i))->disappearIndex();
    }
}

void FileSelector::invalidateSelectAll() {
    for (int i = 0; i < mItems->getObjNum(); i++) {
        static_cast< FileSelectItem* >(mItems->getActor(i))->invalidateSelect();
    }
}

void FileSelector::validateSelectAll() {
    for (int i = 0; i < mItems->getObjNum(); i++) {
        static_cast< FileSelectItem* >(mItems->getActor(i))->validateSelect();
    }
}

void FileSelector::initUserFile() {
    for (int i = 0; i < USER_FILE_NUM; i++) {
        GameSequenceFunction::restoreUserFile(&mUserFile[i], i + 1);
    }

    checkAllComplete();
}

void FileSelector::restoreUserFile() {
    for (int i = 0; i < USER_FILE_NUM; i++) {
        GameSequenceFunction::restoreUserFile(&mUserFile[i], i + 1, mUserFile[i].mIsPlayerMario);
    }

    checkAllComplete();
}

void FileSelector::checkAllComplete() {
    bool isPlayerMario;

    for (int i = 0; i < USER_FILE_NUM; i++) {
        isPlayerMario = mUserFile[i].mIsPlayerMario;
        _CC[i] = false;

        if (!isPlayerMario) {
            GameSequenceFunction::restoreUserFile(&mUserFile[i], i + 1, true);
        }

        if (mUserFile[i].isPowerStarGetFinalChallengeGalaxy()) {
            GameSequenceFunction::restoreUserFile(&mUserFile[i], i + 1, false);

            if (mUserFile[i].isPowerStarGetFinalChallengeGalaxy()) {
                _CC[i] = true;
            }
        }

        if (isPlayerMario != mUserFile[i].mIsPlayerMario) {
            GameSequenceFunction::restoreUserFile(&mUserFile[i], i + 1, isPlayerMario);
        }
    }
}

void FileSelector::onPoint(FileSelectItem* pItem) {
    if (!mBackButton->isHidden() && mBackButton->isPointing()) {
        return;
    }

    if (_C0 != nullptr) {
        if (_C0->_140 > pItem->_140) {
            _C0 = pItem;
        }
    } else {
        _C0 = pItem;
    }
}

void FileSelector::onSelect(FileSelectItem* pItem) {
    if (isNerve(GET_NERVE(FileSelector, FileSelectorNrvCopySelect))) {
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvCopyConfirmStart));
    } else if (isNerve(GET_NERVE(FileSelector, FileSelectorNrvFileSelect))) {
        if (pItem->isNew()) {
            setNerve(GET_NERVE(FileSelector, FileSelectorNrvCreateConfirmStart));
        } else {
            setNerve(GET_NERVE(FileSelector, FileSelectorNrvFileConfirmStart));
        }
    } else {
        return;
    }

    _B4 = pItem;

    MR::startSystemSE("SE_SY_GALAXY_SELECTED");
    playSelectedME();
}

void FileSelector::clearPointing() {
    if (_BC == nullptr) {
        return;
    }

    _BC->offPointing();
    _BC = nullptr;
}

void FileSelector::setFileInfo(s32 id) {
    OSCalendarTime td;
    u16 nameBuffer[RFL_NAME_LEN + 1];
    wchar_t dateBuffer[32];
    wchar_t timeBuffer[32];

    if (static_cast< FileSelectItem* >(mItems->getActor(::getItemArrayIndex(id)))->_146) {
        FileSelectFunc::copyMiiName(nameBuffer, FileSelectIconID());
    } else {
        FileSelectIconID iconId = FileSelectIconID();

        getIconId(&iconId, id);
        FileSelectFunc::copyMiiName(nameBuffer, iconId);
    }

    OSTicksToCalendarTime(mUserFile[id - 1].getLastModified(), &td);

    MR::makeDateString(dateBuffer, ARRAY_SIZE(dateBuffer), td.year, td.mon + 1, td.mday);
    MR::makeTimeString(timeBuffer, ARRAY_SIZE(timeBuffer), td.hour, td.min);

    bool isViewCompleteEnding = mUserFile[id - 1].isViewCompleteEnding();
    bool isViewNormalEnding = mUserFile[id - 1].isViewNormalEnding();
    s32 starPieceNum = mUserFile[id - 1].getStarPieceNum();
    s32 powerStarNum = mUserFile[id - 1].getPowerStarNum();
    s32 missCount = getMissCount(id);
    bool isUserFileMario = !isUserFileLuigi(id);

    mFileInfo->setInfo(nameBuffer, id, powerStarNum, starPieceNum, isUserFileMario, isViewNormalEnding, isViewCompleteEnding, dateBuffer, timeBuffer,
                       missCount);
}

bool FileSelector::checkSelectedBackButton() {
    if (mBackButton->isHidden()) {
        return false;
    }

    if (mBackButton->_24) {
        return true;
    }

    if (MR::testSystemTriggerB()) {
        MR::startSystemSE("SE_SY_GALAXY_DECIDE_CANCEL");
        mBackButton->disappear();

        return true;
    }

    return false;
}

void FileSelector::goToNearPoint() {
    calcBasePos(-16000.0f);
    mCameraController->goToNearPoint(_98[::getItemArrayIndex(_B4->_140)]);
}

void FileSelector::calcBasePos(f32 offset) {
    f32 thetaStep = 2.0f * PI / USER_FILE_NUM;
    TPos3f translation;
    translation.makeTrans(0.0f, offset, 0.0f);

    TPos3f rotation;
    rotation.makeRotate(TVec3f(1.0f, 0.0f, 0.0f), ::sSlopeDegree * PI / 180.0f);

    TPos3f transform;
    transform.concat(translation, rotation);

    for (int i = 0; i < USER_FILE_NUM; i++) {
        if (_B4 != nullptr && _B4 == mItems->getActor(i)) {
            continue;
        }

        f32 theta = static_cast< f32 >(-(i + 4)) * thetaStep - ::sItemThetaOffset[i] * PI / 180.0f;
        f32 cosTheta = MR::cos(theta);
        f32 sinTheta = MR::sin(theta);
        _98[i].set(::sItemPosRadius * cosTheta, 0.0f, ::sItemPosRadius * sinTheta);
        transform.mult(_98[i], _98[i]);
    }
}

void FileSelector::initAllItems() {
    s32 id;
    s32 index;
    FileSelectItem* pItem;

    for (int i = 0; i < USER_FILE_NUM; i++) {
        pItem = static_cast< FileSelectItem* >(mItems->getActor(i));
        id = pItem->_140;
        index = id - 1;

        if (!mUserFile[index].isCreated()) {
            continue;
        }

        FileSelectIconID iconId = FileSelectIconID();
        u32 tempIconId;
        RFLCreateID tempMiiId;

        if (mUserFile[index].getIconId(&tempIconId)) {
            iconId.setFellowID(getUserFileFellowID(id));
        } else if (mUserFile[index].getMiiId(&tempMiiId)) {
            if (::getMiiFacePartsHolder()->isError()) {
                pItem->_146 = true;
            } else if (isUserFileMiiIdValid(id)) {
                u32 miiIndex = getUserFileMiiIndex(id);

                iconId.setMiiIndex(miiIndex);
            } else {
                pItem->_146 = true;
            }
        }

        pItem->forceChange(iconId, _CC[id - 1]);
    }
}

void FileSelector::validateRotateAllItems() {
    for (int i = 0; i < mItems->getObjNum(); i++) {
        static_cast< FileSelectItem* >(mItems->getActor(i))->validateRotate();
    }
}

FileSelectIconID::EFellowID FileSelector::getUserFileFellowID(s32 id) const {
    u32 iconId;

    if (mUserFile[id - 1].getIconId(&iconId)) {
        FileSelectIconID::EFellowID fellowId = FileSelectIconID::Mario;

        if (iconId <= 5) {
            fellowId = static_cast< FileSelectIconID::EFellowID >(iconId - 1);
        }

        return fellowId;
    }

    return FileSelectIconID::Mario;
}

bool FileSelector::isUserFileMiiIdValid(s32 id) const {
    RFLCreateID createId;
    u16 index;

    if (mUserFile[id - 1].getMiiId(&createId)) {
        if (RFLSearchOfficialData(&createId, &index) == TRUE) {
            return RFLIsAvailableOfficialData(index) == TRUE;
        }
    }

    return false;
}

u16 FileSelector::getUserFileMiiIndex(s32 id) const {
    RFLCreateID createId;
    u16 index;

    if (mUserFile[id - 1].getMiiId(&createId)) {
        if (RFLSearchOfficialData(&createId, &index)) {
            return index;
        }
    }

    return 0;
}

bool FileSelector::isUserFileCorrupted(s32 id) const {
    id--;

    bool corrupted = false;

    if (mUserFile[id].mIsGameDataCorrupted || mUserFile[id].mIsConfigDataCorrupted) {
        corrupted = true;
    }

    return corrupted;
}

bool FileSelector::isUserFileAppearLuigi(s32 id) const {
    id--;

    return !mUserFile[id].mIsPlayerMario || mUserFile[id].isViewCompleteEnding();
}

bool FileSelector::isUserFileLuigi(s32 id) const {
    id--;

    return !mUserFile[id].mIsPlayerMario;
}

void FileSelector::setUserFileMario(s32 id, bool isMario) NO_INLINE {
    id--;

    mUserFile[id].mIsPlayerMario = isMario;
}

void FileSelector::storeSetMiiIdUserFile(s32 id, const FileSelectIconID& rIconId) {
    if (rIconId.isFellow()) {
        u32 fellowId = rIconId.getFellowID() + 1;

        GameSequenceFunction::storeMiiOrIconIdUserFileSequence(id, nullptr, &fellowId);
    } else {
        getMiiId(mMiiId, rIconId);
        GameSequenceFunction::storeMiiOrIconIdUserFileSequence(id, mMiiId, nullptr);
    }

    GameSequenceFunction::startSaveAllUserFileSequence();
}

void FileSelector::getMiiId(RFLCreateID* pCreateId, const FileSelectIconID& rIconId) const {
    RFLAdditionalInfo additionalInfo;
    u16 index = rIconId.getMiiIndex();

    if (RFLGetAdditionalInfo(&additionalInfo, RFLDataSource_Official, nullptr, index) == RFLErrcode_Success) {
        *pCreateId = additionalInfo.createID;
    }
}

void FileSelector::getIconId(FileSelectIconID* pIconId, s32 id) const {
    u32 iconId;
    RFLCreateID miiId;

    if (mUserFile[id - 1].getIconId(&iconId)) {
        FileSelectIconID::EFellowID fellowId = getUserFileFellowID(id);

        pIconId->setFellowID(fellowId);
    } else if (mUserFile[id - 1].getMiiId(&miiId) && isUserFileMiiIdValid(id)) {
        u16 miiIndex = getUserFileMiiIndex(id);

        pIconId->setMiiIndex(miiIndex);
    } else {
        pIconId->setFellowID(FileSelectIconID::Mario);
    }
}

s32 FileSelector::getMissCount(s32 id) const {
    if (!_CC[id - 1]) {
        return -1;
    }

    return mUserFile[id - 1].getPlayerMissNum();
}

void FileSelector::playSelectedME() {
    switch (MR::getRandom(0L, 4L)) {
    case 0:
        MR::startSystemME("ME_ASTRO_DOME_SELECT1");
        break;
    case 1:
        MR::startSystemME("ME_ASTRO_DOME_SELECT2");
        break;
    case 2:
        MR::startSystemME("ME_ASTRO_DOME_SELECT3");
        break;
    case 3:
        MR::startSystemME("ME_ASTRO_DOME_SELECT4");
        break;
    default:
        break;
    }
}

void FileSelector::updateBgm() {
    if (mCameraController == nullptr) {
        return;
    }

    s32 state = 0;
    u32 stateChangeFrames = 0;

    if (mCameraController->isToOrAtFarPoint()) {
        state = ::sBgmFarState;
        stateChangeFrames = ::sBgmFarStateChangeFrames;
    } else if (mCameraController->isToOrAtNearPoint()) {
        state = ::sBgmNearState;
        stateChangeFrames = ::sBgmNearStateChangeFrames;
    }

    if (mBgmState == state) {
        return;
    }

    MR::setStageBGMState(state, stateChangeFrames);

    mBgmState = state;
}

void FileSelector::exeWaitBind() {
    mPosition.set(*MR::getPlayerPos());
}

void FileSelector::exeTitle() {
    if (MR::isFirstStep(this)) {
        mTitle->appear();
        MR::deactivateDefaultGameLayout();
        MR::startStarPointerModeTitle(this);
        MR::resetCameraMan();
    }

    mTitle->updateNerve();

    if (mTitle->isActive()) {
        return;
    }

    if (::getMiiFacePartsHolder()->isInitEnd()) {
        if (::getMiiFacePartsHolder()->isError()) {
            setNerve(GET_NERVE(FileSelector, FileSelectorNrvRFLError));
        } else {
            setNerve(GET_NERVE(FileSelector, FileSelectorNrvTitleEnd));
        }
    } else {
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvRFLWait));
    }
}

void FileSelector::exeTitleEnd() {
    if (MR::isFirstStep(this)) {
        mCameraController->goToFarPoint();
        calcBasePos(0.0f);
        appearAllItems();
        initAllItems();
        mMiiSelect->collectValidMiiIndex();
        invalidateSelectAll();
        MR::startStarPointerModeFileSelect(this);
        MR::startStageBGM("MBGM_FILE_SELECT", false);
    }

    if (mCameraController->isAtFarPoint()) {
        validateRotateAllItems();
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvFileSelect));
    }
}

void FileSelector::exeRFLError() {
    if (MR::isFirstStep(this)) {
        mSysInfoWindow->appear("RFL_02", SysInfoWindow::Type_Key, SysInfoWindow::TextPos_Center, SysInfoWindow::MessageType_System);
    }

    if (MR::isDead(mSysInfoWindow)) {
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvTitleEnd));
    }
}

void FileSelector::exeRFLWait() {
    if (MR::isFirstStep(this)) {
        mSysInfoWindowMini->appear("RFL_01", SysInfoWindow::Type_Blocking, SysInfoWindow::TextPos_Center, SysInfoWindow::MessageType_System);
    }

    if (!::getMiiFacePartsHolder()->isInitEnd()) {
        return;
    }

    if (!mSysInfoWindowMini->isWait()) {
        return;
    }

    setNerve(GET_NERVE(FileSelector, FileSelectorNrvRFLWaitEnd));
}

void FileSelector::exeRFLWaitEnd() {
    if (MR::isFirstStep(this)) {
        mSysInfoWindowMini->disappear();
    }

    if (MR::isDead(mSysInfoWindowMini)) {
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvTitleEnd));
    }
}

void FileSelector::exeFileSelectStart() {
    if (MR::isFirstStep(this)) {
        mCameraController->goToFarPoint();
        calcBasePos(0.0f);
    }

    if (mCameraController->isAtFarPoint()) {
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvFileSelect));
    }
}

void FileSelector::exeFileSelect() {
    if (MR::isFirstStep(this)) {
        validateSelectAll();
        appearAllIndex();
        MR::activeStarPointerGuidance();
    } else {
        updateFileInfo();
    }

    MR::requestFileSelectGuidance();
}

void FileSelector::exeFileConfirmStart() {
    if (MR::isFirstStep(this)) {
        goToNearPoint();
        invalidateSelectAll();
        disappearAllIndex();
        _B4->turnToFront(40);
    }

    if (!mCameraController->isAtNearPoint()) {
        return;
    }

    if (isUserFileCorrupted(_B4->_140)) {
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvFileBroken));
    } else if (_B4->_146 != 0) {
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvFileConfirmMiiDeleteWarningStart));
    } else {
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvFileConfirm));
    }
}

void FileSelector::exeFileConfirmMiiDeleteWarningStart() {
    if (MR::isFirstStep(this)) {
        disappearAllLayout();
    }

    if (isHiddenAllLayout()) {
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvFileConfirmMiiDeleteWarning));
    }
}

void FileSelector::exeFileConfirmMiiDeleteWarning() {
    if (MR::isFirstStep(this)) {
        mSysInfoWindowMini->appear("System_FileSelect009", SysInfoWindow::Type_Key, SysInfoWindow::TextPos_Center, SysInfoWindow::MessageType_System);
    }

    if (MR::isDead(mSysInfoWindowMini)) {
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvFileConfirmMiiDeleteSave));
    }
}

void FileSelector::exeFileConfirmMiiDeleteSave() {
    if (MR::isFirstStep(this)) {
        u32 tempIconId = 1;

        GameSequenceFunction::startSetMiiOrIconIdUserFileSequence(_B4->_140, nullptr, &tempIconId);
    }

    if (GameSequenceFunction::isActiveSaveDataHandleSequence()) {
        return;
    }

    _B4->_146 = 0;

    restoreUserFile();
    setNerve(GET_NERVE(FileSelector, FileSelectorNrvFileConfirm));
}

void FileSelector::exeFileConfirm() {
    if (MR::isFirstStep(this)) {
        mOperationButton->appear();
        mBackButton->appear();

        if (isUserFileAppearLuigi(_B4->_140)) {
            bool isAppear = !isUserFileLuigi(_B4->_140);

            mBrosButton->appear(isAppear);
        }

        if (MR::isDead(mFileInfo)) {
            setFileInfo(_B4->_140);
            mFileInfo->appear();
            mFileInfo->forceChange();
        }

        mFileInfo->slide();
    }

    if (!MR::isDead(mBrosButton) && mBrosButton->isSelected()) {
        s32 id = _B4->_140;

        if (mBrosButton->isSelectedMario()) {
            setUserFileMario(id, true);
        } else {
            setUserFileMario(id, false);
        }

        restoreUserFile();
        setFileInfo(id);
        mFileInfo->change();
        mBrosButton->resume();
    }

    if (!checkSelectedBackButton()) {
        return;
    }

    disappearAllLayout();
    clearPointing();
    setNerve(GET_NERVE(FileSelector, FileSelectorNrvFileSelectStart));
}

void FileSelector::exeDemoStartWait() {
    if (MR::isFirstStep(this)) {
        MR::startSystemSE("SE_SY_FILE_SELECTED");
        MR::stopStageBGM(90);
        disappearAllLayout();
        MR::closeWipeFade(60);
    }

    if (!MR::isWipeBlank()) {
        return;
    }

    setNerve(GET_NERVE(FileSelector, FileSelectorNrvDemo));
}

void FileSelector::exeDemo() {
    if (MR::isFirstStep(this)) {
        s32 id = _B4->_140;
        bool isStartLoadSequence = !isUserFileLuigi(id);

        GameSequenceFunction::startGameDataLoadSequence(id, isStartLoadSequence);
        MR::stopStageBGM(90);
    }

    if (GameSequenceFunction::isActiveSaveDataHandleSequence()) {
        return;
    }

    wchar_t name[RFL_NAME_LEN + 1];
    FileSelectIconID iconId = FileSelectIconID();

    getIconId(&iconId, _B4->_140);

    if (iconId.isMii()) {
        FileSelectFunc::copyMiiName(reinterpret_cast< u16* >(name), iconId);
    } else if (isUserFileLuigi(_B4->_140)) {
        const wchar_t* pMessage = MR::getGameMessageDirect(::cLuigiNameMessageID);

        MR::copyString(name, pMessage, ARRAY_SIZE(name));
    } else {
        const wchar_t* pMessage = MR::getGameMessageDirect(::cMarioNameMessageID);

        MR::copyString(name, pMessage, ARRAY_SIZE(name));
    }

    GameSequenceFunction::reserveUserName(name);
    MR::requestChangeStageInGameAfterLoadingGameData();
}

void FileSelector::exeCreateConfirmStart() {
    if (MR::isFirstStep(this)) {
        goToNearPoint();
        invalidateSelectAll();
        disappearAllIndex();
    }

    if (!mCameraController->isAtNearPoint()) {
        return;
    }

    setNerve(GET_NERVE(FileSelector, FileSelectorNrvCreateConfirm));
}

void FileSelector::exeCreateConfirm() {
    if (MR::isFirstStep(this)) {
        mSysInfoWindow->appear("System_FileSelect001", SysInfoWindow::Type_YesNo, SysInfoWindow::TextPos_Center, SysInfoWindow::MessageType_System);
        mSysInfoWindow->setYesNoSelectorSE("SE_SY_BUTTON_CURSOR_ON", "SE_SY_FILE_SEL_NEW_FILE", "SE_SY_TALK_SELECT_NO");
    }

    if (!MR::isDead(mSysInfoWindow)) {
        return;
    }

    mSysInfoWindow->resetYesNoSelectorSE();

    if (mSysInfoWindow->isSelectedYes()) {
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvCreate));
    } else {
        clearPointing();
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvFileSelectStart));
    }
}

void FileSelector::exeCreate() {
    if (MR::isFirstStep(this)) {
        GameSequenceFunction::startCreateUserFileSequence(_B4->_140);
    }

    if (GameSequenceFunction::isActiveSaveDataHandleSequence()) {
        return;
    }

    if (GameSequenceFunction::isSuccessSaveDataHandleSequence()) {
        restoreUserFile();
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvMiiSelectStartFirst));
    } else {
        clearPointing();
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvFileSelectStart));
    }
}

void FileSelector::exeCopyWait() {
    if (MR::isFirstStep(this)) {
        MR::startSystemSE("SE_SY_FILE_SEL_UPPER_DECIDE");

        _B8 = _B4;

        disappearAllLayout();
        clearPointing();
        mCameraController->goToFarPoint();
        calcBasePos(0.0f);
    }

    if (!mCameraController->isAtFarPoint()) {
        return;
    }

    setNerve(GET_NERVE(FileSelector, FileSelectorNrvCopySelect));
}

void FileSelector::exeCopySelect() {
    if (MR::isFirstStep(this)) {
        validateSelectAll();
        _B8->invalidateSelect();
        mBackButton->appear();
        appearAllIndex();
        MR::activeStarPointerGuidance();
        mSelectEffect[::getItemArrayIndex(_B8->_140)].appear();
    }

    MR::requestFileSelectCopyGuidance();
    updateFileInfo();

    if (mBackButton->isPointing()) {
        clearPointing();
        mFileInfo->disappear();
    }

    if (!checkSelectedBackButton()) {
        return;
    }

    invalidateSelectAll();
    mSelectEffect[::getItemArrayIndex(_B8->_140)].disappear();
    mInfoMessage->disappear();
    MR::deactiveStarPointerGuidance();
    setFileInfo(_B4->_140);
    mFileInfo->appear();
    mFileInfo->forceChange();
    setNerve(GET_NERVE(FileSelector, FileSelectorNrvFileConfirmStart));
}

void FileSelector::exeCopyConfirmStart() {
    if (MR::isFirstStep(this)) {
        disappearAllLayout();
        invalidateSelectAll();
        mSelectEffect[::getItemArrayIndex(_B8->_140)].disappear();
    }

    if (!isHiddenAllLayout()) {
        return;
    }

    setNerve(GET_NERVE(FileSelector, FileSelectorNrvCopyConfirm));
}

void FileSelector::exeCopyConfirm() {
    if (MR::isFirstStep(this)) {
        clearPointing();

        if (_B4->isNew()) {
            mSysInfoWindow->appear("System_FileSelect016", SysInfoWindow::Type_YesNo, SysInfoWindow::TextPos_Center,
                                   SysInfoWindow::MessageType_System);
        } else {
            mSysInfoWindow->appear("System_FileSelect014", SysInfoWindow::Type_YesNo, SysInfoWindow::TextPos_Center,
                                   SysInfoWindow::MessageType_System);
        }

        mSysInfoWindow->setTextBoxArgNumber(_B8->_140, 0);
        mSysInfoWindow->setTextBoxArgNumber(_B4->_140, 1);
    }

    if (!MR::isDead(mSysInfoWindow)) {
        return;
    }

    if (mSysInfoWindow->isSelectedYes()) {
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvCopySave));
    } else {
        _B4 = _B8;

        setNerve(GET_NERVE(FileSelector, FileSelectorNrvCopySelect));
    }
}

void FileSelector::exeCopySave() {
    if (MR::isFirstStep(this)) {
        if (isUserFileLuigi(_B8->_140)) {
            setUserFileMario(_B4->_140, false);
        } else {
            setUserFileMario(_B4->_140, true);
        }

        if (_B4->isNew()) {
            GameSequenceFunction::startCopyUserFileSequence(_B4->_140, _B8->_140);
        } else {
            GameSequenceFunction::storeCopyUserFileSequence(_B4->_140, _B8->_140);
            setNerve(GET_NERVE(FileSelector, FileSelectorNrvCopySaveMii));
            return;
        }
    }

    if (GameSequenceFunction::isActiveSaveDataHandleSequence()) {
        return;
    }

    setNerve(GET_NERVE(FileSelector, FileSelectorNrvCopyDemo));
}

void FileSelector::exeCopySaveMii() {
    if (MR::isFirstStep(this)) {
        RFLCreateID createId;

        if (_B4->_146 && mUserFile[_B4->_140 - 1].getMiiId(&createId)) {
            GameSequenceFunction::storeMiiOrIconIdUserFileSequence(_B4->_140, &createId, nullptr);
            GameSequenceFunction::startSaveAllUserFileSequence();
        } else {
            FileSelectIconID iconId = FileSelectIconID();

            _B4->copyIconID(&iconId);
            storeSetMiiIdUserFile(_B4->_140, iconId);
        }
    }

    if (GameSequenceFunction::isActiveSaveDataHandleSequence()) {
        return;
    }

    setNerve(GET_NERVE(FileSelector, FileSelectorNrvCopyDemo));
}

void FileSelector::exeCopyDemo() {
    if (MR::isFirstStep(this)) {
        MR::startSystemSE("SE_SY_FILE_SEL_COPY");
        restoreUserFile();

        FileSelectIconID iconId = FileSelectIconID();

        getIconId(&iconId, _B4->_140);
        _B4->change(iconId, _CC[_B4->_140 - 1]);
        mBackButton->disappear();
        mInfoMessage->disappear();
        invalidateSelectAll();
    }

    if (!_B4->isExist()) {
        return;
    }

    clearPointing();
    setNerve(GET_NERVE(FileSelector, FileSelectorNrvFileSelect));
}

void FileSelector::exeCopyRejectStart() {
    if (MR::isFirstStep(this)) {
        disappearAllLayout();
    }

    if (!isHiddenAllLayout()) {
        return;
    }

    setNerve(GET_NERVE(FileSelector, FileSelectorNrvCopyReject));
}

void FileSelector::exeCopyReject() {
    if (MR::isFirstStep(this)) {
        mSysInfoWindowMini->appear("System_FileSelect003", SysInfoWindow::Type_Key, SysInfoWindow::TextPos_Center, SysInfoWindow::MessageType_System);
        MR::startSystemSE("SE_SY_FILE_SEL_NG");
    }

    if (!MR::isDead(mSysInfoWindowMini)) {
        return;
    }

    mOperationButton->shiftSelect();
    setNerve(GET_NERVE(FileSelector, FileSelectorNrvFileConfirm));
}

void FileSelector::exeMiiWait() {
    if (MR::isFirstStep(this)) {
        MR::startSystemSE("SE_SY_FILE_SEL_UPPER_DECIDE");
    }

    if (!mBackButton->isHidden()) {
        return;
    }

    if (!MR::isDead(mFileInfo)) {
        return;
    }

    setNerve(GET_NERVE(FileSelector, FileSelectorNrvMiiSelectStart));
}

void FileSelector::exeMiiTip() {
    if (MR::isFirstStep(this)) {
        mSysInfoWindow->appear("System_FileSelect004", SysInfoWindow::Type_Key, SysInfoWindow::TextPos_Center, SysInfoWindow::MessageType_System);
        clearPointing();
        mFileInfo->disappear();
    }

    if (!MR::isDead(mSysInfoWindow)) {
        return;
    }

    setNerve(GET_NERVE(FileSelector, FileSelectorNrvMiiSelectStart));
}

void FileSelector::exeMiiSelectStart() {
    if (MR::isFirstStep(this)) {
        if (isNerve(GET_NERVE(FileSelector, FileSelectorNrvMiiSelectStart))) {
            FileSelectIconID iconId = FileSelectIconID();

            _B4->copyIconID(&iconId);
            mMiiSelect->prohibitIcon(iconId);
        } else {
            mIsMiiSelectStartFirst = true;

            mMiiSelect->admitIcon();
        }

        if (isUserFileAppearLuigi(_B4->_140)) {
            mMiiSelect->validateAllSpecialMii();
        } else {
            mMiiSelect->invalidateSpecialMii(FileSelectIconID::Luigi);
        }

        mMiiSelect->appear();
    }

    if (mMiiSelect->isAppearing()) {
        return;
    }

    setNerve(GET_NERVE(FileSelector, FileSelectorNrvMiiSelect));
}

void FileSelector::exeMiiSelect() {
    if (MR::isFirstStep(this)) {
        if (!mIsMiiSelectStartFirst) {
            mBackButton->appear();
        }
    }

    if (mMiiSelect->isDummySelected()) {
        MR::startSystemSE("SE_SY_FILE_SEL_MII_SELECTED");
        mMiiSelect->disappear();

        if (!mBackButton->isHidden()) {
            mBackButton->disappear();
        }

        setNerve(GET_NERVE(FileSelector, FileSelectorNrvMiiInfoStart));
    } else if (mMiiSelect->isSelected()) {
        MR::startSystemSE("SE_SY_FILE_SEL_MII_SELECTED");
        mMiiSelect->disappear();

        if (!mBackButton->isHidden()) {
            mBackButton->disappear();
        }

        setNerve(GET_NERVE(FileSelector, FileSelectorNrvMiiConfirmWait));
    } else if (checkSelectedBackButton()) {
        mMiiSelect->disappear();
        mBackButton->disappear();
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvMiiCancel));
    }
}

void FileSelector::exeMiiCancel() {
    if (!MR::isDead(mMiiSelect)) {
        return;
    }

    if (!mBackButton->isHidden()) {
        return;
    }

    setNerve(GET_NERVE(FileSelector, FileSelectorNrvFileConfirm));
}

void FileSelector::exeMiiConfirmWait() {
    if (!MR::isDead(mMiiSelect)) {
        return;
    }

    if (!mBackButton->isHidden()) {
        return;
    }

    setNerve(GET_NERVE(FileSelector, FileSelectorNrvMiiConfirm));
}

void FileSelector::exeMiiConfirm() {
    if (MR::isFirstStep(this)) {
        if (mIsMiiSelectStartFirst) {
            mSysInfoWindow->appear("System_FileSelect013", SysInfoWindow::Type_YesNo, SysInfoWindow::TextPos_Bottom,
                                   SysInfoWindow::MessageType_System);
        } else {
            mSysInfoWindow->appear("System_FileSelect005", SysInfoWindow::Type_YesNo, SysInfoWindow::TextPos_Bottom,
                                   SysInfoWindow::MessageType_System);
        }

        u16 name[RFL_NAME_LEN + 1];
        FileSelectIconID iconId = FileSelectIconID();

        mMiiSelect->getSelectedID(&iconId);
        FileSelectFunc::copyMiiName(name, iconId);
        mMiiConfirmIcon->appear(mMiiSelect->getSelectedMiiTexMap(), reinterpret_cast< wchar_t* >(name));
        mSysInfoWindow->setYesNoSelectorSE("SE_SY_BUTTON_CURSOR_ON", "SE_SY_FILE_SEL_MII_CHANGE", "SE_SY_TALK_SELECT_NO");
    }

    if (!mMiiConfirmIcon->isDisappear() && mSysInfoWindow->isDisappear()) {
        mMiiConfirmIcon->disappear();
    }

    if (!MR::isDead(mSysInfoWindow)) {
        return;
    }

    mSysInfoWindow->resetYesNoSelectorSE();

    if (mSysInfoWindow->isSelectedYes()) {
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvMiiCreateWait));
    } else if (mIsMiiSelectStartFirst) {
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvMiiSelectStartFirst));
    } else {
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvMiiSelectStart));
    }
}

void FileSelector::exeMiiCreateWait() {
    if (MR::isFirstStep(this)) {
        FileSelectIconID iconId = FileSelectIconID();

        mMiiSelect->getSelectedID(&iconId);
        storeSetMiiIdUserFile(_B4->_140, iconId);

        mIsMiiSelectStartFirst = false;
    }

    if (GameSequenceFunction::isActiveSaveDataHandleSequence()) {
        return;
    }

    setNerve(GET_NERVE(FileSelector, FileSelectorNrvMiiCreateDemo));
}

void FileSelector::exeMiiCreateDemo() {
    if (MR::isFirstStep(this)) {
        restoreUserFile();

        s32 id = _B4->_140;
        FileSelectIconID iconId = FileSelectIconID();

        mMiiSelect->getSelectedID(&iconId);
        _B4->change(iconId, _CC[id - 1]);
        setFileInfo(id);
    }

    if (!_B4->isExist()) {
        return;
    }

    FileSelectIconID iconId = FileSelectIconID();

    mMiiSelect->getSelectedID(&iconId);

    if (iconId.isMii()) {
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvMiiCaution));
    } else {
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvFileConfirm));
    }
}

void FileSelector::exeMiiCaution() {
    if (MR::isFirstStep(this)) {
        mSysInfoWindowMini->appear("System_FileSelect006", SysInfoWindow::Type_Key, SysInfoWindow::TextPos_Center, SysInfoWindow::MessageType_System);
    }

    if (MR::isDead(mSysInfoWindowMini)) {
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvFileConfirm));
    }
}

void FileSelector::exeMiiInfoStart() {
    if (!MR::isDead(mMiiSelect)) {
        return;
    }

    if (!mBackButton->isHidden()) {
        return;
    }

    setNerve(GET_NERVE(FileSelector, FileSelectorNrvMiiInfo));
}

void FileSelector::exeMiiInfo() {
    if (MR::isFirstStep(this)) {
        mSysInfoWindowMini->appear("System_FileSelect015", SysInfoWindow::Type_Key, SysInfoWindow::TextPos_Center, SysInfoWindow::MessageType_System);
    }

    if (!MR::isDead(mSysInfoWindowMini)) {
        return;
    }

    if (mIsMiiSelectStartFirst) {
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvMiiSelectStartFirst));
    } else {
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvMiiSelectStart));
    }
}

void FileSelector::exeDeleteConfirmStart() {
    if (MR::isFirstStep(this)) {
        MR::startSystemSE("SE_SY_FILE_SEL_UPPER_DECIDE");
        disappearAllLayout();
    }

    if (isHiddenAllLayout()) {
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvDeleteConfirm));
    }
}

void FileSelector::exeDeleteConfirm() {
    if (MR::isFirstStep(this)) {
        mSysInfoWindow->setYesNoSelectorSE("SE_SY_BUTTON_CURSOR_ON", "SE_SY_FILE_SEL_DELETE", "SE_SY_TALK_SELECT_NO");
        mSysInfoWindow->appear("System_FileSelect007", SysInfoWindow::Type_YesNo, SysInfoWindow::TextPos_Center, SysInfoWindow::MessageType_System);
    }

    if (!MR::isDead(mSysInfoWindow)) {
        return;
    }

    mSysInfoWindow->resetYesNoSelectorSE();

    if (mSysInfoWindow->isSelectedYes()) {
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvDelete));
    } else {
        mOperationButton->shiftSelect();
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvFileConfirm));
    }
}

void FileSelector::exeDelete() {
    if (MR::isFirstStep(this)) {
        setUserFileMario(_B4->_140, true);
        GameSequenceFunction::startDeleteUserFileSequence(_B4->_140);
    }

    if (GameSequenceFunction::isActiveSaveDataHandleSequence()) {
        return;
    }

    if (GameSequenceFunction::isSuccessSaveDataHandleSequence()) {
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvDeleteDemo));
    } else {
        clearPointing();
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvFileSelectStart));
    }
}

void FileSelector::exeDeleteDemo() {
    if (MR::isFirstStep(this)) {
        _B4->format();
    }

    if (_B4->isNew()) {
        initUserFile();
        clearPointing();
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvFileSelectStart));
    }
}

void FileSelector::exeFileBroken() {
    if (MR::isFirstStep(this)) {
        mSysInfoWindow->appear("System_FileSelect012", SysInfoWindow::Type_Key, SysInfoWindow::TextPos_Center, SysInfoWindow::MessageType_System);
        MR::startSystemSE("SE_SY_FILE_SEL_NG");
    }

    if (MR::isDead(mSysInfoWindow)) {
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvFileConfirm));
    }
}

void FileSelector::exeManualStart() {
    if (MR::isFirstStep(this)) {
        disappearAllLayout();
    }

    if (isHiddenAllLayout()) {
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvManual));
    }
}

void FileSelector::exeManual() {
    if (MR::isFirstStep(this)) {
        mManual->appear();
    }

    if (mManual->isClosed()) {
        setNerve(GET_NERVE(FileSelector, FileSelectorNrvFileConfirm));
    }
}

FileSelector::~FileSelector() {
}
