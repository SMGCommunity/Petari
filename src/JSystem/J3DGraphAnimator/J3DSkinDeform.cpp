#include "JSystem/J3DGraphAnimator/J3DSkinDeform.hpp"
#include "JSystem/J3DGraphAnimator/J3DModel.hpp"

void J3DSkinDeform::deform(J3DModel* pModel) {
    if (pModel->checkFlag(J3DMdlFlag_SkinPosCpu)) {
        onFlag(2);
    } else {
        offFlag(2);
    }

    if (pModel->checkFlag(J3DMdlFlag_SkinNrmCpu)) {
        onFlag(1);
    } else {
        offFlag(1);
    }

    deform(pModel->getVertexBuffer(), pModel->getMtxBuffer());
}

void J3DVtxColorCalc::calc(J3DModel* pModel) {
    calc(pModel->getVertexBuffer());
}
