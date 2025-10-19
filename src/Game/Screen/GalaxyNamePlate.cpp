#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/GalaxyNamePlate.hpp"
#include "Game/Screen/GalaxyNamePlateDrawer.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    const s32 cAppearReadyFrame = 40;
};

namespace NrvGalaxyNamePlate {
    NEW_NERVE(GalaxyNamePlateNrvAppearReady, GalaxyNamePlate, AppearReady);
    NEW_NERVE(GalaxyNamePlateNrvAppear, GalaxyNamePlate, Appear);
    NEW_NERVE(GalaxyNamePlateNrvWait, GalaxyNamePlate, Wait);
};

GalaxyNamePlate::GalaxyNamePlate(const char* pGalaxyName, bool param2) :
    LayoutActor("ギャラクシー名プレート", true),
    mGalaxyName(pGalaxyName),
    _24(false),
    _25(false),
    mShowBalloonNozzle(true),
    mDrawerEntry(nullptr),
    _2C(0),
    _30(true)
{
    initLayoutManager("GalaxyNamePlate", 3);

    if (param2) {
        MR::connectToSceneLayoutMovementCalcAnim(this);

        mDrawerEntry = MR::registerToGalaxyNamePlateDrawer(this);
    }

    initEffectKeeper(0, nullptr, nullptr);
    MR::startAnim(this, "OneLine", 2);
}

void GalaxyNamePlate::init(const JMapInfoIter& rIter) {
    initNerve(&NrvGalaxyNamePlate::GalaxyNamePlateNrvAppear::sInstance);
}

void GalaxyNamePlate::kill() {
    MR::forceDeleteEffectAll(this);
    LayoutActor::kill();
}

void GalaxyNamePlate::showUnknown(bool a1) {
    show(MR::getGameMessageDirect("GalaxyNameShort_Unkonwn"), 0, true, a1);
}

void GalaxyNamePlate::showNew(bool a1) {
    show(MR::getGameMessageDirect("GalaxyNameShort_New"), 1, true, a1);
}

void GalaxyNamePlate::showOpen(bool a1) {
    show(MR::getGalaxyNameShortOnCurrentLanguage(mGalaxyName), 2, true, a1);
}

void GalaxyNamePlate::show(const wchar_t *pName, bool a2) {
    show(pName, 2, a2, true);
}

void GalaxyNamePlate::setShowBalloonNozzle(bool showBalloonNozzle) {
    mShowBalloonNozzle = showBalloonNozzle;
}

void GalaxyNamePlate::control() {
    _24 = false;
}

void GalaxyNamePlate::exeAppearReady() {
    if (MR::isFirstStep(this)) {
        MR::hideLayout(this);
    }

    if (!_24) {
        kill();
    }
    else {
        MR::setNerveAtStep(this, &NrvGalaxyNamePlate::GalaxyNamePlateNrvAppear::sInstance, cAppearReadyFrame);
    }
}

void GalaxyNamePlate::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Appear", 0);
        MR::showLayout(this);
    }

    if (!_24) {
        kill();
    }
    else {
        MR::setNerveAtAnimStopped(this, &NrvGalaxyNamePlate::GalaxyNamePlateNrvWait::sInstance, 0);
    }
}

void GalaxyNamePlate::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Wait", 0);

        if (_2C == 1) {
            MR::emitEffect(this, "GalaxyNamePlateNew");

            if (_30) {
                MR::startSystemSE("SE_SY_ASTRO_GALAXY_NEW_EFF", -1, -1);
            }
        }

        _30 = false;
    }

    if (!_24) {
        kill();
    }
}
