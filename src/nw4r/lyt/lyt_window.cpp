#include "nw4r/lyt/layout.h"
#include "nw4r/lyt/material.h"
#include "nw4r/lyt/window.h"

namespace nw4r {
    namespace lyt {

        namespace {
            struct TextureFlipInfo {
                u8 coords[4][2];
                u8 idx[2];
            };

            const TextureFlipInfo& GetTexutreFlipInfo(u8 textureFlip) {
                static TextureFlipInfo flipInfos[] = {
                    {{{0, 0}, {1, 0}, {0, 1}, {1, 1}}, {0, 1}},  // TEXTUREFLIP_NONE
                    {{{1, 0}, {0, 0}, {1, 1}, {0, 1}}, {0, 1}},  // TEXTUREFLIP_FLIPH
                    {{{0, 1}, {1, 1}, {0, 0}, {1, 0}}, {0, 1}},  // TEXTUREFLIP_FLIPV
                    {{{0, 1}, {0, 0}, {1, 1}, {1, 0}}, {1, 0}},  // TEXTUREFLIP_ROTATE90
                    {{{1, 1}, {0, 1}, {1, 0}, {0, 0}}, {0, 1}},  // TEXTUREFLIP_ROTATE180
                    {{{1, 0}, {1, 1}, {0, 0}, {0, 1}}, {1, 0}},  // TEXTUREFLIP_ROTATE270
                };

                return flipInfos[textureFlip];
            }

            void GetLTFrameSize(math::VEC2* pPoint, Size* pSize, const math::VEC2& rBase, const Size& rWindowSize,
                                const WindowFrameSize& rFrameSize) {
                *pPoint = rBase;

                pSize->width = rWindowSize.width - rFrameSize.r;
                pSize->height = rFrameSize.t;
            }

            void GetLTTexCoord(math::VEC2* pTexCoords, const Size& rSize, const Size& rTexSize, u8 flipType) {
                const TextureFlipInfo& rInfo = GetTexutreFlipInfo(flipType);

                int ix = rInfo.idx[0];
                int iy = rInfo.idx[1];

                math::VEC2 texSize(rTexSize.width, rTexSize.height);

                pTexCoords[0][ix] = pTexCoords[2][ix] = rInfo.coords[0][ix];

                pTexCoords[0][iy] = pTexCoords[1][iy] = rInfo.coords[0][iy];

                pTexCoords[3][ix] = pTexCoords[1][ix] =
                    rInfo.coords[0][ix] + rSize.width / ((rInfo.coords[1][ix] - rInfo.coords[0][ix]) * texSize[ix]);

                pTexCoords[3][iy] = pTexCoords[2][iy] =
                    rInfo.coords[0][iy] + rSize.height / ((rInfo.coords[2][iy] - rInfo.coords[0][iy]) * texSize[iy]);
            }

            void GetRTFrameSize(math::VEC2* pPoint, Size* pSize, const math::VEC2& rBase, const Size& rWindowSize,
                                const WindowFrameSize& rFrameSize) {
                *pPoint = math::VEC2(rBase.x + rWindowSize.width - rFrameSize.r, rBase.y);

                pSize->width = rFrameSize.r;
                pSize->height = rWindowSize.height - rFrameSize.b;
            }

            void GetRTTexCoord(math::VEC2* pTexCoords, const Size& rSize, const Size& rTexSize, u8 flipType) {
                const TextureFlipInfo& rInfo = GetTexutreFlipInfo(flipType);

                int ix = rInfo.idx[0];
                int iy = rInfo.idx[1];

                math::VEC2 texSize(rTexSize.width, rTexSize.height);

                pTexCoords[1][ix] = pTexCoords[3][ix] = rInfo.coords[1][ix];

                pTexCoords[1][iy] = pTexCoords[0][iy] = rInfo.coords[1][iy];

                pTexCoords[2][ix] = pTexCoords[0][ix] =
                    rInfo.coords[1][ix] + rSize.width / ((rInfo.coords[0][ix] - rInfo.coords[1][ix]) * texSize[ix]);

                pTexCoords[2][iy] = pTexCoords[3][iy] =
                    rInfo.coords[1][iy] + rSize.height / ((rInfo.coords[3][iy] - rInfo.coords[1][iy]) * texSize[iy]);
            }

