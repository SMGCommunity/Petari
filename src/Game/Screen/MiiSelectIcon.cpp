#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/FileSelectIconID.hpp"
#include "Game/NPC/MiiFaceIcon.hpp"
#include "Game/NPC/MiiFaceRecipe.hpp"
#include "Game/Screen/ButtonPaneController.hpp"
#include "Game/Screen/MiiSelectIcon.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <nw4r/lyt/texMap.h>

namespace {
    static const f32 sCharacterTexFrame[] = {
        0.0f,
        4.0f,
        3.0f,
        2.0f,
        1.0f,
    };

    NEW_NERVE(MiiSelectIconNrvCreate, MiiSelectIcon, Create);
    NEW_NERVE(MiiSelectIconNrvWait, MiiSelectIcon, Wait);
    NEW_NERVE(MiiSelectIconNrvSelected, MiiSelectIcon, Selected);
    NEW_NERVE(MiiSelectIconNrvDisappear, MiiSelectIcon, Disappear);
    NEW_NERVE(MiiSelectIconNrvInvalid, MiiSelectIcon, Invalid);
}; // namespace

MiiSelectIcon::MiiSelectIcon(int param1, int param2, int param3, const char* pName)
    : LayoutActor(pName, true),
      _20(nullptr),
      _24(nullptr),
      _28(nullptr),
      mIcon(nullptr),
      mMiiTexMap(nullptr),
      mFellowTexMap(nullptr),
      mIconID(new FileSelectIconID()),
      mIsMiiDummy(false),
      _3D(true) {
    initLayoutManager("MiiIcon", 1);
    MR::createAndAddPaneCtrl(this, "MarioIcon", 3);
    MR::createAndAddPaneCtrl(this, "MiiIcon", 3);
    MR::invalidateParentAnim(this);
    createButton();
    createFaceImageObj();
    initNerve(&MiiSelectIconNrvCreate::sInstance);

    if (param1 < 0) {
        param1 = 14;
    }

    if (param2 < 0) {
        param2 = 13;
    }

    if (param3 < 0) {
        param3 = 61;
    }

    MR::connectToScene(this, param1, param2, -1, param3);
}

void MiiSelectIcon::appear(const FileSelectIconID& rIconID) {
    if (MR::isDead(this)) {
        appear();
    }

    mIconID->set(rIconID);

    if (rIconID.isFellow()) {
        MR::showLayout(this);
        MR::startAnim(this, "Character", 0);
        MR::setAnimFrameAndStop(this, sCharacterTexFrame[rIconID.getFellowID()], 0);
        MR::startPaneAnim(this, "MarioIcon", "Character", 1);
        MR::setPaneAnimFrameAndStop(this, "MarioIcon", sCharacterTexFrame[rIconID.getFellowID()], 1);

        _20 = _24;

        MR::hidePane(this, "MiiIcon");
        MR::showPane(this, "MarioIcon");
        setNerve(&MiiSelectIconNrvWait::sInstance);
    } else if (rIconID.isMii()) {
        MR::hideLayout(this);
        mIcon->setIndex(rIconID.getMiiIndex());

        nw4r::lyt::TexMap* pTexMap = mMiiTexMap;

        MR::replacePaneTexture(this, "ShaMii", pTexMap, 0);
        MR::replacePaneTexture(this, "PicMii", pTexMap, 0);
        MR::replacePaneTexture(this, "PicMiiShine", pTexMap, 0);

        _20 = _28;

        MR::hidePane(this, "MarioIcon");
        MR::showPane(this, "MiiIcon");
        setNerve(&MiiSelectIconNrvCreate::sInstance);
    }

    mIsMiiDummy = false;

    MR::startPaneAnim(this, "MarioIcon", "Hide", 2);
    MR::setPaneAnimFrameAndStop(this, "MarioIcon", 0.0f, 2);
    MR::startPaneAnim(this, "MiiIcon", "Hide", 2);
    MR::setPaneAnimFrameAndStop(this, "MiiIcon", 0.0f, 2);
}

void MiiSelectIcon::appearMiiDummy() {
    if (MR::isDead(this)) {
        appear();
    }

    MR::showLayout(this);
    MR::startAnim(this, "Character", 0);
    MR::setAnimFrameAndStop(this, 5.0f, 0);
    MR::startPaneAnim(this, "MarioIcon", "Character", 1);
    MR::setPaneAnimFrameAndStop(this, "MarioIcon", 5.0f, 1);
    _20 = _24;
    MR::hidePane(this, "MiiIcon");
    MR::showPane(this, "MarioIcon");
    setNerve(&MiiSelectIconNrvWait::sInstance);
    mIsMiiDummy = true;
    MR::startPaneAnim(this, "MarioIcon", "Hide", 2);
    MR::setPaneAnimFrameAndStop(this, "MarioIcon", 0.0f, 2);
    MR::startPaneAnim(this, "MiiIcon", "Hide", 2);
    MR::setPaneAnimFrameAndStop(this, "MiiIcon", 0.0f, 2);
}

