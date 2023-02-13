#include "Game/Scene/Scene.h"

Scene::Scene(const char *pName) : NerveExecutor(pName) {
    mListExecutor = nullptr;
    _C = 0;
    mSceneObjHolder = nullptr;
}

Scene::~Scene() {
    if (mSceneObjHolder != nullptr) {
        delete mSceneObjHolder;
    }

    if (mListExecutor != nullptr) {
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