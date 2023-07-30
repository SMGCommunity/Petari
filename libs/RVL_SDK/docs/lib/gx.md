# gx
| Symbol | Meaning 
| ------------- | ------------- 
| :x: | Object has not yet been started. 
| :eight_pointed_black_star: | Object is in progress. 
| :white_check_mark: | Object is completed. 


| Object | Percentage (of Bytes) | Functions Done / Total Functions | Percentage (Functions) | Status 
| ------------- | ------------- | ------------- | ------------- | ------------- 
| GXInit.o | 6.672519754170326% | 1 / 6 | 16.666666666666664% | :eight_pointed_black_star: 
| GXFifo.o | 13.102595797280594% | 2 / 17 | 11.76470588235294% | :eight_pointed_black_star: 
| GXAttr.o | 21.540656205420827% | 3 / 11 | 27.27272727272727% | :eight_pointed_black_star: 
| GXMisc.o | 18.152350081037277% | 2 / 25 | 8.0% | :eight_pointed_black_star: 
| GXGeometry.o | 45.380434782608695% | 2 / 9 | 22.22222222222222% | :eight_pointed_black_star: 
| GXFrameBuf.o | 0.0% | 0 / 15 | 0.0% | :x: 
| GXLight.o | 0.0% | 0 / 13 | 0.0% | :x: 
| GXTexture.o | 11.89801699716714% | 2 / 31 | 6.451612903225806% | :eight_pointed_black_star: 
| GXBump.o | 0.33783783783783783% | 1 / 10 | 10.0% | :eight_pointed_black_star: 
| GXTev.o | 0.0% | 0 / 16 | 0.0% | :x: 
| GXPixel.o | 63.896103896103895% | 11 / 12 | 91.66666666666666% | :eight_pointed_black_star: 
| GXDisplayList.o | 0.0% | 0 / 3 | 0.0% | :x: 
| GXTransform.o | 24.565217391304348% | 4 / 19 | 21.052631578947366% | :eight_pointed_black_star: 
| GXPerf.o | 0.0% | 0 / 3 | 0.0% | :x: 


# GXInit.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __GXDefaultTexRegionCallback | :x: |
| __GXDefaultTlutRegionCallback | :x: |
| __GXShutdown | :x: |
| __GXInitRevisionBits | :white_check_mark: |
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
| __GXIsGPFifoReady | :white_check_mark: |
| GXGetGPStatus | :x: |
| GXGetCPUFifo | :x: |
| GXGetFifoPtrs | :x: |
| GXGetFifoCount | :x: |
| GXGetFifoWrap | :x: |
| GXSetBreakPtCallback | :x: |
| GXEnableBreakPt | :x: |
| GXDisableBreakPt | :x: |
| __GXFifoInit | :x: |
| __GXCleanGPFifo | :white_check_mark: |


# GXAttr.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| GXSetVtxDesc | :x: |
| __GXSetVCD | :white_check_mark: |
| __GXCalculateVLim | :white_check_mark: |
| GXClearVtxDesc | :x: |
| GXSetVtxAttrFmt | :x: |
| GXSetVtxAttrFmtv | :x: |
| __GXSetVAT | :white_check_mark: |
| GXSetArray | :x: |
| GXInvalidateVtxCache | :x: |
| GXSetTexCoordGen2 | :x: |
| GXSetNumTexGens | :x: |


# GXMisc.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| GXSetMisc | :x: |
| GXFlush | :white_check_mark: |
| GXResetWriteGatherPipe | :x: |
| __GXAbort | :white_check_mark: |
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
| __GXSetDirtyState | :white_check_mark: |
| GXBegin | :x: |
| __GXSendFlushPrim | :x: |
| GXSetLineWidth | :x: |
| GXSetPointSize | :x: |
| GXEnableTexOffsets | :x: |
| GXSetCullMode | :x: |
| GXSetCoPlanar | :x: |
| __GXSetGenMode | :white_check_mark: |


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
| __SetSURegs | :white_check_mark: |
| __GXSetSUTexRegs | :white_check_mark: |
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
| __GXUpdateBPMask | :white_check_mark: |
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
| __GXSetProjection | :white_check_mark: |
| GXSetProjection | :x: |
| GXSetProjectionv | :x: |
| GXGetProjectionv | :x: |
| GXLoadPosMtxImm | :x: |
| GXLoadNrmMtxImm | :x: |
| GXSetCurrentMtx | :x: |
| GXLoadTexMtxImm | :x: |
| GXLoadTexMtxIndx | :x: |
| __GXSetViewport | :white_check_mark: |
| GXSetViewport | :x: |
| GXGetViewportv | :x: |
| GXSetZScaleOffset | :x: |
| GXSetScissor | :white_check_mark: |
| GXGetScissor | :x: |
| GXSetScissorBoxOffset | :x: |
| GXSetClipMode | :x: |
| __GXSetMatrixIndex | :white_check_mark: |


# GXPerf.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| GXSetGPMetric | :x: |
| GXClearGPMetric | :x: |
| GXReadXfRasMetric | :x: |


