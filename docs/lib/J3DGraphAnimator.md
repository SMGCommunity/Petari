# J3DGraphAnimator
| Symbol | Meaning 
| ------------- | ------------- 
| :x: | Object has not yet been started. 
| :eight_pointed_black_star: | Object is in progress. 
| :white_check_mark: | Object is completed. 


| Object | Percentage (of Bytes) | Functions Done / Total Functions | Percentage (Functions) | Status 
| ------------- | ------------- | ------------- | ------------- | ------------- 
| J3DShapeTable.o | 0.0% | 0 / 2 | 0.0% | :x: 
| J3DJointTree.o | 0.0% | 0 / 6 | 0.0% | :x: 
| J3DModelData.o | 0.0% | 0 / 7 | 0.0% | :x: 
| J3DMtxBuffer.o | 0.0% | 0 / 14 | 0.0% | :x: 
| J3DModel.o | 0.0% | 0 / 21 | 0.0% | :x: 
| J3DAnimation.o | 0.0% | 0 / 74 | 0.0% | :x: 
| J3DMaterialAnm.o | 0.0% | 0 / 7 | 0.0% | :x: 
| J3DSkinDeform.o | 0.0% | 0 / 2 | 0.0% | :x: 
| J3DCluster.o | 0.0% | 0 / 8 | 0.0% | :x: 
| J3DJoint.o | 0.0% | 0 / 10 | 0.0% | :x: 
| J3DMaterialAttach.o | 0.0% | 0 / 13 | 0.0% | :x: 


# J3DShapeTable.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| initShapeNodes__13J3DShapeTableFP14J3DDrawMtxDataP13J3DVertexData | :x: |
| sortVcdVatCmd__13J3DShapeTableFv | :x: |


# J3DJointTree.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__12J3DJointTreeFv | :x: |
| makeHierarchy__12J3DJointTreeFP8J3DJointPPC17J3DModelHierarchyP16J3DMaterialTableP13J3DShapeTable | :x: |
| findImportantMtxIndex__12J3DJointTreeFv | :x: |
| calc__12J3DJointTreeFP12J3DMtxBufferRC3VecRA3_A4_Cf | :x: |
| setMtxBuffer__10J3DMtxCalcFP12J3DMtxBuffer | :x: |
| __dt__12J3DJointTreeFv | :x: |


# J3DModelData.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| clear__12J3DModelDataFv | :x: |
| __ct__12J3DModelDataFv | :x: |
| newSharedDisplayList__12J3DModelDataFUl | :x: |
| indexToPtr__12J3DModelDataFv | :x: |
| syncJ3DSysFlags__12J3DModelDataCFv | :x: |
| __dt__13J3DShapeTableFv | :x: |
| __dt__12J3DModelDataFv | :x: |


# J3DMtxBuffer.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| initialize__12J3DMtxBufferFv | :x: |
| create__12J3DMtxBufferFP12J3DModelDataUl | :x: |
| createAnmMtx__12J3DMtxBufferFP12J3DModelData | :x: |
| createWeightEnvelopeMtx__12J3DMtxBufferFP12J3DModelData | :x: |
| setNoUseDrawMtx__12J3DMtxBufferFv | :x: |
| createDoubleDrawMtx__12J3DMtxBufferFP12J3DModelDataUl | :x: |
| createBumpMtxArray__12J3DMtxBufferFP12J3DModelDataUl | :x: |
| calcWeightEnvelopeMtx__12J3DMtxBufferFv | :x: |
| calcDrawMtx__12J3DMtxBufferFUlRC3VecRA3_A4_Cf | :x: |
| calcNrmMtx__12J3DMtxBufferFv | :x: |
| calcBBoardMtx__12J3DMtxBufferFv | :x: |
| J3DCalcViewBaseMtx__FPA4_fRC3VecRA3_A4_CfPA4_f | :x: |
| setNrmMtx__12J3DMtxBufferFiPA4_f | :x: |
| getNrmMtx__12J3DMtxBufferFi | :x: |


# J3DModel.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| initialize__8J3DModelFv | :x: |
| entryModelData__8J3DModelFP12J3DModelDataUlUl | :x: |
| createShapePacket__8J3DModelFP12J3DModelData | :x: |
| createMatPacket__8J3DModelFP12J3DModelDataUl | :x: |
| lock__8J3DModelFv | :x: |
| unlock__8J3DModelFv | :x: |
| makeDL__8J3DModelFv | :x: |
| calcMaterial__8J3DModelFv | :x: |
| calcDiffTexMtx__8J3DModelFv | :x: |
| calcAnmMtx__8J3DModelFv | :x: |
| calcWeightEnvelopeMtx__8J3DModelFv | :x: |
| update__8J3DModelFv | :x: |
| calc__8J3DModelFv | :x: |
| entry__8J3DModelFv | :x: |
| viewCalc__8J3DModelFv | :x: |
| calcBumpMtx__8J3DModelFv | :x: |
| calcBBoardMtx__8J3DModelFv | :x: |
| prepareShapePackets__8J3DModelFv | :x: |
| getNrmMtxPtr__8J3DModelFv | :x: |
| getBumpMtxPtr__8J3DModelFi | :x: |
| __dt__12J3DMtxBufferFv | :x: |


