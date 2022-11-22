#include "Game/Scene/Scene.h"

Scene::Scene(const char *pName) : NerveExecutor(pName) {
    mListExecutor = NULL;
    _C = 0;
    mSceneObjHolder = NULL;
}

Scene::~Scene() {
    if (mSceneObjHolder != NULL) {
        delete mSceneObjHolder;
    }

    if (mListExecutor != NULL) {
        delete mListExecutor;
    }
}

void Scene::init() {

}

void Scene::start() {

}

void Scene::update() {

}
 
void Scene::draw() const {

}

void Scene::calcAnim() {

}

void Scene::initNameObjListExecutor() {
    SceneNameObjListExecutor* exec = new SceneNameObjListExecutor();
    exec->init();
    mListExecutor = exec;
}

void Scene::initSceneObjHolder() {
    mSceneObjHolder = new SceneObjHolder();
}