#include "Game/NPC/TalkSupportPlayerWatcher.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"

namespace {
    static const f32 sPlayerSpeedMin = 4.0f;
    // static const s32 sStepToTalkStart = _;
    // static const s32 sStepToTalkEnd = _;
    // static const s32 sStepToTalkRestart = _;
};  // namespace

TalkSupportPlayerWatcher::TalkSupportPlayerWatcher() : mEnableTalkCounter(), mDisableTalkCounter() {
}

void TalkSupportPlayerWatcher::update() {
    if (isEnableTalkPlayerSpeed()) {
        mEnableTalkCounter++;
        mDisableTalkCounter = 0;
    } else {
        mDisableTalkCounter++;
        mEnableTalkCounter = 0;
    }
}

bool TalkSupportPlayerWatcher::isEnableTalkPlayerStateNormal() const {
    if (MR::isPlayerInWaterMode()) {
        return false;
    }

    if (MR::isOffPlayerControl()) {
        return false;
    }

    if (!MR::isNearZero(MR::getSubPadStickX(WPAD_CHAN0)) || !MR::isNearZero(MR::getSubPadStickY(WPAD_CHAN0))) {
        return false;
    }

    return isEnableTalkPlayerStateEvent();
}

bool TalkSupportPlayerWatcher::isEnableTalkPlayerStateEvent() const {
    if (MR::isPlayerInBind()) {
        return false;
    }

    if (MR::isPlayerRefuseTalk()) {
        return false;
    }

    if (MR::isPlayerElementModeTeresa()) {
        return true;
    }

    if (MR::isPlayerInWaterMode() || MR::isInWater(MR::getPlayerDemoActor(), TVec3f(0.0f, 0.0f, 0.0f))) {
        return true;
    }

    return MR::isOnGroundPlayer();
}

bool TalkSupportPlayerWatcher::isEnableTalkPlayerSpeed() const {
    return MR::getPlayerVelocity()->length() < ::sPlayerSpeedMin;
}
