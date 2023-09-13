#include "Game/Map/ScenarioSelectStar.h"
#include "Game/MapObj/PowerStar.h"
#include "Game/Util.h"
#include <JSystem/JMath/JMATrigonometric.h>

ScenarioSelectStar::ScenarioSelectStar(EffectSystem *pSystem) : MultiSceneActor("シナリオ選択のスター", "PowerStar", false),
    _44(0), mFrame(1), _4C(0), _50(0), _54(0.0f), _5C(gZeroVec), _68(gZeroVec) {
        _74 = 60;
        _78 = 1.0f;
        initEffect(pSystem, 0, "ScenarioStar");
    }

void ScenarioSelectStar::init(const JMapInfoIter &rIter) {
    initNerve(&NrvScenarioSelectStar::ScenarioSelectStarNrvNotPointing::sInstance);
}

void ScenarioSelectStar::calcViewAndEntry() {
    if (isNerve(&NrvScenarioSelectStar::ScenarioSelectStarNrvAppear::sInstance) || !MultiScene::isLessEqualStep(this, _74 + 1)) {
        MultiSceneActor::calcViewAndEntry();
    }
}

void ScenarioSelectStar::appear() {
    mScale.x = 0.89999998f;
    mScale.y = 0.89999998f;
    mScale.z = 0.89999998f;
    _50 = 0;
    _54 = 0.0f;
    updatePos();
    MultiScene::startBtp(this, "PowerStar");
    MultiScene::startBva(this, "PowerStar");
    MultiScene::setBtpFrameAndStop(this, PowerStar::getBtpFrameCurrentStage(mFrame));
    MultiScene::setBvaFrameAndStop(this, _44 == 0 ? 1.0f : 0.0f);
    MultiSceneActor::appear();
    setNerve(&NrvScenarioSelectStar::ScenarioSelectStarNrvAppear::sInstance);
}

void ScenarioSelectStar::kill() {
    MultiScene::forceDeleteEffectAll(this);
    MultiSceneActor::kill();
}

void ScenarioSelectStar::tryPointing() {
    _50 = 1;

    if (isNerve(&NrvScenarioSelectStar::ScenarioSelectStarNrvNotPointing::sInstance) || isNerve(&NrvScenarioSelectStar::ScenarioSelectStarNrvEndPointing::sInstance)) {
        setNerve(&NrvScenarioSelectStar::ScenarioSelectStarNrvPointing::sInstance);
    }
}

void ScenarioSelectStar::select() {
    setNerve(&NrvScenarioSelectStar::ScenarioSelectStarNrvSelected::sInstance);
}

void ScenarioSelectStar::notSelect() {
    setNerve(&NrvScenarioSelectStar::ScenarioSelectStarNrvNotSelected::sInstance);
}

bool ScenarioSelectStar::isAppearEnd() const {
    bool ret = false;

    if (!_30 && !isNerve(&NrvScenarioSelectStar::ScenarioSelectStarNrvAppear::sInstance)) {
        ret = true;
    }

    return ret;
}

void ScenarioSelectStar::setup(s32 a1, int a2, const TVec3f &a3, s32 a4) {
    _44 = a2;
    mFrame = a1;
    _5C.set(a3);
    _4C = a4;

    mRotation.set(0.0f, 0.0f, (150.0f * a4));
    int v7 = _4C;
    _74 = 15 * v7 + 60;
    _58 = 15 * v7 + ((v7 % 2) ? 0 : 0x5A);
}

void ScenarioSelectStar::control() {
    _50 = 0;
    mRotation.z = MR::modAndSubtract(mRotation.z + _54, 360.0f, 0.0f);

    if (isNerve(&NrvScenarioSelectStar::ScenarioSelectStarNrvNotPointing::sInstance)
        || isNerve(&NrvScenarioSelectStar::ScenarioSelectStarNrvPointing::sInstance)
        || isNerve(&NrvScenarioSelectStar::ScenarioSelectStarNrvEndPointing::sInstance)) {
        _58++;
        updatePos();
    }
}

bool ScenarioSelectStar::tryEndPointing() {
    if (!_50) {
        setNerve(&NrvScenarioSelectStar::ScenarioSelectStarNrvEndPointing::sInstance);
        return true;
    }

    return false;
}

/*
void ScenarioSelectStar::updatePos() {
    f32 mod = MR::modAndSubtract(360.0f * (_58 / 180.0f), 360.0f, 0.0f);
    //f32 val = MR::sin_inline(mod);

    _58 = mod;
}
*/