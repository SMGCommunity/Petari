#include "Game/Player/J3DModelX.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/ModelHolder.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"

void MarioActor::initShadow() {
    _A08 = 1;

    if (gIsLuigi) {
        _A10 = new ModelHolder("LuigiShadow", true);
    } else {
        _A10 = new ModelHolder("MarioShadow", true);
    }

    _A10->initWithoutIter();

    _A14 = reinterpret_cast< J3DModelX* >(MR::getJ3DModel(_A10));
    _A14->_DD = mModels[0]->_DD;

    for (s32 idx = 0; idx < _A14->_DD; idx++) {
        _A14->mExtraMtxBuffer[idx] = mModels[0]->mExtraMtxBuffer[idx];
    }
}

// void MarioActor::calcViewReflectionModel() {}

// void MarioActor::calcViewWallShadowModel() {}

// void MarioActor::drawShadow() const {}

// void MarioActor::decideShadowMode() {}

// void MarioActor::calcViewSilhouetteModel() {}

f32 MarioActor::calcCylinderToCenter(const AreaObj* pAreaObj, TVec3f* pVec) {
    TVec3f upVec;
    MR::calcCylinderUpVec(&upVec, pAreaObj);

    TVec3f centerPos;
    MR::calcCylinderCenterPos(&centerPos, pAreaObj);

    MR::vecKillElement(_2A0 - centerPos, upVec, pVec);

    if (MR::normalizeOrZero(pVec)) {
        if (_208 >= 1000.0f) {
            return -1.0f;
        }

        _208 += 10.0f;

        if (_208 >= 1000.0f) {
            _208 = 1000.0f;
        }

        return -1.0f;
    }

    return MR::getAreaObjArg(pAreaObj, 0);
}

// void MarioActor::calcViewBlurModel() {}

// void MarioActor::calcViewFootPrint() {}

// void MarioActor::drawSilhouette() const {}
