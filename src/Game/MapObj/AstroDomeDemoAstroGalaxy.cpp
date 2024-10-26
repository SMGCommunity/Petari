#include "Game/MapObj/AstroDomeDemoAstroGalaxy.hpp"

AstroDomeDemoAstroGalaxy::AstroDomeDemoAstroGalaxy(const char *pName) : MapObjActor(pName) {
    
}

void AstroDomeDemoAstroGalaxy::init(const JMapInfoIter &rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    MapObjActorUtil::setupInitInfoTypical(&info, mObjectName);
    initialize(rIter, info);

    f32 color;
    
    if (MR::hasGrandStar(5)) {
        color = 2.0f;
    }
    else if (MR::hasGrandStar(3)) {
        color = 1.0f;
    }
    else {
        color = 0.0f;
    }

    MR::setBrkFrameAndStop(this, color);
}

AstroDomeDemoAstroGalaxy::~AstroDomeDemoAstroGalaxy() {

}
