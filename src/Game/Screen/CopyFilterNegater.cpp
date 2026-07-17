#include "Game/Screen/CopyFilterNegater.hpp"
#include <JSystem/JUtility/JUTVideo.hpp>

CopyFilterNegater::CopyFilterNegater() {
    mRenderObj = JUTVideo::getManager()->getRenderMode();
    GXSetCopyFilter(GX_FALSE, mRenderObj->sample_pattern, GX_FALSE, mRenderObj->vfilter);
}

CopyFilterNegater::~CopyFilterNegater() {
    GXSetCopyFilter(GX_FALSE, mRenderObj->sample_pattern, GX_TRUE, mRenderObj->vfilter);
}
