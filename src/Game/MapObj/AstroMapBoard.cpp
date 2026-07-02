#include "Game/MapObj/AstroMapBoard.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/AstroDemoFunction.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Screen/GalaxyMapController.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace {
    static const char* cDummyTexName = "MapDummy";
};  // namespace

namespace NrvAstroMapBoard {
    NEW_NERVE(AstroMapBoardNrvWait, AstroMapBoard, Wait);
};  // namespace NrvAstroMapBoard

AstroMapBoard::AstroMapBoard(const char* pName) : MapObjActor(pName) {
}

void AstroMapBoard::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    info.setupPrepareChangeDummyTexture(::cDummyTexName);
    info.setupNerve(&NrvAstroMapBoard::AstroMapBoardNrvWait::sInstance);
    info.setupFarClipping(-1.0f);
    info.setupNoAppearRiddleSE();
    initialize(rIter, info);
    AstroDemoFunction::tryRegisterGrandStarReturnAndSimpleCast(this, rIter);
    AstroDemoFunction::tryRegisterDemo(this, "ロゼッタトーチの炎説明デモ", rIter);
    AstroDemoFunction::tryRegisterDemo(this, "ロゼッタトーチの炎進捗デモ", rIter);

    if (MR::isButlerMapAppear()) {
        makeActorAppeared();
    } else {
        makeActorDead();
    }
}

void AstroMapBoard::connectToScene(const MapObjActorInitInfo& rInfo) {
    MR::connectToSceneIndirectMapObj(this);
}

void AstroMapBoard::initAfterPlacement() {
    if (MR::isButlerMapAppear()) {
        MR::changeModelDataTexAll(this, ::cDummyTexName, *MR::getGalaxyMapResTIMG());
    }
}

void AstroMapBoard::exeWait() {
}

AstroMapBoard::~AstroMapBoard() {
}
