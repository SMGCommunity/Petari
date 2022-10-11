#include "Game/NameObj/NameObjExecuteHolder.h"
#include "Game/NameObj/NameObjListExecutor.h"
#include "Game/LiveActor/LiveActor.h"
#include "Game/Scene/SceneObjHolder.h"
#include "Game/System/GameSystem.h" 
#include "Game/SingletonHolder.h"

NameObjExecuteInfo::NameObjExecuteInfo() {
    mExecutedObject = 0;
    _4 = 0;
    _5 = 0;
    _6 = -1;
    _7 = -1;
    _8 = -1;
    _9 = -1;
    _A = -1;
}

void NameObjExecuteInfo::setConnectInfo(NameObj *pObj, int a2, int a3, int a4, int a5) {
    _4 = 2;
    mExecutedObject = pObj;
    _5 = 2;
    _6 = a2;
    _7 = a3;
    _9 = a4;
    _8 = a5;
    NameObjListExecutor* exec = SingletonHolder<GameSystem>::sInstance->mSceneController->getNameObjListExecutor();

    if (a4 != -1) {
        _A = exec->registerDrawBuffer((LiveActor*)pObj, a4);
    }

    if (a2 != -1) {
        exec->incrementCheckMovement(pObj, a2);
    }

    if (a3 != -1) {
        exec->incrementCheckCalcAnim(pObj, a3);
    }

    if (a5 != -1) {
        exec->incrementCheckDraw(pObj, a5);
    }
}

void NameObjExecuteInfo::initConnectting() {
    if (_4 == 1) {
        _4 = 4;
        connectToScene();
    }
    else {
        _4 = 5;
    }

    if (_5 == 1) {
        _5 = 4;
        connectToDraw();
    }
    else {
        _5 = 5;
    }
}

void NameObjExecuteInfo::requestConnect(u8 *pData) {
    switch (*pData) {
        case 2:
            *pData = 1;
            break;
        case 5:
            *pData = 4;
            break;
        case 6:
        case 7:
            *pData = 3;
            break;
        case 1:
            return;
    }
}

void NameObjExecuteInfo::requestDisconnect(u8 *pData, bool a2) {
    switch (*pData) {
        case 1:
            *pData = 2;
            break;
        case 3:
            u8 val = 6;
            if (a2) {
                val = 7;
            }

            *pData = val;
            break;
        case 5:
        case 6:
        case 7:
            break;
        case 4:
            *pData = 5;
            break;
        case 2:
            return;
    }
}

void NameObjExecuteInfo::executeRequirementConnectMovement() {
    if (_4 == 4) {
        connectToScene();
    }
}

void NameObjExecuteInfo::executeRequirementDisconnectMovement() {
    if (_4 == 6) {
        disconnectToScene();
    }
}

void NameObjExecuteInfo::executeRequirementConnectDraw() {
    if (_5 == 4) {
        connectToDraw();
    }
}

void NameObjExecuteInfo::executeRequirementDisconnectDraw() {
    if (_5 == 6) {
        disconnectToDraw();
    }
}

void NameObjExecuteInfo::executeRequirementDisconnectDrawDelay() {
    if (_5 == 7) {
        disconnectToDraw();
    }
}

void NameObjExecuteInfo::requestMovementOn(int a1) {
    s8 arg = a1;
    if (_6 != arg) {
        return;
    }

    MR::requestMovementOn(mExecutedObject);
}

void NameObjExecuteInfo::requestMovementOff(int a1) {
    s8 arg = a1;
    if (_6 != arg) {
        return;
    }

    MR::requestMovementOff(mExecutedObject);
}

void NameObjExecuteInfo::findLightInfo() const {
    SingletonHolder<GameSystem>::sInstance->mSceneController->getNameObjListExecutor()->findLightInfo(reinterpret_cast<LiveActor*>(mExecutedObject), _9, _A);
}

void NameObjExecuteInfo::connectToScene() {
    _4 = 3;
    NameObjListExecutor* exec = SingletonHolder<GameSystem>::sInstance->mSceneController->getNameObjListExecutor();

    if (_6 != -1) {
        exec->addToMovement(mExecutedObject, _6);
    }

    if (_7 != -1) {
        exec->addToCalcAnim(mExecutedObject, _7);
    }
}

void NameObjExecuteInfo::disconnectToScene() {
    _4 = 5;
    NameObjListExecutor* exec = SingletonHolder<GameSystem>::sInstance->mSceneController->getNameObjListExecutor();

    if (_5 != -1) {
        exec->removeToMovement(mExecutedObject, _5);
    }

    if (_7 != -1) {
        exec->removeToCalcAnim(mExecutedObject, _7);
    }   
}

