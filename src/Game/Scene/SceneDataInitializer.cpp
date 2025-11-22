#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Scene/SceneDataInitializer.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Screen/LuigiLetter.hpp"

SceneDataInitializer::SceneDataInitializer()
    : NameObj("SceneDataInitializer") {
    mFileLoader = nullptr;
    mDataHolder = nullptr;
}

void SceneDataInitializer::startStageFileLoad() {
    mFileLoader = new StageFileLoader(MR::getCurrentStageName());
    mFileLoader->startLoadingStageFile();

    if (!MR::isStageDisablePauseMenu()) {
        MR::mountAsyncArchive("/LayoutData/PauseMenu.arc", MR::getAproposHeapForSceneArchive(0.03f));
    }
}

void SceneDataInitializer::startStageFileLoadAfterScenarioSelected() {
    NameObjArchiveListCollector collector;
    JMapInfoIter                iter;
    iter.mInfo = nullptr;
    iter.mIndex = -1;

    LuigiLetter::makeArchiveListForMenu(&collector, iter);

    for (int i = 0; i < collector.mCount; i++) {
        MR::mountAsyncArchiveByObjectOrLayoutName(collector.getArchive(i), nullptr);
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
