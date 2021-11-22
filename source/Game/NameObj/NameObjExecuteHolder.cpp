#include "Game/NameObj/NameObjExecuteHolder.h"
#include "Game/NameObj/NameObjListExecutor.h"
#include "Game/LiveActor/LiveActor.h"
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

// NameObjExecuteInfo::findLightInfo

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

NameObjExecuteInfo* NameObjExecuteHolder::getConnectToSceneInfo(const NameObj *pObj) const {
    if (pObj->_A >= 0) {
        return &mExecuteArray[pObj->_A];
    }

    return 0;
}

void NameObjExecuteHolder::registerActor(NameObj *pObj, int a2, int a3, int a4, int a5) {
    mExecuteArray[_14].setConnectInfo(pObj, a2, a3, a4, a5);
    pObj->_A = _14;
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