#include "MR/actor/ActorSensorUtil.h"
#include "defines.h"

namespace MR
{
    u8 isMsgPlayerSpinAttack(u32 msg)
    {
        return __cntlzw(msg - 1) >> 5;
    }
};