            void GetLBFrameSize(math::VEC2* pPoint, Size* pSize, const math::VEC2& rBase, const Size& rWindowSize,
                                const WindowFrameSize& rFrameSize) {
                *pPoint = math::VEC2(rBase.x, rBase.y + rFrameSize.t);

                pSize->width = rFrameSize.l;
                pSize->height = rWindowSize.height - rFrameSize.t;
            }

            void GetLBTexCoord(math::VEC2* pTexCoords, const Size& rSize, const Size& rTexSize, u8 flipType) {
                const TextureFlipInfo& rInfo = GetTexutreFlipInfo(flipType);

                int ix = rInfo.idx[0];
                int iy = rInfo.idx[1];

                math::VEC2 texSize(rTexSize.width, rTexSize.height);

                pTexCoords[2][ix] = pTexCoords[0][ix] = rInfo.coords[2][ix];

                pTexCoords[2][iy] = pTexCoords[3][iy] = rInfo.coords[2][iy];

                pTexCoords[1][ix] = pTexCoords[3][ix] =
                    rInfo.coords[2][ix] + rSize.width / ((rInfo.coords[3][ix] - rInfo.coords[2][ix]) * texSize[ix]);

                pTexCoords[1][iy] = pTexCoords[0][iy] =
                    rInfo.coords[2][iy] + rSize.height / ((rInfo.coords[0][iy] - rInfo.coords[2][iy]) * texSize[iy]);
            }

            void GetRBFrameSize(math::VEC2* pPoint, Size* pSize, const math::VEC2& rBase, const Size& rWindowSize,
                                const WindowFrameSize& rFrameSize) {
                *pPoint = math::VEC2(rBase.x + rFrameSize.l, rBase.y + rWindowSize.height - rFrameSize.b);

                pSize->width = rWindowSize.width - rFrameSize.l;
                pSize->height = rFrameSize.b;
            }

            void GetRBTexCoord(math::VEC2* pTexCoords, const Size& rSize, const Size& rTexSize, u8 flipType) {
                const TextureFlipInfo& rInfo = GetTexutreFlipInfo(flipType);

                int ix = rInfo.idx[0];
                int iy = rInfo.idx[1];

                math::VEC2 texSize(rTexSize.width, rTexSize.height);
                pTexCoords[3][ix] = pTexCoords[1][ix] = rInfo.coords[3][ix];
                pTexCoords[3][iy] = pTexCoords[2][iy] = rInfo.coords[3][iy];

                pTexCoords[0][ix] = pTexCoords[2][ix] =
                    rInfo.coords[3][ix] + rSize.width / ((rInfo.coords[2][ix] - rInfo.coords[3][ix]) * texSize[ix]);

                pTexCoords[0][iy] = pTexCoords[1][iy] =
                    rInfo.coords[3][iy] + rSize.height / ((rInfo.coords[1][iy] - rInfo.coords[3][iy]) * texSize[iy]);
            }

        };  // namespace

        NW4R_UT_RTTI_DEF_DERIVED(Window, Pane);

        Window::Window(const res::Window* pRes, const ResBlockSet& rBlockSet) : Pane(pRes) {
            mContentInflation = pRes->inflation;

            const u32* const pMatOffsetTbl = detail::ConvertOffsToPtr< u32 >(rBlockSet.pMaterialList, sizeof(res::MaterialList));

            const res::WindowContent* pResContent = detail::ConvertOffsToPtr< res::WindowContent >(pRes, pRes->contentOffset);

            for (int i = 0; i < 4; i++) {
                mContent.vtxColors[i] = pResContent->vtxCols[i];
            }

            if (pResContent->texCoordNum > 0) {
                u8 num = ut::Min< u8 >(pResContent->texCoordNum, GX_MAX_TEXCOORD);
                mContent.texCoordAry.Reserve(num);

                if (!mContent.texCoordAry.IsEmpty()) {
                    mContent.texCoordAry.Copy(reinterpret_cast< const u8* >(pResContent) + sizeof(res::WindowContent), num);
                }
            }

            void* pMaterialBuf = Layout::AllocMemory(sizeof(Material));

            if (pMaterialBuf != NULL) {
                const res::Material* const pResMaterial =
                    detail::ConvertOffsToPtr< res::Material >(rBlockSet.pMaterialList, pMatOffsetTbl[pResContent->materialIdx]);

                mpMaterial = new (pMaterialBuf) Material(pResMaterial, rBlockSet);
            }

            mFrameNum = 0;
            mFrames = NULL;

            if (pRes->frameNum > 0) {
                mFrames = static_cast< Frame* >(Layout::AllocMemory(pRes->frameNum * sizeof(Frame)));

                if (mFrames != NULL) {
                    mFrameNum = pRes->frameNum;

                    const u32* const pFrmOffsetTbl = detail::ConvertOffsToPtr< u32 >(pRes, pRes->frameOffsetTableOffset);

                    for (int i = 0; i < mFrameNum; i++) {
                        const res::WindowFrame* const pResFrame = detail::ConvertOffsToPtr< res::WindowFrame >(pRes, pFrmOffsetTbl[i]);

                        mFrames[i].textureFlip = pResFrame->textureFlip;
                        mFrames[i].pMaterial = NULL;

                        void* pMaterialBuf = Layout::AllocMemory(sizeof(Material));

                        if (pMaterialBuf != NULL) {
                            const res::Material* const pResMaterial =
                                detail::ConvertOffsToPtr< res::Material >(rBlockSet.pMaterialList, pMatOffsetTbl[pResFrame->materialIdx]);

                            mFrames[i].pMaterial = new (pMaterialBuf) Material(pResMaterial, rBlockSet);
                        }
                    }
                }
            }
        }

