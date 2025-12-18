#include "Game/Screen/StarPointerGuidance.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

namespace NrvStarPointerGuidance {
    NEW_NERVE(HostTypeNrvAppear1P, StarPointerGuidance, Appear1P);
    NEW_NERVE(HostTypeNrvWait1P, StarPointerGuidance, Wait1P);
    NEW_NERVE(HostTypeNrvEnd1P, StarPointerGuidance, End1P);
    NEW_NERVE(HostTypeNrvAppearBlueStarGuide, StarPointerGuidance, AppearBlueStarGuide);
    NEW_NERVE(HostTypeNrvWaitBlueStarGuide, StarPointerGuidance, WaitBlueStarGuide);
    NEW_NERVE(HostTypeNrvEndBlueStarGuide, StarPointerGuidance, EndBlueStarGuide);
    NEW_NERVE(HostTypeNrvAppear2P, StarPointerGuidance, Appear2P);
    NEW_NERVE(HostTypeNrvWait2P, StarPointerGuidance, Wait2P);
    NEW_NERVE(HostTypeNrvEnd2P, StarPointerGuidance, End2P);
    NEW_NERVE(HostTypeNrvEndWait, StarPointerGuidance, EndWait);
};  // namespace NrvStarPointerGuidance

StarPointerGuidance::StarPointerGuidance(const char* pName)
    : LayoutActor(pName, true), mSpineFrame1P(nullptr), mSpineGuidance(nullptr), mSpineFrame2P(nullptr), _2C(0), _30(0), _34(0), _38(nullptr), _3C(nullptr), _40(false), _41(false),
      _42(true), _43(false), _44(1), _48(1), _4C(1) {}

void StarPointerGuidance::init(const JMapInfoIter& rIter) {
    initLayoutManager("PointerGuidance", 1);
    MR::createAndAddPaneCtrl(this, "NPointerGuide", 1);
    MR::createAndAddPaneCtrl(this, "PointerFrame1", 1);
    MR::createAndAddPaneCtrl(this, "PointerFrame2", 1);

    mSpineFrame1P = new Spine(this, &NrvStarPointerGuidance::HostTypeNrvEndWait::sInstance);
    mSpineGuidance = new Spine(this, &NrvStarPointerGuidance::HostTypeNrvEndWait::sInstance);
    mSpineFrame2P = new Spine(this, &NrvStarPointerGuidance::HostTypeNrvEndWait::sInstance);

    appear();
    MR::hidePaneRecursive(this, "NPointerGuide");
    MR::hidePaneRecursive(this, "PointerFrame1");
    MR::hidePaneRecursive(this, "PointerFrame2");

    _48 = MR::getTextLineNumMaxRecursive(this, "PointerFrame1");
    _4C = MR::getTextLineNumMaxRecursive(this, "PointerFrame2");
}

void StarPointerGuidance::control() {
    mSpineFrame1P->update();
    mSpineGuidance->update();
    mSpineFrame2P->update();

    if (!_42) {
        _40 = false;
        _41 = false;
    }

    checkRequest2P();
    checkRequest1P();
}

void StarPointerGuidance::checkRequest2P() {
    if (!_41) {
        if (mSpineFrame2P->getCurrentNerve() == &NrvStarPointerGuidance::HostTypeNrvWait2P::sInstance) {
            mSpineFrame2P->setNerve(&NrvStarPointerGuidance::HostTypeNrvEnd2P::sInstance);
        }

        _34 = 0;
    } else {
        _34++;

        if (!MR::isStarPointerInScreen(WPAD_CHAN1)) {
            if (mSpineFrame2P->getCurrentNerve() == &NrvStarPointerGuidance::HostTypeNrvEnd2P::sInstance ||
                mSpineFrame2P->getCurrentNerve() == &NrvStarPointerGuidance::HostTypeNrvEndWait::sInstance) {
                mSpineFrame2P->setNerve(&NrvStarPointerGuidance::HostTypeNrvAppear2P::sInstance);

                if (_34 == 1) {
                    MR::startCSSound2P("CS_CAN_PLAY_2P", nullptr);
                } else {
                    MR::startCSSound2P("CS_OUT_DISPLAY_2P", nullptr);
                }
            }
        } else if (mSpineFrame2P->getCurrentNerve() == &NrvStarPointerGuidance::HostTypeNrvWait2P::sInstance) {
            mSpineFrame2P->setNerve(&NrvStarPointerGuidance::HostTypeNrvEnd2P::sInstance);
            MR::startCSSound2P("CS_IN_DISPLAY_2P", nullptr);
        }

        _41 = false;
    }
}

// StarPointerGuidance::checkRequest1P

void StarPointerGuidance::exeAppear1P() {
    exeAppearCore(mSpineFrame1P, "PointerFrame1", _48, &NrvStarPointerGuidance::HostTypeNrvWait1P::sInstance);
}

void StarPointerGuidance::exeWait1P() {
    exeWaitCore(mSpineFrame1P, "PointerFrame1", _48);
}

void StarPointerGuidance::exeEnd1P() {
    exeEndCore(mSpineFrame1P, "PointerFrame1", _48);
}

void StarPointerGuidance::exeAppearBlueStarGuide() {
    exeAppearCore(mSpineGuidance, "NPointerGuide", _44, &NrvStarPointerGuidance::HostTypeNrvWaitBlueStarGuide::sInstance);
}

