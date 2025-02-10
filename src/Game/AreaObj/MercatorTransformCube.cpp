#include "Game/AreaObj/MercatorTransformCube.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "JSystem/JMath/JMath.hpp"

MercatorTransformCube::MercatorTransformCube(int type, const char *pName) : AreaObj(type, pName) {
    _3C.identity();
}

MercatorTransformCube::~MercatorTransformCube() {

}

void MercatorTransformCube::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);

    TMtx34f *matrix = MR::getZonePlacementMtx(rIter);
    _3C.setInline(*matrix);
}

f32 MercatorTransformCube::getSphereRadius() const {
    if (mObjArg0 == -1) {
        return 3000.0f;
    }

    return static_cast<f32>(mObjArg0);
}

void MercatorTransformCube::calcUV(TVec2f *uv, const TVec3f &a2) const {
    TVec3f boxSize;
    calcLocalBoxSize(&boxSize);
    boxSize *= 0.5f;

    TVec3f localPos;
    MR::calcCubeLocalPos(&localPos, this, a2);

    uv->x = 3.1415927f * localPos.x / boxSize.x;
    uv->y = 1.5707964f * -localPos.z / boxSize.z;
}

void MercatorTransformCube::calcLocalBoxSize(TVec3f *pPos) const {
    TBox3f *box = MR::getCubeLocalBox(this);

    TVec3f max;
    TVec3f min;

    min.set<f32>(box->mMin);
    max.set<f32>(box->mMax);

    JMathInlineVEC::PSVECSubtract(reinterpret_cast<Vec *>(&max), reinterpret_cast<Vec *>(&min), reinterpret_cast<Vec *>(pPos));
}
