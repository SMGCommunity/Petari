#include "Game/LiveActor/AllLiveActorGroup.hpp"
#include "Game/NameObj/NameObjExecuteHolder.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Kamek.hpp"
#include "Macros.hpp"

namespace {
    static void listObj(const JMapInfoIter& rIter) {
        MR::initLiveActorSystemInfo(rIter);

        register NameObj* pObj;
        GET_REGISTER(pObj, r28);

        NameObjExecuteHolder* pExecuteHolder = MR::getSceneObj<NameObjExecuteHolder>(SceneObj_NameObjExecuteHolder);

        const char* pIterName;
        if (!MR::getObjectName(&pIterName, rIter)) {
            pIterName = "";
        }

        bool isLiveActor = false;

        NameObjExecuteInfo* pInfo = pExecuteHolder->getConnectToSceneInfo(pObj);
        if (pInfo != nullptr) {
            isLiveActor = pInfo->mDrawBufferType != -1;
        }
    
        OSReport("[%d] IterName=%s Name=%s Flag=%04X LiveActor=%d\n", pObj->mExecutorIdx, pObj->mName, pIterName, pObj->mFlag, isLiveActor);
    }
}

// kmCall(0x80342A44, listObj);