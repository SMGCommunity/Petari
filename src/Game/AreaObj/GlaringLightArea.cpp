#include "Game/AreaObj/GlaringLightArea.hpp"
#include "Game/AudioLib/AudSoundObject.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

GlaringLightAreaMgr::GlaringLightAreaMgr(s32 maxNum, const char* pName) : AreaObjMgr(maxNum, pName) {
}

GlaringLightArea::GlaringLightArea(int formType, const char* pName) : AreaObj(formType, pName), mSoundObj(), mPos(0.0f, 0.0f, 0.0f) {
}

void GlaringLightArea::init(const JMapInfoIter& rIter) {
    AreaObj::init(rIter);

    TVec3f pos;
    MR::calcCylinderPos(&pos, this);

    mPos.x = pos.x;
    mPos.y = pos.y;
    mPos.z = pos.z;

    mSoundObj = new AudSoundObject(&mPos, 4, MR::getCurrentHeap());

    if (isValidSwitchA()) {
        MR::connectToSceneAreaObj(this);
    }
}

GlaringLightArea::~GlaringLightArea() {
}

GlaringLightAreaMgr::~GlaringLightAreaMgr() {
}

void GlaringLightArea::movement() {
    if (isOnSwitchA()) {
        if (!isValid()) {
            MR::startSoundObject(mSoundObj, "SE_OJ_RAY_START");
        }

        mIsValid = true;
    } else {
        mIsValid = false;
    }

    mSoundObj->process();
}
