#include "Game/Map/RaceManager.hpp"
#include "Game/Util/LayoutUtil.hpp"

RaceManagerLayout::RaceManagerLayout(const char* pName) :
    LayoutActor(pName, true)
{
    
}

void RaceManagerLayout::init(const JMapInfoIter& rIter) {
    MR::connectToSceneLayout(this);
    initLayoutManager("Race", 1);
    MR::createAndAddPaneCtrl(this, "Go", 1);
    MR::createAndAddPaneCtrl(this, "Goal", 1);
    MR::createAndAddPaneCtrl(this, "Lose", 1);
    MR::createAndAddPaneCtrl(this, "TimeUp", 1);
    MR::createAndAddPaneCtrl(this, "Rank", 1);
    MR::createAndAddPaneCtrl(this, "ScoreTime", 1);
    initEffectKeeper(2, "Race", nullptr);
    hideAllPane();
}

void RaceManagerLayout::playCountAndGo() {
    MR::showPane(this, "Go");
    MR::showPane(this, "BestRecord");
    MR::startPaneAnim(this, "Go", "Count", 0);
}

void RaceManagerLayout::playGo() {
    MR::showPane(this, "Go");
    MR::startPaneAnim(this, "Go", "Count", 0);
    MR::setPaneAnimFrame(this, "Go", 180.0f, 0);
    MR::emitEffect(this, "RaceStart");
}

void RaceManagerLayout::playGoal() {
    MR::showPane(this, "Goal");
    MR::startPaneAnim(this, "Goal", "Goal", 0);
    MR::startPaneAnim(this, "ScoreTime", "Goal", 0);
    MR::emitEffect(this, "RaceGoal");
}

void RaceManagerLayout::playLose() {
    MR::showPane(this, "Lose");
    MR::startPaneAnim(this, "Lose", "Lose", 0);
    MR::startPaneAnim(this, "ScoreTime", "Lose", 0);
}

void RaceManagerLayout::playTimeUp() {
    MR::showPane(this, "TimeUp");
    MR::startPaneAnim(this, "TimeUp", "TimeUp", 0);
    MR::startPaneAnim(this, "ScoreTime", "TimeUp", 0);
}

void RaceManagerLayout::playRank(u32 rank) {
    MR::showPane(this, "Rank");
    MR::startPaneAnim(this, "Rank", "Rank", 0);
    MR::setPaneAnimFrameAndStop(this, "Rank", rank, 0);
}

bool RaceManagerLayout::isAllAnimStopped() {
    return MR::isPaneAnimStopped(this, "Go", 0)
        && MR::isPaneAnimStopped(this, "Goal", 0)
        && MR::isPaneAnimStopped(this, "Lose", 0)
        && MR::isPaneAnimStopped(this, "TimeUp", 0)
        && MR::isPaneAnimStopped(this, "ScoreTime", 0)
        && MR::isPaneAnimStopped(this, "Rank", 0);
}

void RaceManagerLayout::hideRecordPane() {
    MR::hidePane(this, "Record");
}

void RaceManagerLayout::hideBestRecordPane() {
    MR::hidePane(this, "BestRecord");
}

void RaceManagerLayout::hideAllPane() {
    MR::hidePane(this, "Go");
    MR::hidePane(this, "Goal");
    MR::hidePane(this, "Lose");
    MR::hidePane(this, "TimeUp");
    MR::hidePane(this, "Record");
    MR::hidePane(this, "BestRecord");
    MR::hidePane(this, "Rank");
}

bool RaceManagerLayout::isPlayCountAnim() const {
    return MR::getPaneAnimFrame(this, "Go", 0) < 180.0f;
}

bool RaceManagerLayout::isPlayCountAndGoAnim() const {
    return !MR::isPaneAnimStopped(this, "Go", 0);
}

void RaceManagerLayout::setTime(u32 time) {
    MR::updateClearTimeTextBox(this, "TimeCounter", time);
}

void RaceManagerLayout::setBestTime(u32 bestTime) {
    MR::updateClearTimeTextBox(this, "BestTimeCounter", bestTime);
}
