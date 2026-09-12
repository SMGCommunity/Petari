#include "JSystem/J3DGraphBase/J3DShapeDraw.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"
#include <cstring>
#include <revolution/gx/GXDispList.h>
#include <stdint.h>

u32 J3DShapeDraw::countVertex(u32 stride) {
    u32 count = 0;
    u8* dlStart = static_cast< u8* >(getDisplayList());

    for (u8* dl = dlStart; (dl - dlStart) < getDisplayListSize();) {
        u8 cmd = *dl;
        dl++;
        if (cmd != GX_TRIANGLEFAN && cmd != GX_TRIANGLESTRIP)
            break;
        int vtxNum = *reinterpret_cast< u16* >(dl);
        dl += 2;
        count += vtxNum;
        dl = static_cast< u8* >(dl) + stride * vtxNum;
    }

    return count;
}

void J3DShapeDraw::addTexMtxIndexInDL(u32 stride, u32 attrOffs, u32 valueBase) {
    u32 newSize = ALIGN_NEXT(countVertex(stride) + mDisplayListSize, 0x20);
    u8* newDLStart = new (0x20) u8[newSize];
    u8* oldDLStart = static_cast< u8* >(mDisplayList);
    u8* oldDL = oldDLStart;
    u8* newDL = newDLStart;

    for (; (oldDL - oldDLStart) < mDisplayListSize;) {
        // Copy command
        u8 cmd = *oldDL;
        oldDL++;
        *newDL++ = cmd;

        if (cmd != GX_TRIANGLEFAN && cmd != GX_TRIANGLESTRIP)
            break;

        // Copy count
        int vtxNum = *reinterpret_cast< u16* >(oldDL);
        oldDL += 2;
        *reinterpret_cast< u16* >(newDL) = vtxNum;
        newDL += 2;

        for (int i = 0; i < vtxNum; i++) {
            u8* oldDLVtx = &oldDL[stride * i];
            u8 pnmtxidx = *oldDLVtx;
            memcpy(newDL, oldDLVtx, static_cast< int >(attrOffs));
            newDL += attrOffs;
            *newDL++ = valueBase + pnmtxidx;
            memcpy(newDL, oldDLVtx + attrOffs, stride - attrOffs);
            newDL += (stride - attrOffs);
        }

        oldDL = static_cast< u8* >(oldDL) + stride * vtxNum;
    }

    u32 realSize = ALIGN_NEXT(reinterpret_cast< uintptr_t >(newDL) - reinterpret_cast< uintptr_t >(newDLStart), 0x20);
    for (; (newDL - newDLStart) < newSize; newDL++)
        *newDL = 0;

    mDisplayListSize = realSize;
    mDisplayList = newDLStart;
    DCStoreRange(newDLStart, mDisplayListSize);
}

J3DShapeDraw::J3DShapeDraw(const u8* displayList, u32 displayListSize) {
    mDisplayList = const_cast< u8* >(displayList);
    mDisplayListSize = displayListSize;
}

void J3DShapeDraw::draw() const {
    GXCallDisplayList(mDisplayList, mDisplayListSize);
}

J3DShapeDraw::~J3DShapeDraw() {
}
