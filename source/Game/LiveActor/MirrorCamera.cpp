#include "Game/LiveActor/MirrorCamera.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util.hpp"

void MirrorCamera::init(const JMapInfoIter &rIter) {
    MR::connectToScene(this, 0x16, -1, -1, -1);
}

void MirrorCamera::setMirrorMapInfo(const TVec3f &a1, const TVec3f &a2) {
    _18.set(a1);
    _C.set(a2);
    _24 = -PSVECDotProduct((const Vec*)&_18, (const Vec*)&_C);
}

f32 MirrorCamera::getDistance(const TVec3f &a1) const {
    TVec3f stack_14;
    TVec3f stack_8;
    
    stack_8.set(_18);
    stack_14.x = stack_8.dot(_C);
    return stack_8.dot(a1) - stack_14.x;
}

namespace MR {
    MirrorCamera* getMirrorCamera() {
        return reinterpret_cast<MirrorCamera*>(MR::getSceneObjHolder()->getObj(SceneObj_MirrorCamera));
    }

    f32 getDistanceToMirror(const TVec3f &rVec) {
        return reinterpret_cast<MirrorCamera*>(MR::getSceneObjHolder()->getObj(SceneObj_MirrorCamera))->getDistance(rVec);
    }
};

MirrorCamera::~MirrorCamera() {

}

void MirrorCamera::movement() {
    if (MR::isPlayerInAreaObj("MirrorArea")) {
        updateViewMtx();
        updateModelTexMtx();
    }   
}