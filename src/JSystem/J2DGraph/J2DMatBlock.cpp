#include "JSystem/JUtility/JUTTexture.hpp"

JUTTexture::JUTTexture() {
    setCaptureFlag(false);
    mEmbPalette = NULL;
    mTIMG = NULL;
}

void JUTTexture::setCaptureFlag(bool flag) {
    mFlag = mFlag & 0x2 | flag;
}
