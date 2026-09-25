#include "Game/Screen/MiiSelect.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/FileSelectFunc.hpp"
#include "Game/Map/FileSelectIconID.hpp"
#include "Game/Screen/ButtonPaneController.hpp"
#include "Game/Screen/MiiSelectIcon.hpp"
#include "Game/Util/BitArray.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "revolution/types.h"
#include <JSystem/J3DGraphAnimator/J3DAnimation.hpp>
#include <RVLFaceLib.h>

namespace {
    NEW_NERVE(MiiSelectNrvAppear, MiiSelect, Appear);
    NEW_NERVE(MiiSelectNrvWait, MiiSelect, Wait);
    NEW_NERVE(MiiSelectNrvScrollRight, MiiSelect, ScrollRight);
    NEW_NERVE(MiiSelectNrvScrollLeft, MiiSelect, ScrollLeft);
    NEW_NERVE(MiiSelectNrvSelected, MiiSelect, Selected);
    NEW_NERVE(MiiSelectNrvDisappear, MiiSelect, Disappear);
    NEW_NERVE(MiiSelectNrvDummySelected, MiiSelect, DummySelected);

    typedef void (MiiSelect::*CallbackFuncPtr)();

    static const CallbackFuncPtr func[2] = {&MiiSelect::callbackLeft, &MiiSelect::callbackRight};
}  // namespace

MiiSelect::MiiSelect(const char* pName)
    : LayoutActor(pName, true), _28(new MR::BitArray(5)), mFellowIconNum(), mFavoriteMiiNum(), mMiiNum(), _1EC(), _1F0(), _1F4(), _1F8(), _1FC(),
      _200(), _204(new FileSelectIconID()) {
    for (int i = 0; i < ARRAY_SIZE(_20); i++) {
        _20[i] = nullptr;
    }

    validateAllSpecialMii();
}

void MiiSelect::init(const JMapInfoIter& rIter) {
    initLayoutManager("MiiSelect", 1);
    MR::invalidateParentAnim(this);
    createButtons();
    MR::connectToSceneLayout(this);
    initNerve(GET_NERVE_ANON(MiiSelectNrvAppear));
    createPage();
    kill();
}

void MiiSelect::appear() {
    LayoutActor::appear();
    setNerve(GET_NERVE_ANON(MiiSelectNrvAppear));
    _1EC = 0;
    refresh();
    setCurrentPageGroupA();
    appearButtons();
    updateButtons();
    _1F8->invalidateAllIcon();
    _1FC->invalidateAllIcon();
    MR::setTextBoxMessageRecursive(this, "TxtName", L"");
}

void MiiSelect::disappear() {
    _1F8->invalidateAllIcon();
    _1FC->invalidateAllIcon();
    disappearButtons();
    setNerve(GET_NERVE_ANON(MiiSelectNrvDisappear));
}

void MiiSelect::calcAnim() {
    LayoutActor::calcAnim();
    _1F8->calcAnim();
    _1FC->calcAnim();
}

bool MiiSelect::isAppearing() const {
    return isNerve(GET_NERVE_ANON(MiiSelectNrvAppear));
}

bool MiiSelect::isSelected() {
    return isNerve(GET_NERVE_ANON(MiiSelectNrvSelected));
}

bool MiiSelect::isDummySelected() {
    return isNerve(GET_NERVE_ANON(MiiSelectNrvDummySelected));
}

void MiiSelect::getSelectedID(FileSelectIconID* pSelectedID) {
    getIconID(pSelectedID, _1F0);
}

nw4r::lyt::TexMap* MiiSelect::getSelectedMiiTexMap() {
    return _1F4;
}

void MiiSelect::admitIcon() {
    _200 = 0;
}

void MiiSelect::prohibitIcon(const FileSelectIconID& rIconID) {
    _200 = 1;

    _204->set(rIconID);
}

void MiiSelect::invalidateSpecialMii(FileSelectIconID::EFellowID fellowID) {
    if (_28->isOn(fellowID)) {
        _28->set(fellowID, false);

        mFellowIconNum--;
    }
}

void MiiSelect::validateAllSpecialMii() {
    for (int i = 0; i < _28->size(); i++) {
        _28->set(i, true);
    }

    mFellowIconNum = _28->size();
}