void StarPointerGuidance::exeWaitBlueStarGuide() {
    exeWaitCore(mSpineGuidance, "NPointerGuide", _44);
}

void StarPointerGuidance::exeEndBlueStarGuide() {
    exeEndCore(mSpineGuidance, "NPointerGuide", _44);
}

void StarPointerGuidance::exeAppear2P() {
    exeAppearCore(mSpineFrame2P, "PointerFrame2", _4C, &NrvStarPointerGuidance::HostTypeNrvWait2P::sInstance);
}

void StarPointerGuidance::exeWait2P() {
    exeWaitCore(mSpineFrame2P, "PointerFrame2", _4C);
}

void StarPointerGuidance::exeEnd2P() {
    exeEndCore(mSpineFrame2P, "PointerFrame2", _4C);
}

void StarPointerGuidance::exeEndWait() {}

s32 StarPointerGuidance::exeAppearCore(Spine* pSpine, const char* pPaneName, u32 param3, const Nerve* pNerve) {
    if (pSpine->mStep == 0) {
        MR::showPaneRecursive(this, pPaneName);

        if (param3 >= 2) {
            MR::startPaneAnim(this, pPaneName, "AppearTwoLine", 0);
        } else {
            MR::startPaneAnim(this, pPaneName, "AppearOneLine", 0);
        }

        return 1;
    }

    if (MR::isPaneAnimStopped(this, pPaneName, 0)) {
        pSpine->setNerve(pNerve);

        return 2;
    }

    return 0;
}

s32 StarPointerGuidance::exeWaitCore(Spine* pSpine, const char* pPaneName, u32 param3) {
    if (pSpine->mStep == 0) {
        _43 = false;

        MR::startPaneAnim(this, pPaneName, "Wait", 0);

        return 1;
    }

    if (_50 && pSpine == mSpineGuidance && mSpineGuidance->mStep == 240) {
        _43 = true;

        if (param3 >= 2) {
            MR::startPaneAnim(this, pPaneName, "EndTwoLine", 0);
        } else {
            MR::startPaneAnim(this, pPaneName, "EndOneLine", 0);
        }
    }

    return 0;
}

s32 StarPointerGuidance::exeEndCore(Spine* pSpine, const char* pPaneName, u32 param3) {
    if (pSpine->mStep == 0) {
        if (pSpine == mSpineGuidance && _43) {
            return 1;
        }

        if (param3 >= 2) {
            MR::startPaneAnim(this, pPaneName, "EndTwoLine", 0);
        } else {
            MR::startPaneAnim(this, pPaneName, "EndOneLine", 0);
        }

        return 1;
    }

    if (MR::isPaneAnimStopped(this, pPaneName, 0)) {
        MR::hidePaneRecursive(this, pPaneName);
        pSpine->setNerve(&NrvStarPointerGuidance::HostTypeNrvEndWait::sInstance);

        return 2;
    }

    return 0;
}

bool StarPointerGuidance::request1PGuidance(const char* pMessageId, bool param2) {
    if (!MR::isConnectedWPad(WPAD_CHAN0) || !MR::isStarPointerValid(WPAD_CHAN0) && !MR::isStarPointerModeMarioLauncher()) {
        return false;
    }

    _38 = pMessageId != nullptr ? MR::getLayoutMessageDirect(pMessageId) : nullptr;
    _40 = true;
    _50 = param2;

    return true;
}

void StarPointerGuidance::tryResetTimeout() {
    if (!_50) {
        return;
    }

    if (!_43) {
        return;
    }

    mSpineGuidance->setNerve(&NrvStarPointerGuidance::HostTypeNrvWaitBlueStarGuide::sInstance);
}

bool StarPointerGuidance::isExistGuidanceOrFrame() {
    return isExistGuidance() || isExistFrame1P() || isExistFrame2P();
}

bool StarPointerGuidance::isExistGuidance() {
    bool isNerve = mSpineGuidance->getCurrentNerve() == &NrvStarPointerGuidance::HostTypeNrvEndWait::sInstance;

    if (isNerve) {
        return false;
    }

    return !_43;
}

bool StarPointerGuidance::isExistFrame1P() {
    bool isNerve = mSpineFrame1P->getCurrentNerve() == &NrvStarPointerGuidance::HostTypeNrvEndWait::sInstance;

    return !isNerve;
}

bool StarPointerGuidance::isExistFrame2P() {
    bool isNerve = mSpineFrame2P->getCurrentNerve() == &NrvStarPointerGuidance::HostTypeNrvEndWait::sInstance;

    return !isNerve;
}

void StarPointerGuidance::deactive() {
    _42 = false;

    MR::hidePaneRecursive(this, "PointerFrame1");
    MR::hidePaneRecursive(this, "NPointerGuide");
    MR::hidePaneRecursive(this, "PointerFrame2");
    mSpineFrame1P->setNerve(&NrvStarPointerGuidance::HostTypeNrvEndWait::sInstance);
    mSpineGuidance->setNerve(&NrvStarPointerGuidance::HostTypeNrvEndWait::sInstance);
    mSpineFrame2P->setNerve(&NrvStarPointerGuidance::HostTypeNrvEndWait::sInstance);

    _3C = nullptr;
    _38 = nullptr;
    _40 = false;
    _41 = false;
}
