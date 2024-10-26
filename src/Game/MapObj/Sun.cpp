#include "Game/MapObj/Sun.hpp"
#include "Game/Util.hpp"

Sun::Sun(const char *pName) : LiveActor(pName) {

}

Sun::~Sun() {

}

void Sun::init(const JMapInfoIter &rIter) {
    initModelManagerWithAnm("Sun", nullptr, false);
    MR::connectToSceneSun(this);
    MR::invalidateClipping(this);
    makeActorAppeared();
}