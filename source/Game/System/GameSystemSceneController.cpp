#include "Game/System/GameSystemSceneController.h"

#include <cstdio.h>

// SceneControlInfo::SceneControlInfo

void SceneControlInfo::setScene(const char *pSceneName) {
    snprintf(mScene, sizeof(mScene), "%s", pSceneName);
}

void SceneControlInfo::setStage(const char *pStageName) {
    snprintf(mStage, sizeof(mStage), "%s", pStageName);
}

// SceneControlInfo::setStartIdInfo