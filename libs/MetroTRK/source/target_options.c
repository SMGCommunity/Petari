#include "TRK_Types.h"

static u8 bUseSerialIO;

/* this is about as close as you are going to get */
u8 GetUseSerialIO(void)
{
    return bUseSerialIO;
}

void SetUseSerialIO(u8 result) {
    bUseSerialIO = result;
    return;
}
