#include "Game/NameObj/NameObjExecuteHolder.h"

NameObjExecuteInfo* NameObjExecuteHolder::getConnectToSceneInfo(const NameObj *pObj) const {
    if (pObj->_A >= 0) {
        return &mExecuteArray[pObj->_A];
    }

    return 0;
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