void MiiSelect::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Appear", 0);
        MR::startSystemSE("SE_SY_FILE_SEL_MIISEL_OPEN");
        setCurrentPageNum();
    }

    if (MR::isAnimStopped(this, 0)) {
        setNerve(GET_NERVE_ANON(MiiSelectNrvWait));
    }
}

void MiiSelect::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Wait", 0);
        validateAllIcon();
        setCurrentPageGroupB();

        for (int i = 0; i < ARRAY_SIZE(_20); i++) {
            if (_20[i]->isHidden()) {
                continue;
            }

            _20[i]->_24 = true;
            _20[i]->forceToWait();
        }

        setCurrentPageNum();
    }

    for (int i = 0; i < ARRAY_SIZE(_20); i++) {
        if (_20[i]->trySelect()) {
            (this->*::func[i])();
            return;
        }
    }

    if (!_20[0]->isHidden() && (MR::testCorePadTriggerLeft(WPAD_CHAN0) || MR::testSubPadStickTriggerLeft(WPAD_CHAN0))) {
        callbackLeft();
    } else if (!_20[1]->isHidden() && (MR::testCorePadTriggerRight(WPAD_CHAN0) || MR::testSubPadStickTriggerRight(WPAD_CHAN0))) {
        callbackRight();
    }
}

void MiiSelect::exeScrollLeft() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "PreviousMii", 0);

        _1EC -= 8;

        if (_1EC < 0) {
            _1EC = ((getIconNum() - 1) / 8) * 8;
        }

        flipPage();
        setCurrentPageGroupB();
        refresh();
        updateButtons();
        _1F8->invalidateAllIcon();
        _1FC->invalidateAllIcon();
    }

    if (MR::isAnimStopped(this, 0)) {
        setNerve(GET_NERVE_ANON(MiiSelectNrvWait));
    }
}

void MiiSelect::exeScrollRight() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "NextMii", 0);

        _1EC += 8;

        if (_1EC >= getIconNum()) {
            _1EC = 0;
        }

        flipPage();
        setCurrentPageGroupB();
        refresh();
        updateButtons();
        _1F8->invalidateAllIcon();
        _1FC->invalidateAllIcon();
    }

    if (MR::isAnimStopped(this, 0)) {
        setNerve(GET_NERVE_ANON(MiiSelectNrvWait));
    }
}

void MiiSelect::exeSelected() {
}

void MiiSelect::exeDisappear() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "End", 0);
    }

    if (MR::isAnimStopped(this, 0)) {
        kill();
    }
}

void MiiSelect::exeDummySelected() {
}

void MiiSelect::control() {
    for (int i = 0; i < ARRAY_SIZE(_20); i++) {
        _20[i]->update();
    }

    J3DFrameCtrl* pAnimCtrl;

    if (_20[0]->isFirstStepWait() && !_20[1]->isFirstStepWait() && _20[1]->isWait()) {
        pAnimCtrl = MR::getPaneAnimCtrl(this, _20[0]->mPaneName, 0);

        pAnimCtrl->setFrame(MR::getPaneAnimFrame(this, _20[1]->mPaneName, 0));
    }

    if (_20[1]->isFirstStepWait() && !_20[0]->isFirstStepWait() && _20[0]->isWait()) {
        pAnimCtrl = MR::getPaneAnimCtrl(this, _20[1]->mPaneName, 0);

        pAnimCtrl->setFrame(MR::getPaneAnimFrame(this, _20[0]->mPaneName, 0));
    }

    _1F8->movement();
}

void MiiSelect::createButtons() {
    static const char* pane[] = {
        "Left",
        "Right",
    };
    static const char* bound[] = {
        "PicArrowL",
        "PicArrowR",
    };

    for (int i = 0; i < ARRAY_SIZE(_20); i++) {
        MR::createAndAddPaneCtrl(this, pane[i], 1);

        _20[i] = new ButtonPaneController(this, pane[i], bound[i], 0, true);
        _20[i]->_22 = false;
    }
}

void MiiSelect::callbackLeft() {
    _20[0]->_24 = false;

    MR::startSystemSE("SE_SY_FILE_SEL_MIISEL_SCRL");
    setNerve(GET_NERVE_ANON(MiiSelectNrvScrollLeft));
}

