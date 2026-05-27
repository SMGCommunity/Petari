#include "Game/System/FindingLuigiEventScheduler.hpp"
#include "Game/NWC24/LuigiMailDirector.hpp"
#include "Game/System/GalaxyMoveArgument.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/System/GameEventFlagTable.hpp"
#include "Game/System/GameSequenceFunction.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <cstdio>

namespace {
    u32 getLuigiEventState() {
        return GameDataFunction::getGameEventValue("LuigiEventState") & 0xFFFF;
    }
}  // namespace

FindingLuigiEventScheduler::FindingLuigiEventScheduler() : mLuigiLostStageName(nullptr), mLuigiLostStarID(-1), mLuigiMailDirector(nullptr) {
}

void FindingLuigiEventScheduler::initAfterResourceLoaded() {
    mLuigiMailDirector = new LuigiMailDirector();
    mLuigiMailDirector->initAfterResourceLoaded();
}

void FindingLuigiEventScheduler::update(const GalaxyMoveArgument& rMoveArg) {
    switch (rMoveArg.mMoveType) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        break;
    case 6:
        // On Normal Level Load
        syncWithGameEventFlag();
        mLuigiMailDirector->reset();
        break;
    case 7:
        // On Title Screen
        GameDataFunction::setGameEventValue("LuigiEventState", STATE_NULL);
        break;
    default:
        break;
    }
}

void FindingLuigiEventScheduler::updateOnStageResult(const char* pClearedStageName, s32 clearedStarID) {
    if (getLuigiEventState() == STATE_NULL && GameDataFunction::isOnGameEventFlag("SpecialStarLuigiRescued")) {
        // if luigi in null state but has been rescued from Ghostly Galaxy
        // force into rescued state by default
        GameDataFunction::setGameEventValue("LuigiEventState", STATE_RESCUED);
    } else {
        if (getLuigiEventState() == STATE_RESCUED) {
            if (!GameSequenceFunction::hasPowerStarYetAtResultSequence()) {
                // if new star collected while luigi is in rescued state
                if (GameDataFunction::calcCurrentPowerStarNum() > 108 || GameDataFunction::isOnGameEventFlag("LuigiTalkAfterRescued")) {
                    // if number of stars safeguard passed or eventflag open
                    // of speaking to luigi in garage after rescued from ghostly galaxy
                    // force luigi to lost state
                    mLuigiMailDirector->lost();
                    GameDataFunction::setGameEventValue("LuigiEventState", STATE_DISAPPEAR);
                    return;
                }
            }
        }

        if (getLuigiEventState() == STATE_HIDING && MR::isEqualString(mLuigiLostStageName, pClearedStageName) && mLuigiLostStarID == clearedStarID) {
            // if Luigi was hiding and just rescued him
            mLuigiMailDirector->found();
            if (calcPowerStarIndexLuigiHas() == 4) {
                // if got all luigi stars, set luigi to "completed" state
                GameDataFunction::setGameEventValue("LuigiEventState", STATE_END);
            } else {
                // set countdown for 5 stars
                setStateReturnAstroGalaxy(5);
            }
            return;
        }

        if (isState(0)) {
            // if in countdown state
            u32 countdown = getLuigiEventState();

            if (countdown != 0 && --countdown == 0) {
                // if just counted down to 0
                // set luigi to lost state
                mLuigiMailDirector->lost();
                GameDataFunction::setGameEventValue("LuigiEventState", STATE_DISAPPEAR);
            } else {
                // if countdown was already 0, or countdown not hit 0 yet,
                // try to return normally (w/ 108 star check), and count down
                setStateReturnAstroGalaxy(countdown);
            }
        }
    }
}

void FindingLuigiEventScheduler::setStateHiding() {
    GameDataFunction::setGameEventValue("LuigiEventState", STATE_HIDING);
    getHidingGalaxyNameAndStarId(&mLuigiLostStageName, &mLuigiLostStarID);
}

