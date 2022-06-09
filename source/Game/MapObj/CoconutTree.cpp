#include "Game/MapObj/CoconutTree.h"
#include "Game/Util.h"

CoconutTree::CoconutTree(const char *pName) : LiveActor(pName) {

}

CoconutTree::~CoconutTree() {

}

void CoconutTree::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("CoconutTree", NULL, false);
    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);
    MR::initCollisionParts(this, "CoconutTree", getSensor("body"), NULL);
    MR::setClippingTypeSphere(this, 2000.0f);
    makeActorAppeared();
}