void NameObjExecuteInfo::connectToDraw() {
    _5 = 3;
    NameObjListExecutor* exec = SingletonHolder<GameSystem>::sInstance->mSceneController->getNameObjListExecutor();

    if (_8 != -1) {
        exec->addToDraw(mExecutedObject, _8);
    }

    if (_9 != -1) {
        exec->addToDrawBuffer((LiveActor*)mExecutedObject, _9, _A);
    }
}

void NameObjExecuteInfo::disconnectToDraw() {
    _5 = 5;
    NameObjListExecutor* exec = SingletonHolder<GameSystem>::sInstance->mSceneController->getNameObjListExecutor();

    if (_8 != -1) {
        exec->removeToDraw(mExecutedObject, _8);
    }

    if (_9 != -1) {
        exec->removeToDrawBuffer((LiveActor*)mExecutedObject, _9, _A);
    }
}

void NameObjExecuteHolder::registerActor(NameObj *pObj, int a2, int a3, int a4, int a5) {
    mExecuteArray[_14].setConnectInfo(pObj, a2, a3, a4, a5);
    pObj->mExecutorIdx = _14;
    _14++;
}

void NameObjExecuteHolder::initConnectting() {
    for (s32 i = 0; i < mExecuteCount; i++) {
        mExecuteArray[i].initConnectting();
    }
}

void NameObjExecuteHolder::connectToScene(NameObj *pObj) {
    NameObjExecuteInfo* inf = getConnectToSceneInfo(pObj);

    if (inf) {
        inf->requestConnect((u8*)&inf->_4);
        _18 = 1;
    }
}

void NameObjExecuteHolder::connectToDraw(NameObj *pObj) {
    NameObjExecuteInfo* inf = getConnectToSceneInfo(pObj);

    if (inf) {
        inf->requestConnect((u8*)&inf->_5);
        _1A = 1;
    }
}

void NameObjExecuteHolder::disconnectToScene(NameObj *pObj) {
    NameObjExecuteInfo* inf = getConnectToSceneInfo(pObj);

    if (inf) {
        inf->requestDisconnect((u8*)&inf->_4, false);
        _19 = 1;
    }
}

void NameObjExecuteHolder::disconnectToDraw(NameObj *pObj) {
    NameObjExecuteInfo* inf = getConnectToSceneInfo(pObj);

    if (inf) {
        inf->requestDisconnect((u8*)&inf->_5, false);
        _1B = 1;
    }
}

bool NameObjExecuteHolder::isConnectToDraw(const NameObj *pObj) const {
    NameObjExecuteInfo* inf = getConnectToSceneInfo(pObj);
    bool ret = false;

    if (inf) {
        if (inf->_5 == 3) {
            ret = true;
        }
    }

    return ret;
}

void NameObjExecuteHolder::executeRequirementConnectMovement() {
    if (_18) {
        for (s32 i = 0; i < mExecuteCount; i++) {
            mExecuteArray[i].executeRequirementConnectMovement();
        }

        _18 = 0;
    }
}

void NameObjExecuteHolder::executeRequirementDisconnectMovement() {
    if (_19) {
        for (s32 i = 0; i < mExecuteCount; i++) {
            mExecuteArray[i].executeRequirementDisconnectMovement();
        }

        _19 = 0;
    }
}

void NameObjExecuteHolder::executeRequirementConnectDraw() {
    if (_1A) {
        for (s32 i = 0; i < mExecuteCount; i++) {
            mExecuteArray[i].executeRequirementConnectDraw();
        }

        _1A = 0;
    }
}

void NameObjExecuteHolder::executeRequirementDisconnectDraw() {
    if (_1B) {
        for (s32 i = 0; i < mExecuteCount; i++) {
            mExecuteArray[i].executeRequirementDisconnectDraw();
        }

        _1B = 0;
    }
}

void NameObjExecuteHolder::executeRequirementDisconnectDrawDelay() {
    if (_1C) {
        for (s32 i = 0; i < mExecuteCount; i++) {
            mExecuteArray[i].executeRequirementDisconnectDrawDelay();
        }

        _1C = 0;
    }
}

void NameObjExecuteHolder::requestMovementOn(int idx) {
    for (s32 i = 0; i < mExecuteCount; i++) {
        mExecuteArray[i].requestMovementOn(idx);
    }
}

void NameObjExecuteHolder::requestMovementOff(int idx) {
    for (s32 i = 0; i < mExecuteCount; i++) {
        mExecuteArray[i].requestMovementOff(idx);
    }
}

