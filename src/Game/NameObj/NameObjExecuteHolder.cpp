#include "Game/NameObj/NameObjExecuteHolder.hpp"
#include "Game/NameObj/NameObjListExecutor.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/SingletonHolder.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemSceneController.hpp"

namespace {
    NameObjExecuteHolder* getNameObjExecuteHolder() {
        return MR::getSceneObj< NameObjExecuteHolder >(SceneObj_NameObjExecuteHolder);
    }
};  // namespace

NameObjExecuteInfo::NameObjExecuteInfo() : mExecutedObj(nullptr), _4(0), _5(0), _6(-1), _7(-1), _8(-1), _9(-1), _A(-1) {}

void NameObjExecuteInfo::setConnectInfo(NameObj* pObj, int a2, int a3, int a4, int a5) {
    _4 = 2;
    mExecutedObj = pObj;
    _5 = 2;
    _6 = a2;
    _7 = a3;
    _9 = a4;
    _8 = a5;
    NameObjListExecutor* pListExecutor = SingletonHolder< GameSystem >::get()->mSceneController->getNameObjListExecutor();

    if (a4 != -1) {
        _A = pListExecutor->registerDrawBuffer(static_cast< LiveActor* >(pObj), a4);
    }

    if (a2 != -1) {
        pListExecutor->incrementCheckMovement(pObj, a2);
    }

    if (a3 != -1) {
        pListExecutor->incrementCheckCalcAnim(pObj, a3);
    }

    if (a5 != -1) {
        pListExecutor->incrementCheckDraw(pObj, a5);
    }
}

void NameObjExecuteInfo::initConnectting() {
    if (_4 == 1) {
        _4 = 4;
        connectToScene();
    } else {
        _4 = 5;
    }

    if (_5 == 1) {
        _5 = 4;
        connectToDraw();
    } else {
        _5 = 5;
    }
}

