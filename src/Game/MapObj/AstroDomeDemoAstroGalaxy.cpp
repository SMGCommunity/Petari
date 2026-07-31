#include "Game/MapObj/AstroDomeDemoAstroGalaxy.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"

AstroDomeDemoAstroGalaxy::AstroDomeDemoAstroGalaxy(const char* pName) : MapObjActor(pName) {
}

void AstroDomeDemoAstroGalaxy::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    MapObjActorUtil::setupInitInfoTypical(&info, mObjectName);
    initialize(rIter, info);

    f32 frame;

    if (MR::hasGrandStar(5)) {
        frame = 2.0f;
    } else if (MR::hasGrandStar(3)) {
        frame = 1.0f;
    } else {
        frame = 0.0f;
    }

    MR::setBrkFrameAndStop(this, frame);
}
