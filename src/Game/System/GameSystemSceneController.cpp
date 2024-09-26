#include "Game/System/GameSystemSceneController.hpp"
#include "Game/Util.hpp"
#include <cstdio>

SceneControlInfo::SceneControlInfo() {
    _48 = nullptr;
    mScene[0] = 0;
    mStage[0] = 0;
    _40 = 1;
    _44 = 1;
    _48 = new JMapIdInfo(MR::getInitializeStartIdInfo());
    
}

void SceneControlInfo::setScene(const char *pSceneName) {
    snprintf(mScene, sizeof(mScene), "%s", pSceneName);
}

void SceneControlInfo::setStage(const char *pStageName) {
    snprintf(mStage, sizeof(mStage), "%s", pStageName);
}

void SceneControlInfo::setStartIdInfo(const JMapIdInfo &rInfo) {
    *_48 = rInfo;
} 