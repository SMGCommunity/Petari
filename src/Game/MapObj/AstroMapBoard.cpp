#include "Game/MapObj/AstroMapBoard.hpp"
#include "Game/MapObj/AstroDemoFunction.hpp"

namespace {
    static const char* cDummyTexName = "MapDummy";
};

namespace NrvAstroMapBoard {
    NERVE_DECL_NULL(AstroMapBoardNrvWait);
};

AstroMapBoard::AstroMapBoard(const char *pName) : MapObjActor(pName) {

}

void AstroMapBoard::init(const JMapInfoIter &rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    info.setupPrepareChangeDummyTexture(cDummyTexName);
    info.setupNerve(NrvAstroMapBoard::AstroMapBoardNrvWait::get());
    info.setupFarClipping(-1.0f);
    info.setupNoAppearRiddleSE();
    initialize(rIter, info);
    AstroDemoFunction::tryRegisterGrandStarReturnAndSimpleCast(this, rIter);
    AstroDemoFunction::tryRegisterDemo(this, "ロゼッタトーチの炎説明デモ", rIter);
    AstroDemoFunction::tryRegisterDemo(this, "ロゼッタトーチの炎進捗デモ", rIter);

    if (MR::isButlerMapAppear()) {
        makeActorAppeared();
    }
    else {
        makeActorDead();
    }
}

void AstroMapBoard::connectToScene(const MapObjActorInitInfo &rInfo) {
    MR::connectToSceneIndirectMapObj(this);
}

void AstroMapBoard::initAfterPlacement() {
    if (MR::isButlerMapAppear()) {
        MR::changeModelDataTexAll(this, cDummyTexName, *MR::getGalaxyMapResTIMG());
    }
}

AstroMapBoard::~AstroMapBoard() {

}

namespace NrvAstroMapBoard {
    INIT_NERVE(AstroMapBoardNrvWait);
};