        Window::~Window() {
            if (mFrames != NULL) {
                for (int i = 0; i < mFrameNum; i++) {
                    mFrames[i].pMaterial->~Material();
                    Layout::FreeMemory(mFrames[i].pMaterial);
                }

                Layout::FreeMemory(mFrames);
            }

            if (mpMaterial != NULL && !mpMaterial->IsUserAllocated()) {
                mpMaterial->~Material();
                Layout::FreeMemory(mpMaterial);
                mpMaterial = NULL;
            }

            mContent.texCoordAry.Free();
        }

        Material* Window::FindMaterialByName(const char* pName, bool recursive) {
            if (mpMaterial != NULL && detail::EqualsMaterialName(mpMaterial->GetName(), pName)) {
                return mpMaterial;
            }

            for (int i = 0; i < mFrameNum; i++) {
                if (detail::EqualsMaterialName(mFrames[i].pMaterial->GetName(), pName)) {
                    return mFrames[i].pMaterial;
                }
            }

            if (recursive) {
                for (PaneList::Iterator it = mChildList.GetBeginIter(); it != mChildList.GetEndIter(); ++it) {
                    if (Material* pMat = it->FindMaterialByName(pName, true)) {
                        return pMat;
                    }
                }
            }

            return NULL;
        }

        void Window::SetAnimationEnable(AnimTransform* pAnimTrans, bool enable, bool recursive) {
            for (int i = 0; i < mFrameNum; i++) {
                mFrames[i].pMaterial->SetAnimationEnable(pAnimTrans, enable);
            }

            Pane::SetAnimationEnable(pAnimTrans, enable, recursive);
        }

        const ut::Color Window::GetVtxColor(u32 idx) const {
            return mContent.vtxColors[idx];
        }

        void Window::SetVtxColor(u32 idx, ut::Color color) {
            mContent.vtxColors[idx] = color;
        }

        u8 Window::GetVtxColorElement(u32 idx) const {
            return detail::GetVtxColorElement(mContent.vtxColors, idx);
        }

        void Window::SetVtxColorElement(u32 idx, u8 value) {
            detail::SetVtxColorElement(mContent.vtxColors, idx, value);
        }

        WindowFrameSize Window::GetFrameSize(u8 frameNum, const Frame* pFrames) {
            WindowFrameSize frameSize = {0.0f, 0.0f, 0.0f, 0.0f};

            switch (frameNum) {
            case 1: {
                Size texSize = detail::GetTextureSize(pFrames[0].pMaterial, 0);

                frameSize.l = texSize.width;
                frameSize.t = texSize.height;

                frameSize.r = texSize.width;
                frameSize.b = texSize.height;
                break;
            }

            case 4:
            case 8: {
                Size texSize = detail::GetTextureSize(pFrames[0].pMaterial, 0);

                frameSize.l = texSize.width;
                frameSize.t = texSize.height;

                texSize = detail::GetTextureSize(pFrames[3].pMaterial, 0);

                frameSize.r = texSize.width;
                frameSize.b = texSize.height;
                break;
            }
            }

            return frameSize;
        }

