#include "Game/NPC/RunawayRabbitCollect.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

namespace {
    static const s32 sStartRunAwayBgmState = 2;
    static const s32 sFoundRabbitBgmState = 3;
    static const s32 sCaughtRabbitBgmState = 2;
    static const s32 sEndRunAwayBgmState = 1;
    static const s32 sStartRunAwayBgmChangeFrames = 60;
    static const s32 sFoundRabbitBgmChangeFrames = 30;
    static const s32 sCaughtRabbitBgmChangeFrames = 90;
    static const s32 sEndRunAwayBgmChangeFrames = 120;
};  // namespace

namespace NrvRunawayRabbitCollect {
    NEW_NERVE(RunawayRabbitCollectNrvWait, RunawayRabbitCollect, Wait);
    NEW_NERVE(RunawayRabbitCollectNrvActive, RunawayRabbitCollect, Active);
};  // namespace NrvRunawayRabbitCollect

RunawayRabbitCollect::RunawayRabbitCollect(const char* pName)
    : LiveActor(pName), mRabbit(), mRabbitNum(), _A0(), _A4(), mCompleteRabbitCount(), mBgmState() {
}

void RunawayRabbitCollect::init(const JMapInfoIter& rIter);

void RunawayRabbitCollect::initAfterPlacement() {
    MR::sendMsgToAllLiveActor(ACTMES_HEAVENSDOOR_RUNAWAY_RABBIT_WAIT, nullptr);
}

s32 RunawayRabbitCollect::calcCompleteRabbitCount() const;
void RunawayRabbitCollect::linkMsgCtrl();
void RunawayRabbitCollect::noticeAppearRabbit(RunawayRabbit*);
void RunawayRabbitCollect::noticeCaughtRabbit(RunawayRabbit*);

void RunawayRabbitCollect::control() {
}

void RunawayRabbitCollect::exeWait();
void RunawayRabbitCollect::appearTico(RunawayTico*, const TVec3f&);
void RunawayRabbitCollect::exeActive();
