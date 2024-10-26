#include "Game/AreaObj/AstroChangeStageCube.hpp"
#include "Game/Util.hpp"

AstroChangeStageCube::AstroChangeStageCube(int a1, const char *pName) : AreaObj(a1, pName) {
    _3C = 1;
    _44 = 0;
}

void AstroChangeStageCube::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);
    MR::connectToSceneAreaObj(this);

    switch(mObjArg0) {
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

bool AstroChangeStageCube::isInVolume(const TVec3f &rVec) const {
    if (MR::isPlayerInBind() || MR::isDemoActive()) {
        return false;
    }

    if (isValidSwitchA() && isOnSwitchA()) {
        return false;
    }

    switch(mObjArg0) {
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

    return AreaObj::isInVolume(rVec);
}

void AstroChangeStageCube::movement() {
    bool isValid = false;

    if (mValid && _15 && mAwake) {
        isValid = true;
    }

    if (isValid) {
        TVec3f* playerPos = MR::getPlayerPos();

        if (!isInVolume(*playerPos)) {
            return;
        }

        mValid = false;
        MR::stopStageBGM(5);
        MR::stopSubBGM(5);

        switch(mObjArg0) {
            case 3:
            case 1:
            case 5:
                MR::startSystemSE("SE_DM_ASTRO_WIPE_IN", -1, -1);
                break;
            case 0:
                MR::startSystemSE("SE_DM_ASTRO_WIPE_OUT", -1, -1);
                break;
        }

        _44 = 0;
    }
    else {
        if (_44 < 5) {
            _44++;
        }
        else {
            MR::closeSystemWipeCircleWithCaptureScreen(0x6E);
            MR::setWipeCircleCenterPos(*MR::getPlayerCenterPos());

            switch(mObjArg0) {
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
                    }
                    else {
                        MR::requestChangeStageInGameMoving("HeavensDoorGalaxy", 2, JMapIdInfo(_3C, 0));   
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
}