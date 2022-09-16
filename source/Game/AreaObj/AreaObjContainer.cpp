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

/* this function is nearly impossible to match without context behind the random stack storages */
AreaObjMgr* AreaObjContainer::getManager(const char *pName) const {
    const char * str;
    AreaObjMgr** first = (AreaObjMgr**)&mManagerArray[0];
    AreaObjMgr** last = (AreaObjMgr**)&mManagerArray[mNumManagers];
    
    while ((str && str == pName) == false) {
        if (++first == last)
            break;

        str = strstr(pName, (*first)->mName);   
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
