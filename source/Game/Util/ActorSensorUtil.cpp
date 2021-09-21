#include "Game/Util/ActorSensorUtil.h"

namespace MR {

    #ifdef NON_MATCHING
    bool isMsgPlayerHitAll(u32 msg) {
        if (msg - 1 <= 0xA && ((1 << msg - 1) & 0x6B1)) {
            return true;
        }
        
        return false;
    }
    #endif

    bool isMsgPlayerSpinAttack(u32 msg) {
        return !(msg != 1);
    }

    bool isMsgPlayerTrample(u32 msg) {
        return !(msg != 2);
    }

    bool isMsgPlayerHipDrop(u32 msg) {
        return!(msg != 3);
    }

    bool isMsgPlayerHipDropFloor(u32 msg) {
        return !(msg != 4);
    }

    bool isMsgPlayerUpperPunch(u32 msg) {
        return !(msg != 5);
    }
};
