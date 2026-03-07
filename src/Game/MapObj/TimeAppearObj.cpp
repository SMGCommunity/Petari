#include "Game/MapObj/TimeAppearObj.hpp"
#include "Game/Util.hpp"

namespace NrvTimeAppearObj {
    NEW_NERVE(TimeAppearObjNrvHide, TimeAppearObj, Hide);
    NEW_NERVE(TimeAppearObjNrvEnd, TimeAppearObj, End);
};  // namespace NrvTimeAppearObj

TimeAppearObj::TimeAppearObj(const char* pName) : MapObjActor(pName), mTimer(0x1E) {}

void TimeAppearObj::init(const JMapInfoIter& rIter) {
    MapObjActorInitInfo info;
    info.setupModelName("KoopaBattleMapStairTurn");
    info.setupSound(4);
    info.setupNerve(&NrvTimeAppearObj::TimeAppearObjNrvHide::sInstance);
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    initialize(rIter, info);
    MR::getJMapInfoArg0NoInit(rIter, &mTimer);
    makeActorDead();
}

void TimeAppearObj::exeHide() {
    if (MR::isStep(this, mTimer)) {
        setNerve(&NrvTimeAppearObj::TimeAppearObjNrvEnd::sInstance);
    }
}

void TimeAppearObj::exeEnd() {
    if (MR::isFirstStep(this)) {
        if (MR::isRegisteredEffect(this, "Appear")) {
            MR::emitEffect(this, "Appear");
        }

        if (MR::isEqualString(mObjectName, "KoopaBattleMapStairTurn")) {
            MR::startSound(this, "SE_OJ_KP_BTL_ST_TURN_APPEAR", -1, -1);
        }

        MR::showModel(this);
        MR::validateCollisionParts(this);
    }
}

void TimeAppearObj::appear() {
    MapObjActor::appear();
    MR::hideModel(this);
    MR::invalidateCollisionParts(this);
    setNerve(&NrvTimeAppearObj::TimeAppearObjNrvHide::sInstance);
}

TimeAppearObj::~TimeAppearObj() {}