void NameObjExecuteInfo::requestConnect(u8* pData) {
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

void NameObjExecuteInfo::requestDisconnect(u8* pData, bool a2) {
    u8 val;

    switch (*pData) {
    case 1:
        *pData = 2;
        break;
    case 3:
        val = 6;
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
    if (_6 != static_cast< s8 >(a1)) {
        return;
    }

    MR::requestMovementOn(mExecutedObj);
}

void NameObjExecuteInfo::requestMovementOff(int a1) {
    if (_6 != static_cast< s8 >(a1)) {
        return;
    }

    MR::requestMovementOff(mExecutedObj);
}

void NameObjExecuteInfo::findLightInfo() const {
    SingletonHolder< GameSystem >::get()->mSceneController->getNameObjListExecutor()->findLightInfo(static_cast< LiveActor* >(mExecutedObj), _9, _A);
}

void NameObjExecuteInfo::connectToScene() {
    _4 = 3;
    NameObjListExecutor* pListExecutor = SingletonHolder< GameSystem >::get()->mSceneController->getNameObjListExecutor();

    if (_6 != -1) {
        pListExecutor->addToMovement(mExecutedObj, _6);
    }

    if (_7 != -1) {
        pListExecutor->addToCalcAnim(mExecutedObj, _7);
    }
}

void NameObjExecuteInfo::disconnectToScene() {
    _4 = 5;
    NameObjListExecutor* pListExecutor = SingletonHolder< GameSystem >::get()->mSceneController->getNameObjListExecutor();

    if (_6 != -1) {
        pListExecutor->removeToMovement(mExecutedObj, _6);
    }

    if (_7 != -1) {
        pListExecutor->removeToCalcAnim(mExecutedObj, _7);
    }
}

void NameObjExecuteInfo::connectToDraw() {
    _5 = 3;
    NameObjListExecutor* pListExecutor = SingletonHolder< GameSystem >::get()->mSceneController->getNameObjListExecutor();

    if (_8 != -1) {
        pListExecutor->addToDraw(mExecutedObj, _8);
    }

    if (_9 != -1) {
        pListExecutor->addToDrawBuffer(static_cast< LiveActor* >(mExecutedObj), _9, _A);
    }
}

void NameObjExecuteInfo::disconnectToDraw() {
    _5 = 5;
    NameObjListExecutor* pListExecutor = SingletonHolder< GameSystem >::get()->mSceneController->getNameObjListExecutor();

    if (_8 != -1) {
        pListExecutor->removeToDraw(mExecutedObj, _8);
    }

    if (_9 != -1) {
        pListExecutor->removeToDrawBuffer(static_cast< LiveActor* >(mExecutedObj), _9, _A);
    }
}

void NameObjExecuteHolder::registerActor(NameObj* pObj, int a2, int a3, int a4, int a5) {
    mExecuteArray[mExecuteArraySize].setConnectInfo(pObj, a2, a3, a4, a5);
    pObj->mExecutorIdx = mExecuteArraySize;
    mExecuteArraySize++;
}

void NameObjExecuteHolder::initConnectting() {
    for (s32 i = 0; i < mExecuteArraySize; i++) {
        mExecuteArray[i].initConnectting();
    }
}

void NameObjExecuteHolder::connectToScene(NameObj* pObj) {
    NameObjExecuteInfo* pInfo = getConnectToSceneInfo(pObj);

    if (pInfo == nullptr) {
        return;
    }

    pInfo->requestConnect(reinterpret_cast< u8* >(&pInfo->_4));

    _18 = true;
}

void NameObjExecuteHolder::connectToDraw(NameObj* pObj) {
    NameObjExecuteInfo* pInfo = getConnectToSceneInfo(pObj);

    if (pInfo == nullptr) {
        return;
    }

    pInfo->requestConnect(reinterpret_cast< u8* >(&pInfo->_5));

    _1A = true;
}

void NameObjExecuteHolder::disconnectToScene(NameObj* pObj) {
    NameObjExecuteInfo* pInfo = getConnectToSceneInfo(pObj);

    if (pInfo == nullptr) {
        return;
    }

    pInfo->requestDisconnect(reinterpret_cast< u8* >(&pInfo->_4), false);

    _19 = true;
}

void NameObjExecuteHolder::disconnectToDraw(NameObj* pObj) {
    NameObjExecuteInfo* pInfo = getConnectToSceneInfo(pObj);

    if (pInfo == nullptr) {
        return;
    }

    pInfo->requestDisconnect(reinterpret_cast< u8* >(&pInfo->_5), false);

    _1B = true;
}

bool NameObjExecuteHolder::isConnectToDraw(const NameObj* pObj) const {
    NameObjExecuteInfo* pInfo = getConnectToSceneInfo(pObj);
    bool ret = pInfo != nullptr && pInfo->_5 == 3;

    return ret;
}

void NameObjExecuteHolder::executeRequirementConnectMovement() {
    if (!_18) {
        return;
    }

    for (s32 i = 0; i < mExecuteArraySize; i++) {
        mExecuteArray[i].executeRequirementConnectMovement();
    }

    _18 = false;
}

void NameObjExecuteHolder::executeRequirementDisconnectMovement() {
    if (!_19) {
        return;
    }

    for (s32 i = 0; i < mExecuteArraySize; i++) {
        mExecuteArray[i].executeRequirementDisconnectMovement();
    }

    _19 = false;
}

void NameObjExecuteHolder::executeRequirementConnectDraw() {
    if (!_1A) {
        return;
    }

    for (s32 i = 0; i < mExecuteArraySize; i++) {
        mExecuteArray[i].executeRequirementConnectDraw();
    }

    _1A = false;
}

void NameObjExecuteHolder::executeRequirementDisconnectDraw() {
    if (!_1B) {
        return;
    }

    for (s32 i = 0; i < mExecuteArraySize; i++) {
        mExecuteArray[i].executeRequirementDisconnectDraw();
    }

    _1B = false;
}

void NameObjExecuteHolder::executeRequirementDisconnectDrawDelay() {
    if (!_1C) {
        return;
    }

    for (s32 i = 0; i < mExecuteArraySize; i++) {
        mExecuteArray[i].executeRequirementDisconnectDrawDelay();
    }

    _1C = false;
}

void NameObjExecuteHolder::requestMovementOn(int idx) {
    for (s32 i = 0; i < mExecuteArraySize; i++) {
        mExecuteArray[i].requestMovementOn(idx);
    }
}

void NameObjExecuteHolder::requestMovementOff(int idx) {
    for (s32 i = 0; i < mExecuteArraySize; i++) {
        mExecuteArray[i].requestMovementOff(idx);
    }
}

NameObjExecuteInfo* NameObjExecuteHolder::getConnectToSceneInfo(const NameObj* pObj) const {
    if (pObj->mExecutorIdx >= 0) {
        return &mExecuteArray[pObj->mExecutorIdx];
    }

    return nullptr;
}

namespace MR {
    void registerNameObjToExecuteHolder(NameObj* pObj, int a2, int a3, int a4, int a5) {
        getNameObjExecuteHolder()->registerActor(pObj, a2, a3, a4, a5);
    }

    void initConnectting() {
        getNameObjExecuteHolder()->initConnectting();
    }

    void connectToSceneTemporarily(NameObj* pObj) {
        getNameObjExecuteHolder()->connectToScene(pObj);
    }

    void disconnectToSceneTemporarily(NameObj* pObj) {
        getNameObjExecuteHolder()->disconnectToScene(pObj);
    }

    void connectToDrawTemporarily(NameObj* pObj) {
        getNameObjExecuteHolder()->connectToDraw(pObj);
    }

    void disconnectToDrawTemporarily(NameObj* pObj) {
        getNameObjExecuteHolder()->disconnectToDraw(pObj);
    }

    bool isConnectToDrawTemporarily(const NameObj* pObj) {
        return getNameObjExecuteHolder()->isConnectToDraw(pObj);
    }

    void executeRequirementConnectMovement() {
        getNameObjExecuteHolder()->executeRequirementConnectMovement();
    }

    void executeRequirementDisconnectMovement() {
        getNameObjExecuteHolder()->executeRequirementDisconnectMovement();
    }

    void executeRequirementConnectDraw() {
        getNameObjExecuteHolder()->executeRequirementConnectDraw();
    }

    void executeRequirementDisconnectDraw() {
        getNameObjExecuteHolder()->executeRequirementDisconnectDraw();
    }

    void executeRequirementDisconnectDrawDelay() {
        getNameObjExecuteHolder()->executeRequirementDisconnectDrawDelay();
    }

    void requestMovementOnWithCategory(int category) {
        getNameObjExecuteHolder()->requestMovementOn(category);
    }

    void requestMovementOffWithCategory(int category) {
        getNameObjExecuteHolder()->requestMovementOff(category);
    }

    void findActorLightInfo(const LiveActor* pActor) {
        getNameObjExecuteHolder()->getConnectToSceneInfo(pActor)->findLightInfo();
    }
};  // namespace MR

NameObjExecuteHolder::NameObjExecuteHolder(int size)
    : NameObj("connectToScene情報保持"), mExecuteArray(nullptr), mExecuteArrayMaxSize(size), mExecuteArraySize(0), _18(false), _19(false), _1A(false),
      _1B(false), _1C(false) {
    mExecuteArray = new NameObjExecuteInfo[mExecuteArrayMaxSize];
}
