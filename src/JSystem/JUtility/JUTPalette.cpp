#include "JSystem/JUtility/JUTPalette.hpp"
#include <revolution/os.h>

void JUTPalette::storeTLUT(_GXTlut param_0, ResTLUT* tlut) {
    if (tlut == NULL) {
        OSPanic("JUTPalette.cpp", 35, "JUTTexture: TLUT is NULL\n");
    }
    mName = param_0;
    mFormat = tlut->mFormat;
    mTransparency = tlut->mTransparency;
    mLutNum = tlut->mLutNum;
    mColorTable = tlut + 8;
    GXInitTlutObj(&mObj, (void*)mColorTable, (GXTlutFmt)mFormat, mLutNum);
}

void JUTPalette::storeTLUT(_GXTlut param_0, _GXTlutFmt param_1, JUTTransparency param_2, u16 param_3, void* param_4) {
    mName = param_0;
    mFormat = param_1;
    mTransparency = param_2;
    mLutNum = param_3;
    mColorTable = (ResTLUT*)param_4;
    GXInitTlutObj(&mObj, (void*)mColorTable, (GXTlutFmt)mFormat, mLutNum);
}

bool JUTPalette::load() {
    bool check = mLutNum != 0;
    if (check) {
        GXLoadTlut(&mObj, mName);
    }

    return check;
}
