#include "Game/NPC/TalkSupportPlayerWatcher.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"

TalkSupportPlayerWatcher::TalkSupportPlayerWatcher() : _00(0), _04(0) {}

void TalkSupportPlayerWatcher::update() {
    if (isEnableTalkPlayerSpeed()) {
        _00++;
        _04 = 0;
    } else {
        _04++;
        _00 = 0;
    }
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

bool TalkSupportPlayerWatcher::isEnableTalkPlayerStateNormal() const {
    if (MR::isPlayerInWaterMode()) {
        return false;
    }

    if (MR::isOffPlayerControl()) {
        return false;
    }

    if (!MR::isNearZero(MR::getSubPadStickX(0), 0.001f) || !MR::isNearZero(MR::getSubPadStickY(0), 0.001f)) {
        return false;
    }

    return isEnableTalkPlayerStateEvent();
}

bool TalkSupportPlayerWatcher::isEnableTalkPlayerSpeed() const {
    return PSVECMag(MR::getPlayerVelocity()) < 4.0f;
}
