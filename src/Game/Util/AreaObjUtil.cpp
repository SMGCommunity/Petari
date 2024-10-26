#include "Game/Util/AreaObjUtil.hpp"
#include "Game/AreaObj/AreaObj.hpp"
#include "Game/AreaObj/AreaObjContainer.hpp"
#include "Game/Util/PlayerUtil.hpp"

namespace MR {

    inline AreaObj* getAreaIn(const char *pName, const TVec3f &rPos) {
        return getAreaObjContainer()->getAreaObj(pName, rPos);
    }

    AreaObjMgr* getAreaObjManager(const char *pMgrName) {
        return MR::getAreaObjContainer()->getManager(pMgrName);
    }

    AreaObj* getAreaObj(const char *pAreaName, const TVec3f &rVec) {
        return MR::getAreaObjContainer()->getAreaObj(pAreaName, rVec);
    }

    bool isInAreaObj(const char *pAreaName, const TVec3f &rVec) {
        return MR::getAreaObjContainer()->getAreaObj(pAreaName, rVec);
    }

    s32 getAreaObjArg(const AreaObj *pObj, s32 which) {
        switch (which) {
            case 0:
                return pObj->mObjArg0;
            case 1:
                return pObj->mObjArg1;
            case 2:
                return pObj->mObjArg2;
            case 3:
                return pObj->mObjArg3;
            case 4:
                return pObj->mObjArg4;
            case 5:
                return pObj->mObjArg5;
            case 6:
                return pObj->mObjArg6;
            case 7:
                return pObj->mObjArg7;
            default:
                return -1;
        }
    }

    AreaObj* getCurrentAstroOverlookAreaObj() {
        return getAreaIn("AstroOverlookArea", *MR::getPlayerPos());
    }
};
