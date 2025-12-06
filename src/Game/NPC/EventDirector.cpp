#include "Game/NPC/EventDirector.hpp"
#include "Game/MapObj/PowerStarHolder.hpp"
#include "Game/NPC/CometEventKeeper.hpp"
#include "Game/NPC/PowerStarEventKeeper.hpp"
#include "Game/NPC/StageStateKeeper.hpp"
#include "Game/NPC/TimeAttackEventKeeper.hpp"
#include "Game/Scene/SceneObjHolder.hpp"

EventDirector::EventDirector()
    : NameObj("イベント指揮"), mPowerStarEventKeeper(nullptr), mStageStateKeeper(nullptr), mPowerStarHolder(nullptr), mCometEventKeeper(nullptr),
      mTimeAttackEventKeeper(nullptr) {}

void EventDirector::init(const JMapInfoIter& rIter) {
    mPowerStarEventKeeper = new PowerStarEventKeeper();
    mStageStateKeeper = new StageStateKeeper();
    mPowerStarHolder = new PowerStarHolder("パワースター保持");
    mPowerStarHolder->initWithoutIter();
    mCometEventKeeper = new CometEventKeeper();
    mCometEventKeeper->init();
    mTimeAttackEventKeeper = new TimeAttackEventKeeper();
    mTimeAttackEventKeeper->init(mCometEventKeeper->isStartTimeLimitEvent());
}

namespace EventFunction {
    PowerStarEventKeeper* getPowerStarEventKeeper() { return MR::getSceneObj< EventDirector >(SceneObj_EventDirector)->mPowerStarEventKeeper; }

    StageStateKeeper* getStageStateKeeper() { return MR::getSceneObj< EventDirector >(SceneObj_EventDirector)->mStageStateKeeper; }

    PowerStarHolder* getPowerStarHolder() { return MR::getSceneObj< EventDirector >(SceneObj_EventDirector)->mPowerStarHolder; }

    CometEventKeeper* getCometEventKeeper() { return MR::getSceneObj< EventDirector >(SceneObj_EventDirector)->mCometEventKeeper; }

    TimeAttackEventKeeper* getTimeAttackEventKeeper() { return MR::getSceneObj< EventDirector >(SceneObj_EventDirector)->mTimeAttackEventKeeper; }

    bool isStartCometEvent(const char* pParam1) { return getCometEventKeeper()->isStartEvent(pParam1); }

    void startCometEvent() {
        getCometEventKeeper()->startCometEventIfExist();
        getTimeAttackEventKeeper()->startEventIfExecute();
    }

    void endCometEvent() {
        getCometEventKeeper()->endCometEvent();
        getTimeAttackEventKeeper()->endEvent();
    }
};  // namespace EventFunction

namespace MR {
    void declareEventPowerStar(const char* pParam1, s32 param2, bool param3) {
        EventFunction::getPowerStarEventKeeper()->declareStar(pParam1, "ゾーン無し", param2, param3);
    }

    void initEventSystemAfterPlacement() { EventFunction::getPowerStarEventKeeper()->initStarInfoTableAfterPlacement(); }

    void declareEventPowerStar(const NameObj* pParam1, s32 param2, bool param3) {
        const char* pVar1 = pParam1->mName;

        EventFunction::getPowerStarEventKeeper()->declareStar(pVar1, nullptr, param2, param3);
    }

    bool isSuccessEventPowerStar(const char* pParam1, s32 param2) { return EventFunction::getPowerStarEventKeeper()->isSuccess(pParam1, param2); }

    bool isGreenEventPowerStar(const char* pParam1, s32 param2) { return EventFunction::getPowerStarEventKeeper()->isGreen(pParam1, param2); }

    bool isRedEventPowerStar(const char* pParam1, s32 param2) { return EventFunction::getPowerStarEventKeeper()->isRed(pParam1, param2); }

    bool isGrandEventPowerStar(const char* pParam1, s32 param2) { return EventFunction::getPowerStarEventKeeper()->isGrand(pParam1, param2); }

    void appearEventPowerStar(const char* pParam1, s32 param2, const TVec3f* pParam3, bool param4, bool param5) {
        EventFunction::getPowerStarEventKeeper()->requestAppearPowerStar(pParam1, param2, pParam3, param4, param5);
    }

    bool isEndEventPowerStarAppearDemo(const char* pParam1) {
        int starId = EventFunction::getPowerStarEventKeeper()->findStarID(pParam1);

        return PowerStarFunction::isEndPowerStarAppearDemo(starId);
    }
};  // namespace MR