void FindingLuigiEventScheduler::getHidingGalaxyNameAndStarId(const char** pStageName, s32* pStarID) const {
    char eventName[] = "SpecialStarFindingLuigi1";

    for (s32 idx = 1; idx <= 3; idx++) {
        snprintf(eventName, sizeof(eventName), "SpecialStarFindingLuigi%1d", idx);
        if (!GameDataFunction::isOnGameEventFlag(eventName)) {
            // found event flag that is not completed
            GameEventFlagAccessor accessor = GameEventFlagTable::makeAccessor(eventName);
            *pStageName = accessor.getGalaxyName();
            *pStarID = accessor.getStarId();
            return;
        }
    }

    // all events completed
    *pStageName = nullptr;
    *pStarID = -1;
}

bool FindingLuigiEventScheduler::isActive() const {
    bool luigiIsActive = getLuigiEventState() != STATE_NULL;
    if (luigiIsActive) {
        // If luigi not in null state, return if luigi is not rescued
        luigiIsActive = getLuigiEventState() != STATE_RESCUED;
    }
    return luigiIsActive;
}

bool FindingLuigiEventScheduler::isDisappear() const {
    // return if luigi is in the "Disappear"/"Lost" state
    return getLuigiEventState() == STATE_DISAPPEAR;
}

bool FindingLuigiEventScheduler::isHiding() const {
    // return if luigi is in the "Hiding" state (after getting the letter)
    return getLuigiEventState() == STATE_HIDING;
}

bool FindingLuigiEventScheduler::isEnd() const {
    // return if all luigi is in state where all luigi stars have been completed
    return getLuigiEventState() == STATE_END;
}

void FindingLuigiEventScheduler::sendMail() {
    mLuigiMailDirector->sendMail();
}

void FindingLuigiEventScheduler::writeSendSize() {
    mLuigiMailDirector->writeSendSize();
}

void FindingLuigiEventScheduler::clearLostAndFoundCount() {
    if (mLuigiMailDirector != nullptr) {
        mLuigiMailDirector->reset();
    }
}

void FindingLuigiEventScheduler::syncWithGameEventFlag() {
    if (getLuigiEventState() == STATE_HIDING) {
        GameDataFunction::setGameEventValue("LuigiEventState", STATE_HIDING);
        getHidingGalaxyNameAndStarId(&mLuigiLostStageName, &mLuigiLostStarID);
    }
}

s32 FindingLuigiEventScheduler::calcPowerStarIndexLuigiHas() const {
    if (!GameDataFunction::isOnGameEventFlag("SpecialStarLuigiRescued")) {
        return 0;
    }

    char printBuffer[] = "SpecialStarFindingLuigi1";
    for (s32 idx = 1; idx <= 3; idx++) {
        snprintf(printBuffer, sizeof(printBuffer), "SpecialStarFindingLuigi%1d", idx);
        if (!GameDataFunction::isOnGameEventFlag(printBuffer)) {
            return idx;
        }
    }

    // all events completed
    return 4;
}

bool FindingLuigiEventScheduler::isState(u32 state) const {
    if (state == 0) {
        // check if in countdown state
        // luigiEventState = GameDataFunction::getGameEventValue("LuigiEventState");
        return getLuigiEventState() <= 5;
    }

    // check if state matches value exactly
    return state == getLuigiEventState();
}

void FindingLuigiEventScheduler::setStateReturnAstroGalaxy(int state) {
    if (GameDataFunction::calcCurrentPowerStarNum() > 108) {
        // safeguard if above 109 stars, force luigi to get lost
        mLuigiMailDirector->lost();
        GameDataFunction::setGameEventValue("LuigiEventState", STATE_DISAPPEAR);
    } else {
        // if before the 109 star safeguard, set countdown state normally
        GameDataFunction::setGameEventValue("LuigiEventState", state);
    }
}
