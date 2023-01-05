# gx
| Symbol | Meaning 
| ------------- | ------------- 
| :x: | Object has not yet been started. 
| :eight_pointed_black_star: | Object is in progress. 
| :white_check_mark: | Object is completed. 


| Object | Percentage (of Bytes) | Functions Done / Total Functions | Percentage (Functions) | Status 
| ------------- | ------------- | ------------- | ------------- | ------------- 
| GXInit.o | 0.0% | 0 / 6 | 0.0% | :x: 
| GXFifo.o | 0.0% | 0 / 17 | 0.0% | :x: 
| GXAttr.o | 0.0% | 0 / 11 | 0.0% | :x: 
| GXMisc.o | 0.0% | 0 / 25 | 0.0% | :x: 
| GXGeometry.o | 0.0% | 0 / 9 | 0.0% | :x: 
| GXFrameBuf.o | 0.0% | 0 / 15 | 0.0% | :x: 
| GXLight.o | 0.0% | 0 / 13 | 0.0% | :x: 
| GXTexture.o | 0.0% | 0 / 31 | 0.0% | :x: 
| GXBump.o | 0.0% | 0 / 10 | 0.0% | :x: 
| GXTev.o | 0.0% | 0 / 16 | 0.0% | :x: 
| GXPixel.o | 63.896103896103895% | 11 / 12 | 91.66666666666666% | :eight_pointed_black_star: 
| GXDisplayList.o | 0.0% | 0 / 3 | 0.0% | :x: 
| GXTransform.o | 5.6521739130434785% | 1 / 19 | 5.263157894736842% | :eight_pointed_black_star: 
| GXPerf.o | 0.0% | 0 / 3 | 0.0% | :x: 


# GXInit.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __GXDefaultTexRegionCallback | :x: |
| __GXDefaultTlutRegionCallback | :x: |
| __GXShutdown | :x: |
| __GXInitRevisionBits | :x: |
| GXInit | :x: |
| __GXInitGX | :x: |


# GXFifo.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| GXCPInterruptHandler | :x: |
| GXInitFifoBase | :x: |
| CPGPLinkCheck | :x: |
| GXSetCPUFifo | :x: |
| GXSetGPFifo | :x: |
| __GXSaveFifo | :x: |
| __GXIsGPFifoReady | :x: |
| GXGetGPStatus | :x: |
| GXGetCPUFifo | :x: |
| GXGetFifoPtrs | :x: |
| GXGetFifoCount | :x: |
| GXGetFifoWrap | :x: |
| GXSetBreakPtCallback | :x: |
| GXEnableBreakPt | :x: |
| GXDisableBreakPt | :x: |
| __GXFifoInit | :x: |
| __GXCleanGPFifo | :x: |


# GXAttr.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| GXSetVtxDesc | :x: |
| __GXSetVCD | :x: |
| __GXCalculateVLim | :x: |
| GXClearVtxDesc | :x: |
| GXSetVtxAttrFmt | :x: |
| GXSetVtxAttrFmtv | :x: |
| __GXSetVAT | :x: |
| GXSetArray | :x: |
| GXInvalidateVtxCache | :x: |
| GXSetTexCoordGen2 | :x: |
| GXSetNumTexGens | :x: |


# GXMisc.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| GXSetMisc | :x: |
| GXFlush | :x: |
| GXResetWriteGatherPipe | :x: |
| __GXAbort | :x: |
| GXAbortFrame | :x: |
| GXSetDrawSync | :x: |
| GXSetDrawDone | :x: |
| GXDrawDone | :x: |
| GXPixModeSync | :x: |
| GXTexModeSync | :x: |
| GXPokeAlphaMode | :x: |
| GXPokeAlphaRead | :x: |
| GXPokeAlphaUpdate | :x: |
| GXPokeBlendMode | :x: |
| GXPokeColorUpdate | :x: |
| GXPokeDstAlpha | :x: |
| GXPokeDither | :x: |
| GXPokeZMode | :x: |
| GXPeekARGB | :x: |
| GXPeekZ | :x: |
| GXSetDrawSyncCallback | :x: |
| GXTokenInterruptHandler | :x: |
| GXSetDrawDoneCallback | :x: |
| GXFinishInterruptHandler | :x: |
| __GXPEInit | :x: |


# GXGeometry.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __GXSetDirtyState | :x: |
| GXBegin | :x: |
| __GXSendFlushPrim | :x: |
| GXSetLineWidth | :x: |
| GXSetPointSize | :x: |
| GXEnableTexOffsets | :x: |
| GXSetCullMode | :x: |
| GXSetCoPlanar | :x: |
| __GXSetGenMode | :x: |


# GXFrameBuf.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| GXSetDispCopySrc | :x: |
| GXSetTexCopySrc | :x: |
| GXSetDispCopyDst | :x: |
| GXSetTexCopyDst | :x: |
| GXSetDispCopyFrame2Field | :x: |
| GXSetCopyClamp | :x: |
| GXGetNumXfbLines | :x: |
| GXGetYScaleFactor | :x: |
| GXSetDispCopyYScale | :x: |
| GXSetCopyClear | :x: |
| GXSetCopyFilter | :x: |
| GXSetDispCopyGamma | :x: |
| GXCopyDisp | :x: |
| GXCopyTex | :x: |
| GXClearBoundingBox | :x: |


