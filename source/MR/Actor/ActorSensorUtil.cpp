#include "MR/actor/ActorSensorUtil.h"

namespace MR
{
    bool isMsgPlayerSpinAttack(u32 msg)
    {
        return !(msg - 1);
    }
};