void MiiSelect::callbackRight() {
    _20[1]->_24 = false;

    MR::startSystemSE("SE_SY_FILE_SEL_MIISEL_SCRL");
    setNerve(GET_NERVE_ANON(MiiSelectNrvScrollRight));
}

void MiiSelect::appearButtons() {
    for (int i = 0; i < ARRAY_SIZE(_20); i++) {
        _20[i]->appear();
    }
}

void MiiSelect::disappearButtons() {
    for (int i = 0; i < ARRAY_SIZE(_20); i++) {
        _20[i]->disappear();
    }
}

void MiiSelect::updateButtons() {
    if (getIconNum() > 16) {
        if (_20[0]->isHidden()) {
            _20[0]->appear();
        }

        if (_20[1]->isHidden()) {
            _20[1]->appear();
        }
    } else {
        if (_1EC - 8 < 0) {
            if (!_20[0]->isHidden()) {
                _20[0]->disappear();
            }
        } else if (_20[0]->isHidden()) {
            _20[0]->appear();
        }

        if (_1EC + 8 >= getIconNum()) {
            if (!_20[1]->isHidden()) {
                _20[1]->disappear();
            }
        } else if (_20[1]->isHidden()) {
            _20[1]->appear();
        }
    }
}

void MiiSelect::collectValidMiiIndex() {
    for (u32 i = 0; i < RFL_DB_CHAR_MAX; i++) {
        if (!RFLIsAvailableOfficialData(i)) {
            continue;
        }

        RFLAdditionalInfo additionalInfo;

        if (RFLGetAdditionalInfo(&additionalInfo, RFLDataSource_Official, nullptr, i) != RFLErrcode_Success) {
            continue;
        }

        if (additionalInfo.favorite == 1) {
            mFavoriteMiiIndices[mFavoriteMiiNum] = static_cast< u16 >(i);
            mFavoriteMiiNum++;
        } else {
            mMiiIndices[mMiiNum] = static_cast< u16 >(i);
            mMiiNum++;
        }
    }
}

void MiiSelect::createPage() {
    _1F8 = new MiiSelectSub::Page(this);
    _1FC = new MiiSelectSub::Page(this);
}

void MiiSelect::flipPage() {
    MiiSelectSub::Page* pPage = _1F8;

    _1F8 = _1FC;
    _1FC = pPage;
}

void MiiSelect::setCurrentPageGroupA() {
    _1F8->_20 = 1;
    _1FC->_20 = 0;
}

void MiiSelect::setCurrentPageGroupB() {
    _1F8->_20 = 0;
    _1FC->_20 = 1;
}

void MiiSelect::setCurrentPageNum() {
    s32 v2 = (getIconNum() - 1) / 8;
    s32 v1 = _1EC / 8;

    MR::setTextBoxFormatRecursive(this, "TxtPage", L"%d/%d", v1 + 1, v2 + 1);
}

void MiiSelect::validateAllIcon() {
    _1F8->validateAllIcon();
    _1FC->validateAllIcon();

    if (_200) {
        _1F8->prohibitIcon(*_204);
        _1FC->prohibitIcon(*_204);
    }
}

void MiiSelect::refresh() {
    _1F8->refresh(_1EC);

    if (_200) {
        _1F8->prohibitIcon(*_204);
        _1FC->prohibitIcon(*_204);
    }
}

void MiiSelect::getIconID(FileSelectIconID* pIconID, s32 param2) const {
    if (param2 < mFellowIconNum) {
        s32 v = 0;

        for (int i = 0; i < _28->size(); i++) {
            if (!_28->isOn(i)) {
                continue;
            }

            if (v == param2) {
                pIconID->setFellowID(static_cast< FileSelectIconID::EFellowID >(i));
                break;
            }

            v++;
        }
    } else if (param2 < mFavoriteMiiNum + mFellowIconNum) {
        pIconID->setMiiIndex(mFavoriteMiiIndices[param2 - mFellowIconNum]);
    } else {
        pIconID->setMiiIndex(mMiiIndices[param2 - mFavoriteMiiNum - mFellowIconNum]);
    }
}

void MiiSelect::onSelect(s32 param1, nw4r::lyt::TexMap* pTexMap) {
    _1F0 = param1;
    _1F4 = pTexMap;

    _1F8->invalidateAllIcon();
    _1FC->invalidateAllIcon();
    disappearButtons();
    setNerve(GET_NERVE_ANON(MiiSelectNrvSelected));
}

