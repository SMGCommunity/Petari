#include <JSystem/JGeometry/TVec.hpp>

void CamPoseSphereInterpolator_DUMMY() {
    TVec3f a, b;
    TQuat4f q;
    q.setRotate(a, b, 1.0f);
    q.setRotate(a, b, 1.0f);
    q.transform(a, b);
    q.transform(a, b);
}
