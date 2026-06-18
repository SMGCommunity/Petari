#include "Game/AreaObj/AstroChangeStageCube.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/JMapIdInfo.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SequenceUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    const s32 cChangeStageWaitFrame = 5;
    const s32 cHeavensDoorScenarioNo = 2;
    const s32 cWipeFrame = 110;
};  // namespace

AstroChangeStageCube::AstroChangeStageCube(int formType, const char* pName) : AreaObj(formType, pName), _3C(1), mWaitFrame(0) {
}

void AstroChangeStageCube::init(const JMapInfoIter& rIter) {
    AreaObj::init(rIter);
    MR::connectToSceneAreaObj(this);

    switch (mObjArg0) {
    case 0:
        _3C = MR::getCurrentScenarioNo();
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        _3C = mObjArg1;
        break;
    case 5:
        _3C = mObjArg1;
        break;
    }
}

bool AstroChangeStageCube::isInVolume(const TVec3f& rPos) const {
    if (MR::isPlayerInBind() || MR::isDemoActive()) {
        return false;
    }

    if (isValidSwitchA() && isOnSwitchA()) {
        return false;
    }

    switch (mObjArg0) {
    case 5:
        if (!MR::hasGrandStar(mObjArg2)) {
            return false;
        }
        break;
    case 1:
        if (!MR::isOnGameEventFlagChildsRoom()) {
            return false;
        }
        break;
    case 3:
        if (!MR::isOnGameEventFlagLibraryRoom()) {
            return false;
        }
        break;
    }

    return AreaObj::isInVolume(rPos);
}

AstroChangeStageCube::~AstroChangeStageCube() {
}

void AstroChangeStageCube::movement() {
    if (isValid()) {
        TVec3f* playerPos = MR::getPlayerPos();

        if (!isInVolume(*playerPos)) {
            return;
        }

        mIsValid = false;

        MR::stopStageBGM(::cChangeStageWaitFrame);
        MR::stopSubBGM(::cChangeStageWaitFrame);

        switch (mObjArg0) {
        case 0:
        case 2:
        case 4:
            MR::startSystemSE("SE_DM_ASTRO_WIPE_OUT");
            break;
        case 1:
        case 3:
        case 5:
        default:
            MR::startSystemSE("SE_DM_ASTRO_WIPE_IN");
            break;
        }

        mWaitFrame = 0;
    } else if (mWaitFrame < ::cChangeStageWaitFrame) {
        mWaitFrame++;
    } else {
        MR::closeSystemWipeCircleWithCaptureScreen(::cWipeFrame);
        MR::setWipeCircleCenterPos(*MR::getPlayerCenterPos());

        switch (mObjArg0) {
        case 0:
        case 4:
            MR::requestGoToAstroGalaxy(_3C);
            break;
        case 2:
            MR::requestChangeStageInGameMoving("AstroGalaxy", 1, JMapIdInfo(_3C, 0));
            break;
        case 1:
            if (MR::isOnGameEventFlagRosettaTalkAboutTicoInTower()) {
                MR::requestStartScenarioSelect("HeavensDoorGalaxy");
            } else {
                MR::requestChangeStageInGameMoving("HeavensDoorGalaxy", ::cHeavensDoorScenarioNo, JMapIdInfo(_3C, 0));
            }
            break;
        case 3:
            MR::requestChangeStageInGameMoving("LibraryRoom", 1, JMapIdInfo(_3C, 0));
            break;
        case 5:
            MR::requestGoToAstroDomeFromAstroGalaxy(mObjArg2, _3C);
            break;
        }
    }
}
