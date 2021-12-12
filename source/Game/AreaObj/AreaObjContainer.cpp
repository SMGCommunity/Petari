#include "Game/AreaObj/AreaObjContainer.h"
#include "Game/AreaObj/AreaObj.h"
#include "Game/Scene/SceneObjHolder.h"

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
