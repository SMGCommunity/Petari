#include "Game/NPC/KinopioAstro.hpp"
#include "Game/Demo/AstroDemoFunction.hpp"
#include "Game/Demo/DemoFunction.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/NPC/NPCActorItem.hpp"
#include "Game/NPC/TalkMessageFunc.hpp"
#include "Game/NWC24/ReceiverTagMail.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Screen/GameSceneLayoutHolder.hpp"
#include "Game/Screen/LuigiLetter.hpp"
#include "Game/Screen/PeachLetter.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/Util.hpp"

namespace {
    const s32 sMessangerOneUpNum = 5;
    const s32 sMessangerOneUpExNum = 20;
    const f32 sMessangerTalkDistance = 3000.0f;
    const f32 sGotMailSePlayAreaRadius = 1500.0f;
    const char* sStaffLetterID = "StaffFinalLetter_000";
    const char* sSenderID = "WiiMessageTitle";
};  // namespace

u32 JKRArchive::getExpandedResSize(const void* pResource) const {
    return getResSize(pResource);
}

void KinopioAstro::makeArchiveList(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter) {
    NPCActorItem item("Kinopio");

    s32 arg1 = 0;
    MR::getJMapInfoArg1NoInit(rIter, &arg1);

    if (arg1 == 2) {
        if (!MR::isEqualStageName("PeachCastleFinalGalaxy")) {
            if (MR::isAnyPlayerLeftSupply()) {
                PeachLetter::makeArchiveList(pCollector, rIter);
            }

            if (MR::isLuigiLetterArrivalAtMessenger()) {
                LuigiLetter::makeArchiveListForNPC(pCollector, rIter);
            }
        } else {
            pCollector->addArchive("AllCompleteImage");
        }

        MR::getNPCItemData(&item, 3);
        NPCActor::addArchive(pCollector, item);
    } else {
        s32 arg7 = -1;
        MR::getJMapInfoArg7NoInit(rIter, &arg7);

        if (MR::getNPCItemData(&item, arg7)) {
            NPCActor::addArchive(pCollector, item);
        }
    }
}

KinopioAstro::KinopioAstro(const char* pName)
    : Kinopio(pName), mLuigiLetter(), mPeachLetter(), mLetterIconTalkCtrl(), _194(), _195(), mIsPlayerLuigi(), _197(), _198(), _19C() {
}

bool KinopioAstro::dispLetter(LayoutActor* pLayout) const {
    if (pLayout == nullptr) {
        return true;
    }

    if (MR::isDead(pLayout)) {
        pLayout->appear();
    }

    pLayout->movement();
    return MR::isDead(pLayout);
}

bool KinopioAstro::isDispLetterIcon() const {
    if (_198 != nullptr) {
        if (MR::isStageStatePowerStarAppeared() && !_197) {
            return true;
        }

        return false;
    }

    return _195 || _194;
}

void KinopioAstro::createLetterIcon(const JMapInfoIter& rIter) {
    mLetterIconTalkCtrl = MR::createTalkCtrlDirect(this, rIter, "Common_Kinopio000", TVec3f(0.0f, 122.0f, 0.0f), nullptr);
    MR::setDistanceToTalk(mLetterIconTalkCtrl, ::sMessangerTalkDistance);
}

bool KinopioAstro::sendLetter() {
    if (!_197) {
        u32 size;
        const char* pImage = MR::isPlayerLuigi() ? "AllCompleteImage2.bin" : "AllCompleteImage1.bin";

        size = _19C->mArchive->getExpandedResSize(_19C->mArchive->getResource(pImage));
        _198->send(reinterpret_cast< u8* >(_19C->mFileInfoTable->getRes(pImage)), size, false);
        _197 = true;
    }

    _198->update();
    return _198->isDone();
}

bool KinopioAstro::branchFunc(u32) {
    return !_198->isError();
}

bool KinopioAstro::eventFunc(u32 letterEvent) {
    if (letterEvent == LetterEvent_LuigiLetter) {
        if (dispLetter(mLuigiLetter)) {
            _195 = false;
            MR::onGameEventFlagGetLuigiLetter();
            return true;
        }

        return false;
    } else if (letterEvent == LetterEvent_PeachLetterNormal) {
        if (dispLetter(mPeachLetter)) {
            _194 = false;
            MR::offAllPlayerLeftSupply();
            return true;
        }

        return false;
    } else if (letterEvent == LetterEvent_PeachLetter1Ups) {
        if (mIsPlayerLuigi) {
            for (s32 i = 0; i < ::sMessangerOneUpExNum; i++) {
                MR::incPlayerLeft();
            }

            MR::getGameSceneLayoutHolder()->requestOneUp(::sMessangerOneUpExNum);
            MR::startSystemSE("SE_SY_5UP");
        } else {
            for (s32 i = 0; i < ::sMessangerOneUpNum; i++) {
                MR::incPlayerLeft();
            }

            MR::getGameSceneLayoutHolder()->requestOneUp(::sMessangerOneUpNum);
            MR::startSystemSE("SE_SY_5UP");
        }

        return true;
    } else if (letterEvent == LetterEvent_3) {
        return true;
    } else if (letterEvent == LetterEvent_SendLetter) {
        return sendLetter();
    } else if (letterEvent == LetterEvent_OnMsgLedPattern) {
        MR::onMsgLedPattern();
        return true;
    } else if (letterEvent == LetterEvent_OffMsgLedPattern) {
        MR::offMsgLedPattern();
        return true;
    }

    return true;
}

