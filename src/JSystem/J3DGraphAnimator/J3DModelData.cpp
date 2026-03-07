#include "JSystem/J3DGraphAnimator/J3DModelData.hpp"
#include "JSystem/J3DGraphAnimator/J3DModel.hpp"
#include "JSystem/J3DGraphBase/J3DMaterial.hpp"

void J3DModelData::clear() {
    mpRawData = 0;
    mFlags = 0;
    mbHasBumpArray = 0;
    mbHasBillboard = 0;
}

J3DModelData::J3DModelData() {
    clear();
}

s32 J3DModelData::newSharedDisplayList(u32 mdlFlags) {
    u16 matNum = getMaterialNum();

    for (u16 i = 0; i < matNum; i++) {
        s32 ret;
        if (mdlFlags & J3DMdlFlag_UseSingleDL) {
            ret = getMaterialNodePointer(i)->newSingleSharedDisplayList(getMaterialNodePointer(i)->countDLSize());
            if (ret != kJ3DError_Success)
                return ret;
        } else {
            ret = getMaterialNodePointer(i)->newSharedDisplayList(getMaterialNodePointer(i)->countDLSize());
            if (ret != kJ3DError_Success)
                return ret;
        }
    }

    return kJ3DError_Success;
}

void J3DModelData::indexToPtr() {
    j3dSys.setTexture(getTexture());

    static BOOL sInterruptFlag = OSDisableInterrupts();
    OSDisableScheduler();

    GDLObj gdl_obj;
    u16 matNum = getMaterialNum();
    for (u16 i = 0; i < matNum; i++) {
        J3DMaterial* matNode = getMaterialNodePointer(i);
        J3DDisplayListObj* dl_obj = matNode->getSharedDisplayListObj();

        GDInitGDLObj(&gdl_obj, dl_obj->getDisplayList(0), dl_obj->getDisplayListSize());
        GDSetCurrent(&gdl_obj);
        matNode->getTevBlock()->indexToPtr();
    }

    GDSetCurrent(NULL);
    OSEnableScheduler();
    OSRestoreInterrupts(sInterruptFlag);
}

void J3DModelData::syncJ3DSysFlags() const {
    if (checkFlag(0x20)) {
        j3dSys.onFlag(J3DSysFlag_PostTexMtx);
    } else {
        j3dSys.offFlag(J3DSysFlag_PostTexMtx);
    }
}

J3DModelData::~J3DModelData() {}
