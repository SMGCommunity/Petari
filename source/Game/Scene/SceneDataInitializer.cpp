#include "Game/Scene/SceneDataInitializer.h"
#include "Game/Scene/SceneObjHolder.h"
#include "Game/Screen/LuigiLetter.h"

SceneDataInitializer::SceneDataInitializer() : NameObj("SceneDataInitializer") {
    mFileLoader = NULL;
    mDataHolder = NULL;
}

void SceneDataInitializer::startStageFileLoad() {
    mFileLoader = new StageFileLoader(MR::getCurrentStageName());
    mFileLoader->startLoadingStageFile();

    if (!MR::isStageDisablePauseMenu()) {
        MR::mountAsyncArchive("/LayoutData/PauseMenu.arc", MR::getAproposHeapForSceneArchive(0.029999999f));
    }
}

void SceneDataInitializer::startStageFileLoadAfterScenarioSelected() {
    NameObjArchiveListCollector collector;
    JMapInfoIter iter;
    iter.mInfo = NULL;
    iter._4 = -1;
    LuigiLetter::makeArchiveListForMenu(&collector, iter);

    s32 curArchive = 0;
    while (curArchive < collector.mCount) {
        MR::mountAsyncArchiveByObjectOrLayoutName(collector.getArchive(curArchive), NULL);
        curArchive++;
    }
}

void SceneDataInitializer::waitDoneStageFileLoad() {
    mFileLoader->waitLoadedStageFile();
    mDataHolder = reinterpret_cast<StageDataHolder*>(MR::createSceneObj(SceneObj_StageDataHolder));
}

void SceneDataInitializer::startActorFileLoadCommon() {
    mDataHolder->requestFileLoadCommon();
}

void SceneDataInitializer::startActorFileLoadScenario() {
    mDataHolder->requestFileLoadScenario();
}

void SceneDataInitializer::startActorPlacement() {
    mDataHolder->initPlacement();
}

void SceneDataInitializer::initAfterScenarioSelected() {
    mDataHolder->initAfterScenarioSelected();
}

SceneDataInitializer::~SceneDataInitializer() {

}
