#include "Scene/SceneObjHolder.h"
#include "NameObj/NameObjGroup.h"
#include "NameObj/NameObjExecuteHolder.h"
#include "LiveActor/AllLiveActorGroup.h"
#include "smg.h"

SceneObjHolder::SceneObjHolder()
{
    s32 curIdx = 0;

    while(curIdx != 0x7B)
    {
        mObjs[curIdx++] = nullptr;
    }
}

NameObj* SceneObjHolder::create(int type)
{
    if (mObjs[type] != 0)
    {
        return mObjs[type];
    }

    NameObj* obj = newEachObj(type);
    obj->initWithoutIter();
    mObjs[type] = obj;
    return obj;
}

NameObj* SceneObjHolder::getObj(int type) const
{
    return mObjs[type];
}

bool SceneObjHolder::isExist(int type) const
{
    return mObjs[type];
}

NameObj* SceneObjHolder::newEachObj(int type)
{
    NameObj* obj;

    switch (type)
    {
        case 24:
            obj = new NameObjGroup("IgnorePauseNameObj", 0x10);
            break;
        case 48:
            obj = new AllLiveActorGroup();
            break;
        default:
            obj = 0;
            break;
    }

    return obj;
}