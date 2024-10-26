#include "Game/Screen/CopyFilterNegater.hpp"

CopyFilterNegater::CopyFilterNegater() {
    mRenderObj = JUTVideo::sManager->mRenderModeObj;
    GXSetCopyFilter(GX_FALSE, mRenderObj->sample_pattern, GX_FALSE, mRenderObj->vfilter);
}

CopyFilterNegater::~CopyFilterNegater() {
    GXSetCopyFilter(GX_FALSE, mRenderObj->sample_pattern, GX_TRUE, mRenderObj->vfilter);
}