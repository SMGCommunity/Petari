

#include <JSystem/JGeometry/TMatrix.hpp>

void CameraTestObj_DUMMY() {
    TVec3f v1;
    TQuat4f a, b;
    a.setRotate(v1, 1.0f);
    b.setRotate(v1, 0.0f);

    TPos3f m, n;
    m.identity33();
    n.identity33();
}
