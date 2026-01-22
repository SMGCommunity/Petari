#include "Game/MapObj/StarPieceSpot.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

StarPieceSpot::StarPieceSpot(const char* pName) : LiveActor(pName) {}

void StarPieceSpot::emit() {
    MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
    MR::appearStarPiece(this, mPosition, MR::getDeclareRemnantStarPieceCount(this), 15.0f, 40.0f, false);
}

void StarPieceSpot::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);

    s32 arg0 = -1;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);
    MR::declareStarPiece(this, arg0);
    
    initSound(1, false);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::listenStageSwitchOnAppear(this, MR::Functor_Inline(this, &emit));
        kill();
    } else {
        initHitSensor(1);
        MR::addHitSensorMapObjSimple(this, "body", 8, 150.0f, TVec3f(0.0f, 0.0f, 0.0f));
        appear();
    }
}

bool StarPieceSpot::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerSpinAttack(msg)) {
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
        MR::appearStarPiece(this, mPosition, MR::getDeclareRemnantStarPieceCount(this), 15.0f, 40.0f, false);
        kill();
        return false;
    } else {
        return false;
    }
}
