#include "Game/NameObj/NameObjRegister.hpp"

void NameObjRegister::setCurrentHolder(NameObjHolder *pHolder) {
    mHolder = pHolder;
}

void NameObjRegister::add(NameObj *pObj) {
    mHolder->add(pObj);
}

NameObjRegister::NameObjRegister() : mHolder(0) {
    
}