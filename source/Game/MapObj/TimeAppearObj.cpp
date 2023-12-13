#include "Game/MapObj/TimeAppearObj.hpp"
#include "Game/Util.hpp"

TimeAppearObj::TimeAppearObj(const char *pName) : MapObjActor(pName), mTimer(0x1E) {

}

void TimeAppearObj::init(const JMapInfoIter &rIter) {
    MapObjActorInitInfo info;
    info.setupModelName("KoopaBattleMapStairTurn");
    info.setupSound(4);
    info.setupNerve(&NrvTimeAppearObj::TimeAppearObjNrvHide::sInstance);
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    initialize(rIter, info);
    MR::getJMapInfoArg0NoInit(rIter, &mTimer);
    makeActorDead();
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

TimeAppearObj::~TimeAppearObj() {

}

namespace NrvTimeAppearObj {
    INIT_NERVE(TimeAppearObjNrvHide);
    INIT_NERVE(TimeAppearObjNrvEnd);

    void TimeAppearObjNrvEnd::execute(Spine *pSpine) const {
        TimeAppearObj* obj = reinterpret_cast<TimeAppearObj*>(pSpine->mExecutor);
        obj->exeEnd();
    }

    void TimeAppearObjNrvHide::execute(Spine *pSpine) const {
        TimeAppearObj* obj = reinterpret_cast<TimeAppearObj*>(pSpine->mExecutor);
        if (MR::isStep(obj, obj->mTimer)) {
            obj->setNerve(&NrvTimeAppearObj::TimeAppearObjNrvEnd::sInstance);
        }
    }
};