#include "Game/NPC/TicoShop.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/MapObj/BenefitItemLifeUp.hpp"
#include "Game/MapObj/BenefitItemObj.hpp"
#include "Game/NPC/NPCActorItem.hpp"
#include "Game/NPC/TalkMessageFunc.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    static const s32 sStarPieceNeed = 30;
    static const f32 sMouthSize = 30.0f;
    static const f32 sSensorSize = 100.0f;
    static const f32 sSensorOffset = 100.0f;
    static const f32 sShadowLength = 500.0f;
};  // namespace

namespace NrvTicoShop {
    NEW_NERVE(TicoShopNrvDemo, TicoShop, Demo);
};  // namespace NrvTicoShop

TicoShop::TicoShop(const char* pName) : TicoComet(pName), mOneUp(), mLifeUp(), mChoseOneUp(), mChoseLifeUp() {
}

void TicoShop::kill() {
    if (mOneUp != nullptr && mChoseOneUp) {
        MR::appearKinokoOneUpPop(mOneUp, getBaseMtx(), 15.0f);
    }

    if (mLifeUp != nullptr && mChoseLifeUp) {
        MR::appearKinokoSuper(mLifeUp, getBaseMtx(), 15.0f);
    }

    NPCActor::kill();
}

void TicoShop::init(const JMapInfoIter& rIter) {
    NPCActorCaps caps("TicoShop");
    NPCActorItem item("TicoShop");
    caps.setDefault();
    caps.mSensorSize = ::sSensorSize;
    caps.mSensorOffset.y = ::sSensorOffset;
    caps._5D = true;
    caps.mSoundSize = 6;
    caps.mSensorMax = 2;
    caps.mSensorJoint = "Center";
    caps._6C = "Center";
    initialize(rIter, caps);
    MR::addHitSensorAtJointEye(this, "Mouth", "Mouth", 8, ::sMouthSize, TVec3f(0.0f, 0.0f, 0.0f));
    MR::getNPCItemData(&item, 0);
    equipment(item, false);

    if (_94 != nullptr) {
        MR::startAction(_94, "Shake");
    }

    if (_98 != nullptr) {
        MR::startAction(_98, "Shake");
    }

    s32 starPieceNeed = ::sStarPieceNeed;
    MR::getJMapInfoArg0NoInit(rIter, &starPieceNeed);
    initStarPiece(starPieceNeed);
    initMessage("Common_TicoShop000");
    MR::setMessageArg(mMsgCtrl, _194);
    MR::registerBranchFunc(mMsgCtrl, TalkMessageFunc(this, &TicoShop::branchFunc));
    MR::registerEventFunc(mMsgCtrl, TalkMessageFunc(this, &TicoShop::eventFunc));
    MR::setDistanceToTalk(mMsgCtrl, 350.0f);
    _180 = &NrvTicoShop::TicoShopNrvDemo::sInstance;
    mOneUp = MR::createKinokoOneUp();
    mLifeUp = MR::createKinokoSuper();
    MR::setShadowDropLength(mOneUp, nullptr, ::sShadowLength);
    MR::setShadowDropLength(mLifeUp, nullptr, ::sShadowLength);
    MR::startBva(this, "Small0");
    MR::startBrk(this, "Metamorphosis");
    MR::setBrkFrameAndStop(this, 0.0f);
    MR::setClippingFar200m(this);
    TicoEat::init(rIter);
    _17C.set(0, 0xE6, 0xFF, 0xFF);
}

bool TicoShop::branchFunc(u32) {
    return hasEnoughStarPiece();
}

bool TicoShop::eventFunc(u32 var) {
    if (var < 3) {
        return TicoComet::eventFunc(var);
    }

    if (var == 3) {
        mChoseLifeUp = true;
    }

    if (var == 4) {
        mChoseOneUp = true;
    }

    return true;
}

void TicoShop::startReactionSound() {
    if (_D8) {
        MR::startSound(this, "SE_SM_NPC_TRAMPLED");
        MR::startSound(this, "SE_SV_TICOSHOP_TRAMPLED");
    }

    if (isPointingSe()) {
        MR::startDPDHitSound();
        MR::startSound(this, "SE_SV_TICOSHOP_POINT");
    }

    if (_D9) {
        MR::startSound(this, "SE_SM_TICOSHOP_SPIN");
    }

    if (_DB) {
        MR::startSound(this, "SE_SV_TICOSHOP_STAR_PIECE_HIT");
    }
}

void TicoShop::exeDemo() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Demo");
        MR::startSound(this, "SE_SM_TICOFAT_META_ITEM");
        MR::startSound(this, "SE_SM_TICOFAT_META");
    }

    if (MR::isBckStopped(this)) {
        MR::startSound(this, "SE_SM_METAMORPHOSE_SMOKE");
        kill();
    }
}
