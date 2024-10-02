#include "Game/AreaObj/GlaringLightArea.hpp"
#include "Game/AudioLib/AudSoundObject.hpp"
#include "Game/Util.hpp"

GlaringLightAreaMgr::GlaringLightAreaMgr(s32 type, const char *pName) : AreaObjMgr(type, pName) {

}

GlaringLightArea::GlaringLightArea(int type, const char *pName) : AreaObj(type, pName) {
    mSound = nullptr;
    mPos.x = 0.0f;
    mPos.y = 0.0f;
    mPos.z = 0.0f;
}

GlaringLightArea::~GlaringLightArea() {

}

GlaringLightAreaMgr::~GlaringLightAreaMgr() {
    
}

void GlaringLightArea::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);

    TVec3f pos;
    MR::calcCylinderPos(&pos, this);

    mPos.x = pos.x;
    mPos.y = pos.y;
    mPos.z = pos.z;

    mSound = new AudSoundObject(&mPos, 4, MR::getCurrentHeap());

    if (isValidSwitchA()) {
        MR::connectToSceneAreaObj(this);
    }
}

void GlaringLightArea::movement() {
    if (isOnSwitchA()) {
        bool bVar1 = false;

        if (mValid && _15 && mAwake) {
            bVar1 = true;
        }

        if (!bVar1) {
            MR::startSoundObject(mSound, "SE_OJ_RAY_START");
        }

        mValid = true;
    }
    else {
        mValid = false;
    }

    mSound->process();
}

const char *GlaringLightArea::getManagerName() const {
    return "GlaringLightArea";
}