# GXLight.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| GXInitLightAttn | :x: |
| GXInitLightSpot | :x: |
| GXInitLightDistAttn | :x: |
| GXInitLightPos | :x: |
| GXInitLightDir | :x: |
| GXInitSpecularDir | :x: |
| GXInitSpecularDirHA | :x: |
| GXInitLightColor | :x: |
| GXLoadLightObjImm | :x: |
| GXSetChanAmbColor | :x: |
| GXSetChanMatColor | :x: |
| GXSetNumChans | :x: |
| GXSetChanCtrl | :x: |


# GXTexture.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| GXGetTexBufferSize | :x: |
| __GetImageTileCount | :x: |
| GXInitTexObj | :x: |
| GXInitTexObjCI | :x: |
| GXInitTexObjLOD | :x: |
| GXInitTexObjWrapMode | :x: |
| GXInitTexObjTlut | :x: |
| GXInitTexObjUserData | :x: |
| GXGetTexObjUserData | :x: |
| GXGetTexObjAll | :x: |
| GXGetTexObjWidth | :x: |
| GXGetTexObjHeight | :x: |
| GXGetTexObjFmt | :x: |
| GXGetTexObjWrapS | :x: |
| GXGetTexObjWrapT | :x: |
| GXGetTexObjMipMap | :x: |
| GXGetTexObjLODAll | :x: |
| GXGetTexObjTlut | :x: |
| GXLoadTexObjPreLoaded | :x: |
| GXLoadTexObj | :x: |
| GXInitTlutObj | :x: |
| GXLoadTlut | :x: |
| GXInitTexCacheRegion | :x: |
| GXInitTlutRegion | :x: |
| GXInvalidateTexAll | :x: |
| GXSetTexRegionCallback | :x: |
| GXSetTlutRegionCallback | :x: |
| GXSetTexCoordScaleManually | :x: |
| __SetSURegs | :x: |
| __GXSetSUTexRegs | :x: |
| __GXSetTmemConfig | :x: |


# GXBump.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| GXSetTevIndirect | :x: |
| GXSetIndTexMtx | :x: |
| GXSetIndTexCoordScale | :x: |
| GXSetIndTexOrder | :x: |
| GXSetNumIndStages | :x: |
| GXSetTevDirect | :x: |
| GXSetTevIndWarp | :x: |
| __GXUpdateBPMask | :x: |
| __GXSetIndirectMask | :x: |
| __GXFlushTextureState | :x: |


# GXTev.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| GXSetTevOp | :x: |
| GXSetTevColorIn | :x: |
| GXSetTevAlphaIn | :x: |
| GXSetTevColorOp | :x: |
| GXSetTevAlphaOp | :x: |
| GXSetTevColor | :x: |
| GXSetTevColorS10 | :x: |
| GXSetTevKColor | :x: |
| GXSetTevKColorSel | :x: |
| GXSetTevKAlphaSel | :x: |
| GXSetTevSwapMode | :x: |
| GXSetTevSwapModeTable | :x: |
| GXSetAlphaCompare | :x: |
| GXSetZTexture | :x: |
| GXSetTevOrder | :x: |
| GXSetNumTevStages | :x: |


# GXPixel.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| GXSetFog | :x: |
| GXSetFogRangeAdj | :white_check_mark: |
| GXSetBlendMode | :white_check_mark: |
| GXSetColorUpdate | :white_check_mark: |
| GXSetAlphaUpdate | :white_check_mark: |
| GXSetZMode | :white_check_mark: |
| GXSetZCompLoc | :white_check_mark: |
| GXSetPixelFmt | :white_check_mark: |
| GXSetDither | :white_check_mark: |
| GXSetDstAlpha | :white_check_mark: |
| GXSetFieldMask | :white_check_mark: |
| GXSetFieldMode | :white_check_mark: |


# GXDisplayList.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| GXBeginDisplayList | :x: |
| GXEndDisplayList | :x: |
| GXCallDisplayList | :x: |


# GXTransform.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| GXProject | :x: |
| __GXSetProjection | :x: |
| GXSetProjection | :x: |
| GXSetProjectionv | :x: |
| GXGetProjectionv | :x: |
| GXLoadPosMtxImm | :x: |
| GXLoadNrmMtxImm | :x: |
| GXSetCurrentMtx | :x: |
| GXLoadTexMtxImm | :x: |
| GXLoadTexMtxIndx | :x: |
| __GXSetViewport | :x: |
| GXSetViewport | :x: |
| GXGetViewportv | :x: |
| GXSetZScaleOffset | :x: |
| GXSetScissor | :white_check_mark: |
| GXGetScissor | :x: |
| GXSetScissorBoxOffset | :x: |
| GXSetClipMode | :x: |
| __GXSetMatrixIndex | :x: |


# GXPerf.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| GXSetGPMetric | :x: |
| GXClearGPMetric | :x: |
| GXReadXfRasMetric | :x: |