NameObjExecuteInfo* NameObjExecuteHolder::getConnectToSceneInfo(const NameObj *pObj) const {
    if (pObj->mExecutorIdx >= 0) {
        return &mExecuteArray[pObj->mExecutorIdx];
    }

    return 0;
}

/* these functions were likely placed here via the linker */
namespace MR {
    void registerNameObjToExecuteHolder(NameObj *pObj, int a2, int a3, int a4, int a5) {
        reinterpret_cast<NameObjExecuteHolder*>(MR::getSceneObjHolder()->getObj(SceneObj_NameObjExecuteHolder))->registerActor(pObj, a2, a3, a4, a5);
    }

    void initConnectting() {
        reinterpret_cast<NameObjExecuteHolder*>(MR::getSceneObjHolder()->getObj(SceneObj_NameObjExecuteHolder))->initConnectting();
    }

    void connectToSceneTemporarily(NameObj *pObj) {
        reinterpret_cast<NameObjExecuteHolder*>(MR::getSceneObjHolder()->getObj(SceneObj_NameObjExecuteHolder))->connectToScene(pObj);
    }

    void disconnectToSceneTemporarily(NameObj *pObj) {
        reinterpret_cast<NameObjExecuteHolder*>(MR::getSceneObjHolder()->getObj(SceneObj_NameObjExecuteHolder))->disconnectToScene(pObj);
    }

    void connectToDrawTemporarily(NameObj *pObj) {
        reinterpret_cast<NameObjExecuteHolder*>(MR::getSceneObjHolder()->getObj(SceneObj_NameObjExecuteHolder))->connectToDraw(pObj);
    }

    void disconnectToDrawTemporarily(NameObj *pObj) {
        reinterpret_cast<NameObjExecuteHolder*>(MR::getSceneObjHolder()->getObj(SceneObj_NameObjExecuteHolder))->disconnectToDraw(pObj);
    }
    
    bool isConnectToDrawTemporarily(const NameObj *pObj) {
        return reinterpret_cast<NameObjExecuteHolder*>(MR::getSceneObjHolder()->getObj(SceneObj_NameObjExecuteHolder))->isConnectToDraw(pObj);
    }

    void executeRequirementConnectMovement() {
        reinterpret_cast<NameObjExecuteHolder*>(MR::getSceneObjHolder()->getObj(SceneObj_NameObjExecuteHolder))->executeRequirementConnectMovement();
    }

    void executeRequirementDisconnectMovement() {
        reinterpret_cast<NameObjExecuteHolder*>(MR::getSceneObjHolder()->getObj(SceneObj_NameObjExecuteHolder))->executeRequirementDisconnectMovement();
    }

    void executeRequirementConnectDraw() {
        reinterpret_cast<NameObjExecuteHolder*>(MR::getSceneObjHolder()->getObj(SceneObj_NameObjExecuteHolder))->executeRequirementConnectDraw();
    }

    void executeRequirementDisconnectDraw() {
        reinterpret_cast<NameObjExecuteHolder*>(MR::getSceneObjHolder()->getObj(SceneObj_NameObjExecuteHolder))->executeRequirementDisconnectDraw();
    }

    void executeRequirementDisconnectDrawDelay() {
        reinterpret_cast<NameObjExecuteHolder*>(MR::getSceneObjHolder()->getObj(SceneObj_NameObjExecuteHolder))->executeRequirementDisconnectDrawDelay();
    }

    void requestMovementOnWithCategory(int category) {
        reinterpret_cast<NameObjExecuteHolder*>(MR::getSceneObjHolder()->getObj(SceneObj_NameObjExecuteHolder))->requestMovementOn(category);
    }

    void requestMovementOffWithCategory(int category) {
        reinterpret_cast<NameObjExecuteHolder*>(MR::getSceneObjHolder()->getObj(SceneObj_NameObjExecuteHolder))->requestMovementOff(category);
    }

    void MR::findActorLightInfo(const LiveActor *pActor) {
        MR::getSceneObj<NameObjExecuteHolder*>(SceneObj_NameObjExecuteHolder)->getConnectToSceneInfo(pActor)->findLightInfo();
    }
};

NameObjExecuteHolder::NameObjExecuteHolder(int count) : NameObj("connectToScene情報保持") {
    mExecuteCount = count;
    mExecuteArray = 0;
    _14 = 0;
    _18 = 0;
    _19 = 0;
    _1A = 0;
    _1B = 0;
    _1C = 0;
    mExecuteArray = new NameObjExecuteInfo[mExecuteCount];
}

NameObjExecuteHolder::~NameObjExecuteHolder() {

}