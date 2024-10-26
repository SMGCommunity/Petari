#include "Game/NameObj/NameObjListExecutor.hpp"

NameObjListExecutor::NameObjListExecutor() {
    mBufferHolder = 0;
    mMovementList = 0;
    mCalcAnimList = 0;
    mDrawList = 0;
}

#ifdef NON_MATCHING
// meh
NameObjListExecutor::~NameObjListExecutor() {
    delete mMovementList;
    delete mCalcAnimList;
    delete mDrawList;

    if (mBufferHolder) {
        delete mBufferHolder->_8;
    }
}
#endif

void NameObjListExecutor::init() {
    initMovementList();
    initCalcAnimList();
    initCalcViewAndEntryList();
    initDrawList();
}

s16 NameObjListExecutor::registerDrawBuffer(LiveActor *pActor, int a2) {
    return mBufferHolder->registerDrawBuffer(pActor, a2);
}

void NameObjListExecutor::allocateDrawBufferActorList() {
    mMovementList->allocateBuffer();
    mCalcAnimList->allocateBuffer();
    mDrawList->allocateBuffer();
    mBufferHolder->allocateActorListBuffer();
}

void NameObjListExecutor::registerPreDrawFunction(const MR::FunctorBase &rFunc, int a2) {
    mDrawList->registerExecuteBeforeFunction(rFunc, a2);
}

void NameObjListExecutor::findLightInfo(LiveActor *pActor, int a2, int a3) const {
    mBufferHolder->findLightInfo(pActor, a2, a3);
}

// NameObjListExecutor::findLightInfo

void NameObjListExecutor::incrementCheckMovement(NameObj *pObj, int category) {
    mMovementList->incrementCheck(pObj, category);
}

void NameObjListExecutor::incrementCheckCalcAnim(NameObj *pObj, int category) {
    mCalcAnimList->incrementCheck(pObj, category);
}

void NameObjListExecutor::incrementCheckDraw(NameObj *pObj, int category) {
    mDrawList->incrementCheck(pObj, category);
}

void NameObjListExecutor::addToMovement(NameObj *pObj, int category) {
    mMovementList->add(pObj, category);
}

void NameObjListExecutor::addToCalcAnim(NameObj *pObj, int category) {
    mCalcAnimList->add(pObj, category);
}

void NameObjListExecutor::addToDrawBuffer(LiveActor *pActor, int a2, int a3) {
    mBufferHolder->active(pActor, a2, a3);
}

void NameObjListExecutor::addToDraw(NameObj *pObj, int category) {
    mDrawList->add(pObj, category);
}

void NameObjListExecutor::removeToMovement(NameObj *pObj, int category) {
    mMovementList->remove(pObj, category);
}

void NameObjListExecutor::removeToCalcAnim(NameObj *pObj, int category) {
    mCalcAnimList->remove(pObj, category);
}

void NameObjListExecutor::removeToDrawBuffer(LiveActor *pActor, int a2, int a3) {
    mBufferHolder->deactive(pActor, a2, a3);
}

void NameObjListExecutor::removeToDraw(NameObj *pObj, int category) {
    mDrawList->remove(pObj, category);
}

void NameObjListExecutor::executeMovement(int category) {
    mMovementList->execute(category);
}

void NameObjListExecutor::executeCalcAnim(int category) {
    mCalcAnimList->execute(category);
}

void NameObjListExecutor::entryDrawBuffer2D() {
    mBufferHolder->entry(1);
}

void NameObjListExecutor::entryDrawBuffer3D() {
    mBufferHolder->entry(0);
}

void NameObjListExecutor::entryDrawBufferMirror() {
    mBufferHolder->entry(2);
}

void NameObjListExecutor::drawOpa(int a1) {
    mBufferHolder->drawOpa(a1);
}

void NameObjListExecutor::drawXlu(int a1) {
    mBufferHolder->drawXlu(a1);
}

void NameObjListExecutor::executeDraw(int category) {
    mDrawList->execute(category);
}

void NameObjListExecutor::initMovementList() {

}

void NameObjListExecutor::initCalcAnimList() {

}

void NameObjListExecutor::initCalcViewAndEntryList() {

}

void NameObjListExecutor::initDrawList() {

}