        void Window::DrawSelf(const DrawInfo& rInfo) {
            LoadMtx(rInfo);

            WindowFrameSize size = GetFrameSize(mFrameNum, mFrames);
            math::VEC2 base = GetVtxPos();
            DrawContent(base, size, mGlbAlpha);

            switch (mFrameNum) {
            case 1: {
                DrawFrame(base, mFrames[0], size, mGlbAlpha);
                break;
            }

            case 4: {
                DrawFrame4(base, mFrames, size, mGlbAlpha);
                break;
            }

            case 8: {
                DrawFrame8(base, mFrames, size, mGlbAlpha);
                break;
            }
            }
        }

        void Window::AnimateSelf(u32 option) {
            Pane::AnimateSelf(option);

            if (IsVisible() || !(option & 1)) {
                for (int i = 0; i < mFrameNum; i++) {
                    mFrames[i].pMaterial->Animate();
                }
            }
        }

        void Window::UnbindAnimationSelf(AnimTransform* pAnimTrans) {
            for (int i = 0; i < mFrameNum; i++) {
                mFrames[i].pMaterial->UnbindAnimation(pAnimTrans);
            }

            Pane::UnbindAnimationSelf(pAnimTrans);
        }

        void Window::DrawContent(const math::VEC2& rBase, const WindowFrameSize& rFrameSize, u8 alpha) {
            bool useVtxColor = mpMaterial->SetupGX(detail::IsModulateVertexColor(mContent.vtxColors, alpha), alpha);

            detail::SetVertexFormat(useVtxColor, mContent.texCoordAry.GetSize());

            detail::DrawQuad(math::VEC2(rBase.x + rFrameSize.l - mContentInflation.l, rBase.y + rFrameSize.t - mContentInflation.t),

                             Size(mSize.width - rFrameSize.l + mContentInflation.l - rFrameSize.r + mContentInflation.r,
                                  mSize.height - rFrameSize.t + mContentInflation.t - rFrameSize.b + mContentInflation.b),

                             mContent.texCoordAry.GetSize(), mContent.texCoordAry.GetArray(),

                             useVtxColor ? mContent.vtxColors : NULL, alpha);
        }

        void Window::DrawFrame(const math::VEC2& basePt, const Frame& frame, const WindowFrameSize& frameSize, u8 alpha) {
            if (frame.pMaterial->GetTextureNum() == 0) {
                return;
            }

            const u8 texCoordNum = 1;

            const bool bUseVtxCol = frame.pMaterial->SetupGX(detail::IsModulateVertexColor(NULL, alpha), alpha);

            detail::SetVertexFormat(bUseVtxCol, texCoordNum);

            const Size texSize = detail::GetTextureSize(frame.pMaterial, 0);
            ut::Color vtxColors[4];
            math::VEC2 texCds[texCoordNum][4];
            math::VEC2 polPt;
            Size polSize;

            GetLTFrameSize(&polPt, &polSize, basePt, mSize, frameSize);
            GetLTTexCoord(texCds[0], polSize, texSize, 0);
            detail::DrawQuad(polPt, polSize, texCoordNum, texCds, bUseVtxCol ? vtxColors : 0, alpha);

            GetRTFrameSize(&polPt, &polSize, basePt, mSize, frameSize);
            GetRTTexCoord(texCds[0], polSize, texSize, 1);
            detail::DrawQuad(polPt, polSize, texCoordNum, texCds, bUseVtxCol ? vtxColors : 0, alpha);

            GetRBFrameSize(&polPt, &polSize, basePt, mSize, frameSize);
            GetRBTexCoord(texCds[0], polSize, texSize, 4);
            detail::DrawQuad(polPt, polSize, texCoordNum, texCds, bUseVtxCol ? vtxColors : 0, alpha);

            GetLBFrameSize(&polPt, &polSize, basePt, mSize, frameSize);
            GetLBTexCoord(texCds[0], polSize, texSize, 2);
            detail::DrawQuad(polPt, polSize, texCoordNum, texCds, bUseVtxCol ? vtxColors : 0, alpha);
        }

