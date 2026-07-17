#include "Game/MapObj/StarPieceSpot.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    static const f32 sRandomPieceValue = 15.0f;
    static const f32 sLaunchYVel = 40.0f;
};  // namespace

StarPieceSpot::StarPieceSpot(const char* pName) : LiveActor(pName) {
}

void StarPieceSpot::emit() {
    MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");
    MR::appearStarPiece(this, mPosition, MR::getDeclareRemnantStarPieceCount(this), ::sRandomPieceValue, ::sLaunchYVel, false);
}

void StarPieceSpot::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);

    s32 arg0 = -1;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);
    MR::declareStarPiece(this, arg0);

    initSound(1, false);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::listenStageSwitchOnAppear(this, MR::Functor_Inline(this, &StarPieceSpot::emit));
        kill();
    } else {
        initHitSensor(1);
        MR::addHitSensorMapObjSimple(this, "body", 8, 150.0f, TVec3f(0.0f, 0.0f, 0.0f));
        appear();
    }
}

bool StarPieceSpot::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerSpinAttack(msg)) {
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");
        MR::appearStarPiece(this, mPosition, MR::getDeclareRemnantStarPieceCount(this), ::sRandomPieceValue, ::sLaunchYVel, false);
        kill();
        return false;
    } else {
        return false;
    }
}
