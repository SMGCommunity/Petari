#include "Game/NPC/TicoStarRing.hpp"
#include "Game/NPC/TicoGalaxy.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/NPCUtil.hpp"

namespace NrvTicoStarRing {
    NEW_NERVE(TicoStarRingNrvWait, TicoStarRing, Wait);
};

TicoStarRing::TicoStarRing(const char *pName) : Tico(pName) {
    mGalaxy = nullptr;
    _194 = nullptr;
    mTicoGalaxy = nullptr;
}

void TicoStarRing::init(const JMapInfoIter &rIter) {
    if (MR::isValidInfo(rIter)) {
        initBase(rIter, 2);
    }
    else {
        initBase(5);
    }

    initMessage("AstroGalaxy_TicoCommon000");

    if (MR::isValidInfo(rIter)) {
        s32 galaxy = 0;
        MR::getJMapInfoArg0NoInit(rIter, &galaxy);
        switch (galaxy) {
            case 0:
                mGalaxy = "TamakoroExLv2Galaxy";
                break;
            case 1:
                mGalaxy = "SurfingLv2Galaxy";
                break;
            case 2:
                mGalaxy = "CubeBubbleExLv2Galaxy";
                break;
            case 3:
                mGalaxy = "PeachCastleFinalGalaxy";
                break;
        }

        MR::setMessageArgToCurrentGalaxyName(mMsgCtrl, mGalaxy);
        if (!MR::isAppearGalaxy(mGalaxy)) {
            makeActorDead();
        }
    }
    else {
        mGalaxy = "unknown";
    }

    setNerve(&NrvTicoStarRing::TicoStarRingNrvWait::sInstance);
}

void TicoStarRing::kill() {
    NPCActor::kill();
    if (_194 != nullptr) {
        _194->appear();
    }

    if (mTicoGalaxy != nullptr) {
        if (MR::isValidSwitchA(mTicoGalaxy)) {
            MR::onSwitchA(mTicoGalaxy);
        }
    }
}

void TicoStarRing::initialize(LiveActor *pGalaxy, LiveActor *a2, const char *pGalaxyName) {
    mTicoGalaxy = pGalaxy;
    _194 = a2;
    mGalaxy = pGalaxyName;
    setBaseMtx(pGalaxy->getBaseMtx());
    mPosition.set(a2->mPosition);
    makeActorDead();
    MR::setMessageArgToCurrentGalaxyName(mMsgCtrl, pGalaxyName);
}

void TicoStarRing::exeWait() {
    if (!tryReaction() && MR::tryTalkNearPlayerAtEndAndStartTalkAction(this)) {
        if (MR::tryTalkSelectLeft(mMsgCtrl)) {
            setNerveMeta();
        }
    }
}

TicoStarRing::~TicoStarRing() {
    
}