void MiiSelect::onSelectDummy() {
    _1F8->invalidateAllIcon();
    _1FC->invalidateAllIcon();
    disappearButtons();
    setNerve(GET_NERVE_ANON(MiiSelectNrvDummySelected));
}

namespace MiiSelectSub {
    Page::Page(MiiSelect* pHost) : mHost(pHost), _20(true) {
        for (u32 i = 0; i < ARRAY_SIZE(mIconArray); i++) {
            mIconArray[i] = new MiiSelectIcon(-1, -1, -1, "Miiセレクト用アイコン");
        }
    }

    void Page::refresh(s32 baseIndex) {
        mBaseIndex = baseIndex;

        s32 iconNum = mHost->getIconNum();

        for (s32 i = 0; i < ARRAY_SIZE(mIconArray); i++) {
            if (mHost->isExistMiiIcon()) {
                if (baseIndex + i >= iconNum) {
                    mIconArray[i]->kill();
                    continue;
                }
            } else if (baseIndex + i == iconNum) {
                mIconArray[i]->appearMiiDummy();
                continue;
            } else if (baseIndex + i > iconNum) {
                mIconArray[i]->kill();
                continue;
            }

            FileSelectIconID iconID;
            mHost->getIconID(&iconID, baseIndex + i);
            mIconArray[i]->appear(iconID);
        }
    }

    inline void setPointedMiiName(MiiSelect* pHost, s32 index) {
        wchar_t name[RFL_NAME_LEN + 1];
        FileSelectFunc::copyMiiName(reinterpret_cast< u16* >(name), pHost->makeIconID(index));
        MR::setTextBoxMessageRecursive(pHost, "TxtName", name);
    }

    void Page::movement() {
        bool isPointingAny = false;

        for (s32 i = 0; i < ARRAY_SIZE(mIconArray); i++) {
            if (!MR::isDead(mIconArray[i])) {
                mIconArray[i]->movement();

                if (mIconArray[i]->isMiiDummy()) {
                    if (mIconArray[i]->isSelected()) {
                        mHost->onSelectDummy();
                        mIconArray[i]->invalidate();
                    }
                } else if (mIconArray[i]->isSelected()) {
                    mHost->onSelect(mBaseIndex + i, mIconArray[i]->getTexMap());
                    mIconArray[i]->invalidate();
                } else if (mIconArray[i]->isPointing()) {
                    setPointedMiiName(mHost, mBaseIndex + i);

                    isPointingAny = true;
                }
            }
        }

        if (!isPointingAny) {
            MR::setTextBoxMessageRecursive(mHost, "TxtName", L"");
        }
    }

    void Page::calcAnim() {
        s32 v = 9;

        if (_20) {
            v = 1;
        }

        for (s32 i = 0; i < ARRAY_SIZE(mIconArray); i++) {
            char paneName[128];

            snprintf(paneName, sizeof(paneName), "Mii%02d", v + i);
            MR::setLayoutPosAtPaneTrans(mIconArray[i], mHost, paneName);
            mIconArray[i]->calcAnim();
        }
    }

    void Page::invalidateAllIcon() {
        for (int i = 0; i < ARRAY_SIZE(mIconArray); i++) {
            mIconArray[i]->invalidate();
        }
    }

    void Page::validateAllIcon() {
        for (int i = 0; i < ARRAY_SIZE(mIconArray); i++) {
            mIconArray[i]->validate();
        }
    }

    void Page::prohibitIcon(const FileSelectIconID& rIconID) {
        for (int i = 0; i < ARRAY_SIZE(mIconArray); i++) {
            if (MR::isDead(mIconArray[i])) {
                continue;
            }

            if (mIconArray[i]->isMiiDummy()) {
                continue;
            }

            FileSelectIconID iconID = FileSelectIconID();

            mIconArray[i]->copyIconID(&iconID);

            if (iconID != rIconID) {
                continue;
            }

            mIconArray[i]->prohibit();
        }
    }
}  // namespace MiiSelectSub

s32 MiiSelect::getIconNum() {
    return mMiiNum + mFavoriteMiiNum + mFellowIconNum;
}