# J3DAnimation.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| init__12J3DFrameCtrlFs | :x: |
| checkPass__12J3DFrameCtrlFf | :x: |
| update__12J3DFrameCtrlFv | :x: |
| __ct__15J3DAnmTransformFsPfPsPf | :x: |
| getTransform__19J3DAnmTransformFullCFUsP16J3DTransformInfo | :x: |
| getTransform__27J3DAnmTransformFullWithLerpCFUsP16J3DTransformInfo | :x: |
| calcTransform__18J3DAnmTransformKeyCFfUsP16J3DTransformInfo | :x: |
| J3DGetKeyFrameInterpolation&lt;s&gt;__FfP18J3DAnmKeyTableBasePs_f | :x: |
| J3DHermiteInterpolation__FfPCsPCsPCsPCsPCsPCs | :x: |
| J3DGetKeyFrameInterpolation&lt;f&gt;__FfP18J3DAnmKeyTableBasePf_f | :x: |
| J3DHermiteInterpolation__FfPCfPCfPCfPCfPCfPCf | :x: |
| __ct__19J3DAnmTextureSRTKeyFv | :x: |
| calcTransform__19J3DAnmTextureSRTKeyCFfUsP17J3DTextureSRTInfo | :x: |
| getWeight__17J3DAnmClusterFullCFUs | :x: |
| getWeight__16J3DAnmClusterKeyCFUs | :x: |
| __ct__14J3DAnmVtxColorFv | :x: |
| __ct__18J3DAnmVtxColorFullFv | :x: |
| getColor__18J3DAnmVtxColorFullCFUcUsP8_GXColor | :x: |
| __ct__17J3DAnmVtxColorKeyFv | :x: |
| getColor__17J3DAnmVtxColorKeyCFUcUsP8_GXColor | :x: |
| __ct__11J3DAnmColorFv | :x: |
| searchUpdateMaterialID__11J3DAnmColorFP16J3DMaterialTable | :x: |
| searchUpdateMaterialID__11J3DAnmColorFP12J3DModelData | :x: |
| __ct__15J3DAnmColorFullFv | :x: |
| getColor__15J3DAnmColorFullCFUsP8_GXColor | :x: |
| __ct__14J3DAnmColorKeyFv | :x: |
| getColor__14J3DAnmColorKeyCFUsP8_GXColor | :x: |
| __ct__15J3DAnmTevRegKeyFv | :x: |
| __ct__16J3DAnmTexPatternFv | :x: |
| getTexNo__16J3DAnmTexPatternCFUsPUs | :x: |
| getVisibility__20J3DAnmVisibilityFullCFUsPUc | :x: |
| searchUpdateMaterialID__16J3DAnmTexPatternFP16J3DMaterialTable | :x: |
| searchUpdateMaterialID__16J3DAnmTexPatternFP12J3DModelData | :x: |
| searchUpdateMaterialID__19J3DAnmTextureSRTKeyFP16J3DMaterialTable | :x: |
| searchUpdateMaterialID__19J3DAnmTextureSRTKeyFP12J3DModelData | :x: |
| getTevColorReg__15J3DAnmTevRegKeyCFUsP11_GXColorS10 | :x: |
| getTevKonstReg__15J3DAnmTevRegKeyCFUsP8_GXColor | :x: |
| searchUpdateMaterialID__15J3DAnmTevRegKeyFP16J3DMaterialTable | :x: |
| searchUpdateMaterialID__15J3DAnmTevRegKeyFP12J3DModelData | :x: |
| __ct__10J3DAnmBaseFv | :x: |
| __dt__10J3DAnmBaseFv | :x: |
| __ct__10J3DAnmBaseFs | :x: |
| __dt__14J3DAnmVtxColorFv | :x: |
| __dt__11J3DAnmColorFv | :x: |
| getKind__16J3DAnmTexPatternCFv | :x: |
| __dt__16J3DAnmTexPatternFv | :x: |
| getKind__15J3DAnmTevRegKeyCFv | :x: |
| __dt__15J3DAnmTevRegKeyFv | :x: |
| getKind__14J3DAnmColorKeyCFv | :x: |
| __dt__14J3DAnmColorKeyFv | :x: |
| getKind__15J3DAnmColorFullCFv | :x: |
| __dt__15J3DAnmColorFullFv | :x: |
| getColor__11J3DAnmColorCFUsP8_GXColor | :x: |
| getKind__11J3DAnmColorCFv | :x: |
| getKind__17J3DAnmVtxColorKeyCFv | :x: |
| __dt__17J3DAnmVtxColorKeyFv | :x: |
| getKind__18J3DAnmVtxColorFullCFv | :x: |
| __dt__18J3DAnmVtxColorFullFv | :x: |
| getColor__14J3DAnmVtxColorCFUcUsP8_GXColor | :x: |
| getKind__14J3DAnmVtxColorCFv | :x: |
| getKind__16J3DAnmClusterKeyCFv | :x: |
| __dt__16J3DAnmClusterKeyFv | :x: |
| getKind__17J3DAnmClusterFullCFv | :x: |
| __dt__17J3DAnmClusterFullFv | :x: |
| getKind__19J3DAnmTextureSRTKeyCFv | :x: |
| __dt__19J3DAnmTextureSRTKeyFv | :x: |
| __dt__15J3DAnmTransformFv | :x: |
| getKind__27J3DAnmTransformFullWithLerpCFv | :x: |
| __dt__27J3DAnmTransformFullWithLerpFv | :x: |
| __dt__19J3DAnmTransformFullFv | :x: |
| getKind__19J3DAnmTransformFullCFv | :x: |
| getKind__15J3DAnmTransformCFv | :x: |
| OSf32tou8 | :x: |
| OSf32tos16 | :x: |