void MiiSelectIcon::validate() {
    if (isNerve(&MiiSelectIconNrvSelected::sInstance) || isNerve(&MiiSelectIconNrvInvalid::sInstance)) {
        _24->forceToWait();
        _28->forceToWait();
        setNerve(&MiiSelectIconNrvWait::sInstance);
    }

    _3D = true;
}

void MiiSelectIcon::invalidate() {
    if (isNerve(&MiiSelectIconNrvSelected::sInstance)) {
        setNerve(&MiiSelectIconNrvInvalid::sInstance);
    }

    _3D = false;
}

void MiiSelectIcon::prohibit() {
    invalidate();
    MR::startPaneAnim(this, "MarioIcon", "Hide", 2);
    MR::setPaneAnimFrameAndStop(this, "MarioIcon", 1.0f, 2);
    MR::startPaneAnim(this, "MiiIcon", "Hide", 2);
    MR::setPaneAnimFrameAndStop(this, "MiiIcon", 1.0f, 2);
}

bool MiiSelectIcon::isSelected() const {
    return isNerve(&MiiSelectIconNrvSelected::sInstance);
}

bool MiiSelectIcon::isMiiDummy() const {
    return mIsMiiDummy;
}

void MiiSelectIcon::copyIconID(FileSelectIconID* pIconID) {
    pIconID->set(*mIconID);
}

nw4r::lyt::TexMap* MiiSelectIcon::getTexMap() {
    if (mIconID->isFellow()) {
        GXTexObj texObj;

        MR::getLytTexMap(this, "PicMario", 0)->Get(&texObj);
        mFellowTexMap->Set(texObj);

        return mFellowTexMap;
    }

    return mMiiTexMap;
}

bool MiiSelectIcon::isPointing() const {
    return _20->isPointing();
}

void MiiSelectIcon::exeCreate() {
    if (MR::isFirstStep(this)) {
        mIcon->requestToMakeIcon();
    }

    if (mIcon->mIsCreated) {
        setNerve(&MiiSelectIconNrvWait::sInstance);
    }
}

void MiiSelectIcon::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::showLayout(this);
        _20->appear();
    }

    if (_3D) {
        _20->trySelect();
    }

    if (_20->isPointingTrigger()) {
        MR::startSystemSE("SE_SY_BUTTON_CURSOR_ON", -1, -1);
    }

    if (_20->mIsSelected) {
        setNerve(&MiiSelectIconNrvSelected::sInstance);
    }
}

void MiiSelectIcon::exeSelected() {
}

void MiiSelectIcon::exeDisappear() {
    if (_20->isHidden()) {
        if (_20 == _24) {
            _28->forceToHide();
        } else {
            _24->forceToHide();
        }

        kill();
    }
}

void MiiSelectIcon::exeInvalid() {
}

void MiiSelectIcon::appear() {
    LayoutActor::appear();
}

void MiiSelectIcon::control() {
    _24->update();
    _28->update();
}

void MiiSelectIcon::createButton() {
    _24 = new ButtonPaneController(this, "MarioIcon", "PicMario", 0, true);
    _24->_22 = false;

    _28 = new ButtonPaneController(this, "MiiIcon", "PicMii", 0, true);
    _28->_22 = false;
}

void MiiSelectIcon::createFaceImageObj() {
    nw4r::lyt::TexMap* pTexMap;

    mIcon = new MiiFaceIcon(
        128,
        128,
        MiiFaceRecipe(RFLDataSource_Official, 0, RFLResolution_256, RFLExpFlag_Normal | RFLExpFlag_Blink),
        "Miiアイコン");
    mIcon->initWithoutIter();

    GXTexObj texObj;
    MR::getLytTexMap(this, "PicMario", 0)->Get(&texObj);

    // TODO: Requires the corresponding header-defined TexMap constructor.
    // mFellowTexMap = new nw4r::lyt::TexMap(texObj);
    mMiiTexMap = MR::createLytTexMap(reinterpret_cast< ResTIMG* >(mIcon->mImageBuffer));
    pTexMap = mMiiTexMap;

    MR::replacePaneTexture(this, "ShaMii", pTexMap, 0);
    MR::replacePaneTexture(this, "PicMii", pTexMap, 0);
    MR::replacePaneTexture(this, "PicMiiShine", pTexMap, 0);
}
