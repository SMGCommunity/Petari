#ifndef OBJUTIL_H
#define OBJUTIL_H

#include "Actor/NameObj/NameObj.h"

namespace MR
{
    void connectToScene(NameObj *, s32, s32, s32, s32);

    void requestMovementOn(NameObj *);
    void requestMovementOff(NameObj *);
    void notifyRequestNameObjMovementOnOff();

};

#endif // OBJUTIL_H