#include "Game/NameObj/MovementOnOffGroupHolder.hpp"
#include "Game/Util.hpp"

MovementOnOffGroupHolder::MovementOnOffGroupHolder(const char *pName) : NameObj(pName) {
    mCount = 0;
}

NameObjGroup* MovementOnOffGroupHolder::joinToGroup(const char *pName, NameObj *pobj, u32 a3) {
    NameObjGroup* group = findGroupFromName(pName);

    if (!group) {
        group = createGroup(pName, a3);
    }

    group->registerObj(pobj);
    return group;
}

void MovementOnOffGroupHolder::onMovementGroup(const char *pGroupName) {
    NameObjGroup* group = findGroupFromName(pGroupName);

    for (s32 i = 0; i < group->mObjectCount; i++) {
        MR::requestMovementOn(group->mObjects[i]);
    }
}

NameObjGroup* MovementOnOffGroupHolder::createGroup(const char *pGroupName, u32 count) {
    NameObjGroup* group = new NameObjGroup(pGroupName, count);
    group->initWithoutIter();
    s32 c = mCount;
    mCount = c + 1;
    mGroups[c] = group;
    return group;
}

NameObjGroup* MovementOnOffGroupHolder::findGroupFromName(const char *pName) const {
    for (u32 i = 0; i < mCount; i++) {
        NameObjGroup* cur = mGroups[i];
        if (MR::isEqualString(cur->mName, pName)) {
            return mGroups[i];
        }
    }

    return 0;
}

MovementOnOffGroupHolder::~MovementOnOffGroupHolder() {

}