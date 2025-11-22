#include "Game/NPC/TicoGalaxy.hpp"
#include "Game/NPC/TicoStarRing.hpp"
#include "Game/MapObj/StarPieceDirector.hpp"
#include "Game/System/GameEventFlagTable.hpp"
#include "Game/Util.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"

namespace {
    static const char* sInfoMessageID = "InformationGalaxy";
};

TicoGalaxy::TicoGalaxy(const char* pName)
    : TicoFat(pName) {
    mGalaxyName = nullptr;
}

void TicoGalaxy::init(const JMapInfoIter& rIter) {
    TicoFat::init(rIter);
    _1F0 = 2;
    if (MR::isOnTicoGalaxyAlreadyTalk(_1EC)) {
        MR::forwardNode(mMsgCtrl);
        MR::forwardNode(mMsgCtrl);
    }

    mStarRing = new TicoStarRing("スターリングチコ");
    mStarRing->initWithoutIter();
}

void TicoGalaxy::initAfterPlacement() {
    mStarRing->initialize(this, MR::getPairedGroupMember(this), mGalaxyName);
    TicoFat::initAfterPlacement();
}

void TicoGalaxy::shootStarPiece() {
    s32 v5 = 5;
    if (_1E0 <= 5) {
        v5 = _1E0;
    }

    if (MR::giftStarPieceToTarget(getSensor("Mouth"), v5)) {
        mCurrentFed += v5;
        MR::tryRumblePadVeryWeak(this, 0);
    }
}

void TicoGalaxy::startEat() {
    TicoFat::startEat();
    MR::setTicoGalaxyAlreadyTalk(_1EC, true);
}

bool TicoGalaxy::enableAppear() {
    return MR::isAppearGalaxy(mGalaxyName);
}

void TicoGalaxy::initStarPieceSaveData(const JMapInfoIter& rIter) {
    MR::getJMapInfoArg7WithInit(rIter, &_1EC);
    mGalaxyName = GameEventFlagTable::getExclamationGalaxyNameFromIndex(_1EC);
    _1E4 = GameEventFlagTable::getStarPieceNumToOpenExclamationGalaxy(mGalaxyName);
    s32 starPieceNum = MR::getStarPieceNumGivingToTicoGalaxy(_1EC);
    _1E0 = (_1E4 - starPieceNum <= 0) ? 0 : _1E4 - starPieceNum;
}

void TicoGalaxy::addStarPieceSaveData(s32 num) {
    MR::addStarPieceGivingToTicoGalaxy(_1EC, num);
}

void TicoGalaxy::disappear(bool a1) {
    if (a1) {
        MR::onGameEventFlagGalaxyOpen(mGalaxyName);
        MR::callAppearAllGroupMember(this);
    } else {
        mStarRing->appear();
    }
}

void TicoGalaxy::appearInformation() const {
    MR::appearInformationMessage(MR::getGameMessageDirect(sInfoMessageID), true);
    MR::setInformationMessageReplaceString(MR::getGalaxyNameShortOnCurrentLanguage(mGalaxyName), 0);
}

TicoGalaxy::~TicoGalaxy() {
}
