#include "Game/MapObj/MapObjActor.h"
#include "Game/Util.h"

void MapObjActor::init(const JMapInfoIter &rIter) {
    if (mObjectName) {
        return;
    }

    MR::getObjectName(&mObjectName, rIter);
}

bool MapObjActor::isObjectName(const char *pName) const {
    return MR::isEqualString(pName, mObjectName);
}

void MapObjActor::connectToScene(const MapObjActorInitInfo &rInfo) {
    if (rInfo.mConnectToScene) {
        if (MR::isExistCollisionResource(this, mObjectName)) {
            s32 type = rInfo._5C;

            if (type == 1) {
                MR::connectToSceneCollisionMapObjStrongLight(this);
            }
            else if (type == 2) {
                MR::connectToSceneCollisionMapObjWeakLight(this);
            }
            else {
                MR::connectToSceneCollisionMapObj(this);
            }
        }
        else if (rInfo._5C == 1) {
            MR::connectToSceneMapObjStrongLight(this);
        }
        else {
            MR::connectToSceneMapObj(this);
        }
    }
}

void MapObjActor::initCaseUseSwitchA(const MapObjActorInitInfo &) {
    setNerve(mWaitNrv);
}

void MapObjActor::initCaseNoUseSwitchA(const MapObjActorInitInfo &) {

}

void MapObjActor::startClipped() {
    tryEmitWaitEffect();
    LiveActor::startClipped();
}

void MapObjActor::endClipped() {
    LiveActor::endClipped();
    tryDeleteWaitEffect();
}