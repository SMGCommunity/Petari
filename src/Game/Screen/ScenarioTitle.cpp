#include "Game/Screen/ScenarioTitle.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace {
    NEW_NERVE(ScenarioTitleAppear, ScenarioTitle, Appear);
    NEW_NERVE(ScenarioTitleWait, ScenarioTitle, Wait);
    NEW_NERVE(ScenarioTitleEnd, ScenarioTitle, End);
}; // namespace

ScenarioTitle::ScenarioTitle()
    : LayoutActor("シナリオ名表示", true) {
}

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

void ScenarioTitle::exeWait() {
}

void ScenarioTitle::exeEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "End", 0);
    }

    if (MR::isAnimStopped(this, 0)) {
        kill();
    }
}
