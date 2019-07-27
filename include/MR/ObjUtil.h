#ifndef OBJUTIL_H
#define OBJUTIL_H

#include "Actor/NameObj/NameObj.h"
#include "System/Resource/ResourceHolder.h"

namespace MR
{
    void connectToScene(NameObj *, s32, s32, s32, s32);

    void requestMovementOn(NameObj *);
    void requestMovementOff(NameObj *);
    void notifyRequestNameObjMovementOnOff();

    ResourceHolder* createAndAddResourceHolder(const char *);
    void* loadResourceFromArc(const char *, const char *);
};

#endif // OBJUTIL_H