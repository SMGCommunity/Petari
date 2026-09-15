#include "Game/Screen/ScenarioTitle.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace {
    NEW_NERVE(ScenarioTitleAppear, ScenarioTitle, Appear);
    NEW_NERVE(ScenarioTitleWait, ScenarioTitle, Wait);
    NEW_NERVE(ScenarioTitleEnd, ScenarioTitle, End);
};  // namespace

ScenarioTitle::ScenarioTitle() : LayoutActor("シナリオ名表示", true) {
}

void ScenarioTitle::init(const JMapInfoIter& rInfo) {
    initNerve(GET_NERVE_GLOBAL(ScenarioTitleAppear));
    MR::connectToSceneLayout(this);
    initLayoutManager("StageTitle", 1);
    kill();
}

void ScenarioTitle::start() {
    appear();
    setNerve(GET_NERVE_GLOBAL(ScenarioTitleAppear));
}

void ScenarioTitle::end() {
    setNerve(GET_NERVE_GLOBAL(ScenarioTitleEnd));
}

void ScenarioTitle::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::setTextBoxMessageRecursive(this, "Galaxy", MR::getCurrentGalaxyNameOnCurrentLanguage());
        MR::setTextBoxMessageRecursive(this, "Scenario", MR::getCurrentScenarioNameOnCurrentLanguage());
        MR::startAnim(this, "Appear", 0);
    }

    MR::setNerveAtAnimStopped(this, GET_NERVE_GLOBAL(ScenarioTitleWait), 0);
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