        void Window::DrawFrame4(const math::VEC2& basePt, const Frame* frames, const WindowFrameSize& frameSize, u8 alpha) {
            const u8 texCoordNum = 1;

            ut::Color vtxColors[4];
            math::VEC2 texCds[texCoordNum][4];
            math::VEC2 polPt;
            Size polSize;
            const bool bModVtxCol = detail::IsModulateVertexColor(NULL, alpha);

            if (frames[0].pMaterial->GetTextureNum() > 0) {
                const bool bUseVtxCol = frames[0].pMaterial->SetupGX(bModVtxCol, alpha);
                GetLTFrameSize(&polPt, &polSize, basePt, mSize, frameSize);
                GetLTTexCoord(texCds[0], polSize, detail::GetTextureSize(frames[0].pMaterial, 0), frames[0].textureFlip);
                detail::SetVertexFormat(bUseVtxCol, texCoordNum);
                detail::DrawQuad(polPt, polSize, texCoordNum, texCds, bUseVtxCol ? vtxColors : 0, alpha);
            }

            if (frames[1].pMaterial->GetTextureNum() > 0) {
                const bool bUseVtxCol = frames[1].pMaterial->SetupGX(bModVtxCol, alpha);
                GetRTFrameSize(&polPt, &polSize, basePt, mSize, frameSize);
                GetRTTexCoord(texCds[0], polSize, detail::GetTextureSize(frames[1].pMaterial, 0), frames[1].textureFlip);
                detail::SetVertexFormat(bUseVtxCol, texCoordNum);
                detail::DrawQuad(polPt, polSize, texCoordNum, texCds, bUseVtxCol ? vtxColors : 0, alpha);
            }

            if (frames[3].pMaterial->GetTextureNum() > 0) {
                const bool bUseVtxCol = frames[3].pMaterial->SetupGX(bModVtxCol, alpha);
                GetRBFrameSize(&polPt, &polSize, basePt, mSize, frameSize);
                GetRBTexCoord(texCds[0], polSize, detail::GetTextureSize(frames[3].pMaterial, 0), frames[3].textureFlip);
                detail::SetVertexFormat(bUseVtxCol, texCoordNum);
                detail::DrawQuad(polPt, polSize, texCoordNum, texCds, bUseVtxCol ? vtxColors : 0, alpha);
            }

            if (frames[2].pMaterial->GetTextureNum() > 0) {
                const bool bUseVtxCol = frames[2].pMaterial->SetupGX(bModVtxCol, alpha);
                GetLBFrameSize(&polPt, &polSize, basePt, mSize, frameSize);
                GetLBTexCoord(texCds[0], polSize, detail::GetTextureSize(frames[2].pMaterial, 0), frames[2].textureFlip);
                detail::SetVertexFormat(bUseVtxCol, texCoordNum);
                detail::DrawQuad(polPt, polSize, texCoordNum, texCds, bUseVtxCol ? vtxColors : 0, alpha);
            }
        }

