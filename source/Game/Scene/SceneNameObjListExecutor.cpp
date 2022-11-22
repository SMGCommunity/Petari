#include "Game/Scene/SceneNameObjListExecutor.h"

static bool false_cond = false;
static bool true_cond = true;

void SceneNameObjListExecutor::initMovementList() {
    mMovementList = new NameObjCategoryList(0x2E, &cMovementListInitTable, &NameObj::executeMovement, true_cond, "");
}

void SceneNameObjListExecutor::initCalcAnimList() {
    mCalcAnimList = new NameObjCategoryList(0x16, &cCalcAnimListInitTable, &NameObj::calcAnim, false_cond, "");
}

void SceneNameObjListExecutor::initCalcViewAndEntryList() {
    mBufferHolder = new DrawBufferHolder();
    mBufferHolder->initTable(&cDrawBufferListInitTable, 0x29);
}

void SceneNameObjListExecutor::initDrawList() {
    mDrawList = new NameObjCategoryList(0x53, &cDrawListInitTable, &NameObj::draw, true_cond, "");
}

SceneNameObjListExecutor::~SceneNameObjListExecutor() {

}
