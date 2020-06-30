#include "Scene/Scene.h"
#include "smg.h"

Scene::Scene(const char *pSceneName) : NerveExecutor(pSceneName)
{
    mExecutor = nullptr;
    _C = 0;
    mObjHolder =  nullptr;
}

Scene::~Scene()
{
    if (mObjHolder)
    {
        delete mObjHolder;
    }

    if (mExecutor)
    {
        delete mExecutor;
    }
}

void Scene::init() { }

void Scene::start() { }

void Scene::update() { }

void Scene::draw() const { }

void Scene::calcAnim() { }

void Scene::initNameObjListExecutor()
{
    // this doesn't get inlined like it should
    mExecutor = new SceneNameObjListExecutor();
    mExecutor->init();
}

void Scene::initSceneObjHolder()
{
    mObjHolder = new SceneObjHolder();
}