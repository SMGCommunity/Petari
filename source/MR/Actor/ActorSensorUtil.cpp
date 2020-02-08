#include "MR/actor/ActorSensorUtil.h"

extern int __cntlwz(int);

namespace MR
{
    u8 isMsgPlayerSpinAttack(u32 msg)
    {
        return __cntlzw(msg - 1) >> 5;
    }
};