        void Window::DrawFrame8(const math::VEC2& basePt, const Frame* frames, const WindowFrameSize& frameSize, u8 alpha) {
            const u8 texCoordNum = 1;
            ut::Color vtxColors[4];
            math::VEC2 texCds[texCoordNum][4];
            Size polSize;
            const bool bModVtxCol = detail::IsModulateVertexColor(NULL, alpha);

            if (frames[0].pMaterial->GetTextureNum() > 0) {
                const bool bUseVtxCol = frames[0].pMaterial->SetupGX(bModVtxCol, alpha);
                polSize = Size(frameSize.l, frameSize.t);
                GetLTTexCoord(texCds[0], polSize, detail::GetTextureSize(frames[0].pMaterial, 0), frames[0].textureFlip);
                detail::SetVertexFormat(bUseVtxCol, texCoordNum);
                detail::DrawQuad(basePt, polSize, texCoordNum, texCds, bUseVtxCol ? vtxColors : 0, alpha);
            }

            if (frames[6].pMaterial->GetTextureNum() > 0) {
                const bool bUseVtxCol = frames[6].pMaterial->SetupGX(bModVtxCol, alpha);
                polSize = Size(mSize.width - frameSize.l - frameSize.r, frameSize.t);
                GetLTTexCoord(texCds[0], polSize, detail::GetTextureSize(frames[6].pMaterial, 0), frames[6].textureFlip);
                detail::SetVertexFormat(bUseVtxCol, texCoordNum);
                detail::DrawQuad(math::VEC2(basePt.x + frameSize.l, basePt.y), polSize, texCoordNum, texCds, bUseVtxCol ? vtxColors : 0, alpha);
            }

            if (frames[1].pMaterial->GetTextureNum() > 0) {
                const bool bUseVtxCol = frames[1].pMaterial->SetupGX(bModVtxCol, alpha);
                polSize = Size(frameSize.r, frameSize.t);
                GetRTTexCoord(texCds[0], polSize, detail::GetTextureSize(frames[1].pMaterial, 0), frames[1].textureFlip);
                detail::SetVertexFormat(bUseVtxCol, texCoordNum);
                detail::DrawQuad(math::VEC2(basePt.x + mSize.width - frameSize.r, basePt.y), polSize, texCoordNum, texCds, bUseVtxCol ? vtxColors : 0,
                                 alpha);
            }

            if (frames[5].pMaterial->GetTextureNum() > 0) {
                const bool bUseVtxCol = frames[5].pMaterial->SetupGX(bModVtxCol, alpha);
                polSize = Size(frameSize.r, mSize.height - frameSize.t - frameSize.b);
                GetRTTexCoord(texCds[0], polSize, detail::GetTextureSize(frames[5].pMaterial, 0), frames[5].textureFlip);
                detail::SetVertexFormat(bUseVtxCol, texCoordNum);
                detail::DrawQuad(math::VEC2(basePt.x + mSize.width - frameSize.r, basePt.y - frameSize.t), polSize, texCoordNum, texCds,
                                 bUseVtxCol ? vtxColors : 0, alpha);
            }

            if (frames[3].pMaterial->GetTextureNum() > 0) {
                const bool bUseVtxCol = frames[3].pMaterial->SetupGX(bModVtxCol, alpha);
                polSize = Size(frameSize.r, frameSize.b);
                GetRBTexCoord(texCds[0], polSize, detail::GetTextureSize(frames[3].pMaterial, 0), frames[3].textureFlip);
                detail::SetVertexFormat(bUseVtxCol, texCoordNum);
                detail::DrawQuad(math::VEC2(basePt.x + mSize.width - frameSize.r, basePt.y - mSize.height + frameSize.b), polSize, texCoordNum,
                                 texCds, bUseVtxCol ? vtxColors : 0, alpha);
            }

            if (frames[7].pMaterial->GetTextureNum() > 0) {
                const bool bUseVtxCol = frames[7].pMaterial->SetupGX(bModVtxCol, alpha);
                polSize = Size(mSize.width - frameSize.l - frameSize.r, frameSize.b);
                GetRBTexCoord(texCds[0], polSize, detail::GetTextureSize(frames[7].pMaterial, 0), frames[7].textureFlip);
                detail::SetVertexFormat(bUseVtxCol, texCoordNum);
                detail::DrawQuad(math::VEC2(basePt.x + frameSize.l, basePt.y - mSize.height + frameSize.b), polSize, texCoordNum, texCds,
                                 bUseVtxCol ? vtxColors : 0, alpha);
            }

            if (frames[2].pMaterial->GetTextureNum() > 0) {
                const bool bUseVtxCol = frames[2].pMaterial->SetupGX(bModVtxCol, alpha);
                polSize = Size(frameSize.l, frameSize.b);
                GetLBTexCoord(texCds[0], polSize, detail::GetTextureSize(frames[2].pMaterial, 0), frames[2].textureFlip);
                detail::SetVertexFormat(bUseVtxCol, texCoordNum);
                detail::DrawQuad(math::VEC2(basePt.x, basePt.y - mSize.height + frameSize.b), polSize, texCoordNum, texCds,
                                 bUseVtxCol ? vtxColors : 0, alpha);
            }

            if (frames[4].pMaterial->GetTextureNum() > 0) {
                const bool bUseVtxCol = frames[4].pMaterial->SetupGX(bModVtxCol, alpha);
                polSize = Size(frameSize.l, mSize.height - frameSize.t - frameSize.b);
                GetLBTexCoord(texCds[0], polSize, detail::GetTextureSize(frames[4].pMaterial, 0), frames[4].textureFlip);
                detail::SetVertexFormat(bUseVtxCol, texCoordNum);
                detail::DrawQuad(math::VEC2(basePt.x, basePt.y - frameSize.t), polSize, texCoordNum, texCds, bUseVtxCol ? vtxColors : 0, alpha);
            }
        }

        Material* Window::GetFrameMaterial(u32 idx) const {
            if (idx >= mFrameNum) {
                return NULL;
            }

            return mFrames[idx].pMaterial;
        }

        Material* Window::GetContentMaterial() const {
            return GetMaterial();
        }

    };  // namespace lyt
};  // namespace nw4r
