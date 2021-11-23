#include "Game/NameObj/NameObjRegister.h"

void NameObjRegister::setCurrentHolder(NameObjHolder *pHolder) {
    mHolder = pHolder;
}

void NameObjRegister::add(NameObj *pObj) {
    mHolder->add(pObj);
}

NameObjRegister::NameObjRegister() : mHolder(0) {
    
}