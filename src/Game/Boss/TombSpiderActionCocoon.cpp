#include "Game/Boss/TombSpiderActionCocoon.hpp"
#include "Game/Boss/TombSpider.hpp"
#include "Game/Boss/TombSpiderActionBase.hpp"
#include "Game/Boss/TombSpiderFunction.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    static const s32 sBreakStep = 3;
};  // namespace

namespace NrvTombSpiderActionCocoon {
    NEW_NERVE(TombSpiderActionCocoonNrvWait, TombSpiderActionCocoon, Wait);
    NEW_NERVE(TombSpiderActionCocoonNrvBreak, TombSpiderActionCocoon, Break);
};  // namespace NrvTombSpiderActionCocoon

TombSpiderActionCocoon::TombSpiderActionCocoon(TombSpider* pParent) : TombSpiderActionBase(pParent, "まゆ状態[トゥームスパイダー]") {
}

void TombSpiderActionCocoon::init() {
    initNerve(GET_NERVE(TombSpiderActionCocoon, TombSpiderActionCocoonNrvWait));
    TombSpiderActionBase::init();
}

void TombSpiderActionCocoon::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isPlayerInBind()) {
        MR::sendMsgPush(pReceiver, pSender);
    }
}

bool TombSpiderActionCocoon::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_IS_BROKEN) {
        if (mIsDead || isNerve(GET_NERVE(TombSpiderActionCocoon, TombSpiderActionCocoonNrvBreak))) {
            return true;
        }
        return false;
    }

    if (msg == ACTMES_SLING_SHOOT_ATTACK) {
        if (!TombSpiderFunction::isAttackDirection(pSender, pReceiver)) {
            return false;
        }

        if (TombSpiderFunction::isSpringAttacker(pSender)) {
            if (isNerve(GET_NERVE(TombSpiderActionCocoon, TombSpiderActionCocoonNrvWait))) {
                setNerve(GET_NERVE(TombSpiderActionCocoon, TombSpiderActionCocoonNrvBreak));
                return true;
            }
        } else {
            MR::shakeCameraNormal();
            MR::startSound(TombSpiderFunction::getCocoon(mParent), "SE_OJ_TSPIDER_COCOON_BOUND");
            return true;
        }
    }

    return false;
}

void TombSpiderActionCocoon::exeWait() {
    if (MR::isFirstStep(this)) {
        mParent->getSensor("cocoon")->validate();
        MR::startBck(TombSpiderFunction::getCocoon(mParent), "Wait", nullptr);
    }
}

void TombSpiderActionCocoon::exeBreak() {
    if (MR::isFirstStep(this)) {
        MR::startSound(TombSpiderFunction::getCocoon(mParent), "SE_OJ_TSPIDER_COCOON_BREAK");
    }

    if (MR::isStep(this, ::sBreakStep)) {
        kill();
    }
}
