#ifndef UTIL_H
#define UTIL_H

#include "actor/nameobj/NameObj.h"
#include "types.h"

namespace MR
{
    void requestMovementOn(NameObj *);
    void requestMovementOff(NameObj *);
    void notifyRequestNameObjMovementOnOff();

    s32 getHashCode(const char *);
};

#endif // UTIL_H