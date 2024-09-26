#include "Game/Screen/ScenarioTitle.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

namespace {
    NERVE_DECL_EXE(ScenarioTitleAppear, ScenarioTitle, Appear);
    NERVE_DECL_EXE(ScenarioTitleWait, ScenarioTitle, Wait);
    NERVE_DECL_EXE(ScenarioTitleEnd, ScenarioTitle, End);

    INIT_NERVE(ScenarioTitleAppear);
    INIT_NERVE(ScenarioTitleWait);
    INIT_NERVE(ScenarioTitleEnd);
};  // namespace

ScenarioTitle::ScenarioTitle() : LayoutActor("シナリオ名表示", true) {}

void ScenarioTitle::init(const JMapInfoIter& rInfo) {
    initNerve(&ScenarioTitleAppear::sInstance);
    MR::connectToSceneLayout(this);
    initLayoutManager("StageTitle", 1);
    kill();
}

void ScenarioTitle::start() {
    appear();
    setNerve(&ScenarioTitleAppear::sInstance);
}

void ScenarioTitle::end() {
    setNerve(&ScenarioTitleEnd::sInstance);
}

void ScenarioTitle::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::setTextBoxMessageRecursive(this, "Galaxy", MR::getCurrentGalaxyNameOnCurrentLanguage());
        MR::setTextBoxMessageRecursive(this, "Scenario", MR::getCurrentScenarioNameOnCurrentLanguage());
        MR::startAnim(this, "Appear", 0);
    }

    MR::setNerveAtAnimStopped(this, &ScenarioTitleWait::sInstance, 0);
}

void ScenarioTitle::exeEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "End", 0);
    }

    if (MR::isAnimStopped(this, 0)) {
        kill();
    }
}

ScenarioTitle::~ScenarioTitle() {}