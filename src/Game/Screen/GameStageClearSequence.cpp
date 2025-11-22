#include "Game/Screen/GameStageClearSequence.hpp"
#include "Game/MapObj/PowerStar.hpp"
#include "Game/Screen/LayoutActor.hpp"
#include "Game/Screen/SimpleLayout.hpp"
#include "Game/Util.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SequenceUtil.hpp"

namespace NrvGameStageClearSequence {
    NEW_NERVE(GameStageClearSequencePowerStarGetDemo, GameStageClearSequence, PowerStarGetDemo);
    NEW_NERVE(GameStageClearSequenceGrandStarGetDemo, GameStageClearSequence, GrandStarGetDemo);
};  // namespace NrvGameStageClearSequence

GameStageClearSequence::GameStageClearSequence() : LayoutActor("ステージクリアシーケンス", true) {
    mEffectLayout = nullptr;
    mFollowPos.x = 0.0f;
    mFollowPos.y = 0.0f;
}

void GameStageClearSequence::init(const JMapInfoIter& rIter) {
    mEffectLayout = new SimpleEffectLayout("スター取得", "GetStar", 1, -1);
    mEffectLayout->initWithoutIter();
    MR::setFollowPos(&mFollowPos, mEffectLayout, nullptr);
    MR::setFollowTypeAdd(mEffectLayout, nullptr);
    mEffectLayout->kill();
    initNerve(&NrvGameStageClearSequence::GameStageClearSequencePowerStarGetDemo::sInstance);
    kill();
}

void GameStageClearSequence::startPowerStarGetDemo() {
    LayoutActor::appear();
    TVec2f pos;
    pos.x = 0.0f;
    pos.y = 80.0f;
    mFollowPos.set(pos);
    MR::setTextBoxGameMessageRecursive(mEffectLayout, "GetStar", "GetStar_PowerStar");
    setNerve(&NrvGameStageClearSequence::GameStageClearSequencePowerStarGetDemo::sInstance);
}

void GameStageClearSequence::startGrandStarGetDemo() {
    LayoutActor::appear();
    TVec2f pos;
    pos.x = 0.0f;
    pos.y = 80.0f;
    mFollowPos.set(pos);
    MR::setTextBoxGameMessageRecursive(mEffectLayout, "GetStar", "GetStar_GrandStar");
    setNerve(&NrvGameStageClearSequence::GameStageClearSequenceGrandStarGetDemo::sInstance);
}

void GameStageClearSequence::appearGetStarLayout() {
    mEffectLayout->appear();
    MR::startAnim(mEffectLayout, "GetStar", 0);
    MR::emitEffect(mEffectLayout, "GetStar");
}

void GameStageClearSequence::exePowerStarGetDemo() {
    if (MR::isFirstStep(this)) {
        MR::stopStageBGM(2);
        MR::setSoundVolumeSetting(1, 20);
        MR::stopScene(10);
        MR::resetGlobalTimer();
        MR::deactivateDefaultGameLayout();
        MR::endGalaxyCometEvent();
    }

    if (MR::isStep(this, 15)) {
        MR::startSubBGM("BGM_CLEAR", false);
    }

    if (MR::isStep(this, 247)) {
        appearGetStarLayout();
    }

    if (MR::isStep(this, 325)) {
        MR::closeWipeCircle(80);
    }

    bool isWipeActive = MR::isWipeActive();
    if (MR::isGreaterStep(this, 325)) {
        if (!isWipeActive) {
            MR::requestChangeStageAfterStageClear();
            kill();
        }
    }
}

void GameStageClearSequence::exeGrandStarGetDemo() {
    if (MR::isFirstStep(this)) {
        MR::stopStageBGM(2);
        MR::setSoundVolumeSetting(1, 20);
        MR::stopScene(10);
        MR::resetGlobalTimer();
        MR::deactivateDefaultGameLayout();
        MR::endGalaxyCometEvent();
    }

    if (MR::isStep(this, 53)) {
        if (PowerStar::isCurrentStageKoopaVs3()) {
            MR::startSubBGM("BGM_GRAND_STAR_GET_2", false);
        } else {
            MR::startSubBGM("BGM_GRAND_STAR_GET", false);
        }
    }

    if (MR::isStep(this, 510)) {
        appearGetStarLayout();
    }

    if (PowerStar::isCurrentStageKoopaVs3()) {
        if (MR::isStep(this, 840)) {
            MR::closeSystemWipeWhiteFade(60);
        }

        bool wipeActive = MR::isSystemWipeActive();

        if (MR::isGreaterStep(this, 840) && !wipeActive) {
            MR::requestChangeStageAfterStageClear();
            kill();
        }
    } else {
        if (MR::isStep(this, 670)) {
            MR::closeWipeCircle(-1);
        }

        bool wipeActive = MR::isWipeActive();

        if (MR::isGreaterStep(this, 670) && !wipeActive) {
            MR::requestChangeStageAfterStageClear();
            kill();
        }
    }
}
