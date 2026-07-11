#include "Game/AreaObj/MercatorTransformCube.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/SceneUtil.hpp"

void FORCE_OPERATOR() {
    TVec3f vec;
    vec *= 0.0f;
}

MercatorTransformCube::MercatorTransformCube(int formType, const char* pName) : AreaObj(formType, pName) {
    _3C.identity();
}

void MercatorTransformCube::init(const JMapInfoIter& rIter) {
    AreaObj::init(rIter);

    _3C.setInline(*MR::getZonePlacementMtx(rIter));
}

f32 MercatorTransformCube::getSphereRadius() const {
    if (mObjArg0 == -1) {
        return 3000.0f;
    }

    return mObjArg0;
}

void MercatorTransformCube::calcUV(TVec2f* pUV, const TVec3f& a2) const {
    TVec3f boxSize;
    calcLocalBoxSize(&boxSize);
    boxSize *= 0.5f;

    TVec3f localPos;
    MR::calcCubeLocalPos(&localPos, this, a2);

    pUV->x = PI * localPos.x / boxSize.x;
    pUV->y = HALF_PI * -localPos.z / boxSize.z;
}

void MercatorTransformCube::calcLocalBoxSize(TVec3f* pPos) const {
    TBox3f* box = MR::getCubeLocalBox(this);

    TVec3f max;
    TVec3f min;

    min.set< f32 >(box->i);
    max.set< f32 >(box->f);

    pPos->sub(max, min);
}
