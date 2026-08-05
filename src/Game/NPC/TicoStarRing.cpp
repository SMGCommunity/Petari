#include "Game/NPC/TicoStarRing.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/NPC/TicoGalaxy.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/NPCUtil.hpp"

namespace NrvTicoStarRing {
    NEW_NERVE(TicoStarRingNrvWait, TicoStarRing, Wait);
};  // namespace NrvTicoStarRing

TicoStarRing::TicoStarRing(const char* pName) : Tico(pName), mGalaxyName(), _194(), mTicoGalaxy() {
}

void TicoStarRing::init(const JMapInfoIter& rIter) {
    if (MR::isValidInfo(rIter)) {
        initBase(rIter, 2);
    } else {
        initBase(5);
    }

    initMessage("AstroGalaxy_TicoCommon000");

    if (MR::isValidInfo(rIter)) {
        s32 challengeGalaxy = 0;
        MR::getJMapInfoArg0NoInit(rIter, &challengeGalaxy);

        switch (challengeGalaxy) {
        case 0:
            mGalaxyName = "TamakoroExLv2Galaxy";
            break;
        case 1:
            mGalaxyName = "SurfingLv2Galaxy";
            break;
        case 2:
            mGalaxyName = "CubeBubbleExLv2Galaxy";
            break;
        case 3:
            mGalaxyName = "PeachCastleFinalGalaxy";
            break;
        }

        MR::setMessageArgToCurrentGalaxyName(mMsgCtrl, mGalaxyName);

        if (!MR::isAppearGalaxy(mGalaxyName)) {
            makeActorDead();
        }
    } else {
        mGalaxyName = "unknown";
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

void TicoStarRing::initialize(LiveActor* pTicoGalaxy, LiveActor* a2, const char* pGalaxyName) {
    mTicoGalaxy = pTicoGalaxy;
    _194 = a2;
    mGalaxyName = pGalaxyName;
    setBaseMtx(pTicoGalaxy->getBaseMtx());
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