void KinopioAstro::startDemo() {
    MR::moveCoordAndTransToRailStartPoint(this);
    tryPushNullNerve();
    turnToPlayer(180.0f);
}

void KinopioAstro::endDemo() {
    popNerve();
}

void KinopioAstro::init(const JMapInfoIter& rIter) {
    Kinopio::init(rIter);
    if (mMsgCtrl != nullptr) {
        MR::registerEventFunc(getMsgCtrl(), TalkMessageFunc(this, &KinopioAstro::eventFunc));
        MR::registerBranchFunc(getMsgCtrl(), TalkMessageFunc(this, &KinopioAstro::branchFunc));
    }

    if (MR::isEqualStageName("PeachCastleFinalGalaxy")) {
        _198 = new ReceiverTagMail("StaffLetter", ::sStaffLetterID, ::sSenderID);
        _19C = MR::createAndAddResourceHolder("AllCompleteImage.arc");
        createLetterIcon(rIter);
        return;
    }

    if (!MR::isKinopioExplorerRescued()) {
        makeActorDead();
    }

    if (mObjArg0 == 2) {
        if (MR::isDemoCast(this, "ルイージ失踪デモ")) {
            TVec3f followOffset(MR::getMessageBalloonFollowOffset(mMsgCtrl));
            TalkMessageCtrl* pMsgCtrl = MR::createTalkCtrlDirectOnRootNodeAutomatic(this, rIter, "AstroGalaxy_Kinopio100", followOffset, nullptr);
            MR::registerEventFunc(pMsgCtrl, TalkMessageFunc(this, &KinopioAstro::eventFunc));
            DemoFunction::registerDemoTalkMessageCtrlDirect(this, pMsgCtrl, "ルイージ失踪デモ");
            MR::registerDemoActionFunctor(this, MR::Functor(this, &KinopioAstro::startDemo), "開始");
            MR::registerDemoActionFunctor(this, MR::Functor(this, &KinopioAstro::endDemo), "終了");
        }

        if (MR::isAnyPlayerLeftSupply()) {
            mPeachLetter = new PeachLetter("ピーチ姫からの手紙");
            mPeachLetter->initWithoutIter();
            _194 = true;
            if (MR::isLuigiLeftSupply()) {
                mIsPlayerLuigi = true;
            }

            if (mIsPlayerLuigi) {
                MR::setMessageArg(mMsgCtrl, ::sMessangerOneUpExNum);
            } else {
                MR::setMessageArg(mMsgCtrl, ::sMessangerOneUpNum);
            }
        }

        if (MR::isLuigiLetterArrivalAtMessenger()) {
            mLuigiLetter = MR::createLuigiLetterForTalk();
            mLuigiLetter->initWithoutIter();
            _195 = true;
        }

        if (mGoodsIndex != 3) {
            NPCActorItem npcItems("Kinopio");
            MR::getNPCItemData(&npcItems, 3);
            _94 = MR::createNPCGoods(this, npcItems.mGoods0, npcItems.mGoodsJoint0);
            _98 = MR::createNPCGoods(this, npcItems.mGoods1, npcItems.mGoodsJoint1);
        }

        if (_94 != nullptr) {
            MR::registerDemoSimpleCastAll(_94);
        }

        if (_98 != nullptr) {
            MR::registerDemoSimpleCastAll(_98);
        }
    }

    if (_194 || _195) {
        createLetterIcon(rIter);
    }

    AstroDemoFunction::tryRegisterDemoForLuigiAndKinopio(this, rIter);
}

void KinopioAstro::control() {
    if (isDispLetterIcon() && MR::calcCameraDistanceZ(mPosition) < ::sMessangerTalkDistance && MR::tryTalkNearPlayer(mLetterIconTalkCtrl) &&
        MR::calcDistanceToPlayer(this) <= ::sGotMailSePlayAreaRadius) {
        MR::startLevelSound(this, "SE_SM_LV_KINOMES_GOT_MAIL");
    }

    NPCActor::control();
}
