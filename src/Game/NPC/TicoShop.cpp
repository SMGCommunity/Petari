#include "Game/NPC/TicoShop.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/NPC/NPCActorItem.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace NrvTicoShop {
    NEW_NERVE(TicoShopNrvDemo, TicoShop, Demo);
};

TicoShop::TicoShop(const char* pName) : TicoComet(pName) {
    mOneUp = nullptr;
    mLifeUp = nullptr;
    mChoseOneUp = false;
    mChoseLifeUp = false;
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
    item._4 = "";
    item._8 = "";
    item.mArchive = "";
    item._10 = "";
    caps.setDefault();
    caps.mSensorSize = 100.0f;
    caps.mSensorOffset.y = 100.0f;
    caps.mUseShadow = true;
    caps._30 = 6;
    caps._58 = 2;
    caps._44 = "Center";
    caps._6C = "Center";
    initialize(rIter, caps);
    MR::addHitSensorAtJointEye(this, "Mouth", "Mouth", 8, 30.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::getNPCItemData(&item, 0);
    equipment(item, false);

    if (_94 != nullptr) {
        MR::startAction(_94, "Shake");
    }

    if (_98 != nullptr) {
        MR::startAction(_98, "Shake");
    }

    s32 starbits = 30;
    MR::getJMapInfoArg0NoInit(rIter, &starbits);
    initStarPiece(starbits);
    initMessage("Common_TicoShop000");
    MR::setMessageArg(mMsgCtrl, _194);
    MR::registerBranchFunc(mMsgCtrl, TalkMessageFunc(this, &TicoShop::branchFunc));
    MR::registerEventFunc(mMsgCtrl, TalkMessageFunc(this, &TicoShop::eventFunc));
    MR::setDistanceToTalk(mMsgCtrl, 350.0f);
    _180 = &NrvTicoShop::TicoShopNrvDemo::sInstance;
    mOneUp = MR::createKinokoOneUp();
    mLifeUp = MR::createKinokoSuper();
    MR::setShadowDropLength(mOneUp, nullptr, 500.0f);
    MR::setShadowDropLength(mLifeUp, nullptr, 500.0f);
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
        MR::startSound(this, "SE_SM_NPC_TRAMPLED", -1, -1);
        MR::startSound(this, "SE_SV_TICOSHOP_TRAMPLED", -1, -1);
    }

    if (isPointingSe()) {
        MR::startDPDHitSound();
        MR::startSound(this, "SE_SV_TICOSHOP_POINT", -1, -1);
    }

    if (_D9) {
        MR::startSound(this, "SE_SM_TICOSHOP_SPIN", -1, -1);
    }

    if (_DB) {
        MR::startSound(this, "SE_SV_TICOSHOP_STAR_PIECE_HIT", -1, -1);
    }
}

void TicoShop::exeDemo() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Demo");
        MR::startSound(this, "SE_SM_TICOFAT_META_ITEM", -1, -1);
        MR::startSound(this, "SE_SM_TICOFAT_META", -1, -1);
    }

    if (MR::isBckStopped(this)) {
        MR::startSound(this, "SE_SM_METAMORPHOSE_SMOKE", -1, -1);
        kill();
    }
}

TicoShop::~TicoShop() {}
