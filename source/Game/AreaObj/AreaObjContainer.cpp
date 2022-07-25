#include "Game/AreaObj/AreaObjContainer.h"
#include "Game/AreaObj/AreaObj.h"
#include "Game/Scene/SceneObjHolder.h"
#include <string.h>

AreaObjContainer::AreaObjContainer(const char *pName) : NameObj(pName) {
    mNumManagers = 0;
}

void AreaObjContainer::init(const JMapInfoIter &rIter) {
    for (u32 i = 0; i < 0x43; i++) {
        AreaObjEntry* entry = &cCreateTable[i];
        AreaObjMgr* mgr = entry->mFuncPtr(entry->_4, entry->mName);
        JMapInfoIter iter;
        iter.mInfo = NULL;
        iter._4 = -1;
        mgr->init(iter);
        s32 num = mNumManagers;
        mNumManagers++;
        mManagerArray[num] = mgr;
    }
}

AreaObjMgr* AreaObjContainer::getManager(const char *pName) const {
    AreaObjMgr** first = (AreaObjMgr**)getFirstMgr();
    while ((AreaObjMgr**)getFirstMgr() != (AreaObjMgr**)getLastMgr()) {
        const char* str = strstr(pName, (*first)->mName);
        bool flag = false;

        if (str) {
            if (str == pName) {
                flag = true;
            }
        }

        if (flag) {
            break;
        }

        first++;
    }

    return *first;
}

AreaObj* AreaObjContainer::getAreaObj(const char *pName, const TVec3f &rVec) const {
    return getManager(pName)->find_in(rVec);
}

namespace MR {
    AreaObjContainer* getAreaObjContainer() {
        return reinterpret_cast<AreaObjContainer*>(MR::getSceneObjHolder()->getObj(SceneObj_AreaObjContainer));
    }
};

AreaObjContainer::~AreaObjContainer() {

}