# J3DMaterialAnm.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| initialize__14J3DMaterialAnmFv | :x: |
| calc__14J3DMaterialAnmCFP11J3DMaterial | :x: |
| setMatColorAnm__14J3DMaterialAnmFiP14J3DMatColorAnm | :x: |
| setTexMtxAnm__14J3DMaterialAnmFiP12J3DTexMtxAnm | :x: |
| setTexNoAnm__14J3DMaterialAnmFiP11J3DTexNoAnm | :x: |
| setTevColorAnm__14J3DMaterialAnmFiP14J3DTevColorAnm | :x: |
| setTevKColorAnm__14J3DMaterialAnmFiP15J3DTevKColorAnm | :x: |


# J3DSkinDeform.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| deform__13J3DSkinDeformFP8J3DModel | :x: |
| calc__15J3DVtxColorCalcFP8J3DModel | :x: |


# J3DCluster.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| deform__13J3DDeformDataFP8J3DModel | :x: |
| deform__13J3DDeformDataFP15J3DVertexBuffer | :x: |
| deform__11J3DDeformerFP15J3DVertexBufferUs | :x: |
| deform_VtxPosF32__11J3DDeformerFP15J3DVertexBufferP10J3DClusterP13J3DClusterKeyPf | :x: |
| deform_VtxNrmF32__11J3DDeformerFP15J3DVertexBufferP10J3DClusterP13J3DClusterKeyPf | :x: |
| deform__11J3DDeformerFP15J3DVertexBufferUsPf | :x: |
| normalizeWeight__11J3DDeformerFiPf | :x: |
| getWeight__13J3DAnmClusterCFUs | :x: |


# J3DJoint.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| init__25J3DMtxCalcJ3DSysInitBasicFRC3VecRA3_A4_Cf | :x: |
| init__24J3DMtxCalcJ3DSysInitMayaFRC3VecRA3_A4_Cf | :x: |
| calcTransform__28J3DMtxCalcCalcTransformBasicFRC16J3DTransformInfo | :x: |
| calcTransform__32J3DMtxCalcCalcTransformSoftimageFRC16J3DTransformInfo | :x: |
| calcTransform__27J3DMtxCalcCalcTransformMayaFRC16J3DTransformInfo | :x: |
| appendChild__8J3DJointFP8J3DJoint | :x: |
| __ct__8J3DJointFv | :x: |
| entryIn__8J3DJointFv | :x: |
| recursiveCalc__8J3DJointFv | :x: |
| checkScaleOne__FRC3Vec | :x: |


# J3DMaterialAttach.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| clear__16J3DMaterialTableFv | :x: |
| __ct__16J3DMaterialTableFv | :x: |
| __dt__16J3DMaterialTableFv | :x: |
| removeMatColorAnimator__16J3DMaterialTableFP11J3DAnmColor | :x: |
| removeTexNoAnimator__16J3DMaterialTableFP16J3DAnmTexPattern | :x: |
| removeTexMtxAnimator__16J3DMaterialTableFP19J3DAnmTextureSRTKey | :x: |
| removeTevRegAnimator__16J3DMaterialTableFP15J3DAnmTevRegKey | :x: |
| createTexMtxForAnimator__16J3DMaterialTableFP19J3DAnmTextureSRTKey | :x: |
| entryMatColorAnimator__16J3DMaterialTableFP11J3DAnmColor | :x: |
| entryTexNoAnimator__16J3DMaterialTableFP16J3DAnmTexPattern | :x: |
| entryTexMtxAnimator__16J3DMaterialTableFP19J3DAnmTextureSRTKey | :x: |
| entryTevRegAnimator__16J3DMaterialTableFP15J3DAnmTevRegKey | :x: |
| getMaterialAnm__11J3DMaterialFv | :x: |


