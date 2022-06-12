# Animation
| Symbol | Meaning 
| :x: | Object has not yet been started. 
| :eight_pointed_black_star: | Object is in progress. 
| :white_check_mark: | Object is completed. 


| Object | Percentage (of Bytes) | Functions Done / Total Functions | Percentage (Functions) | Status 
| ------------- | ------------- | ------------- | ------------- | ------------- 
| AnmPlayer.o | 8.095238095238095% | 9 / 32 | 28.125% | :eight_pointed_black_star: 
| BckCtrl.o | 18.295218295218298% | 3 / 9 | 33.33333333333333% | :eight_pointed_black_star: 
| BpkPlayer.o | 0.0% | 0 / 3 | 0.0% | :x: 
| BrkPlayer.o | 0.0% | 0 / 3 | 0.0% | :x: 
| BtkPlayer.o | 0.0% | 0 / 3 | 0.0% | :x: 
| BtpPlayer.o | 0.0% | 0 / 3 | 0.0% | :x: 
| BvaPlayer.o | 0.0% | 0 / 3 | 0.0% | :x: 
| LayoutAnmPlayer.o | 0.0% | 0 / 5 | 0.0% | :x: 
| MaterialAnmBuffer.o | 0.0% | 0 / 32 | 0.0% | :x: 
| XanimeCore.o | 4.778761061946903% | 5 / 46 | 10.869565217391305% | :eight_pointed_black_star: 
| XanimePlayer.o | 0.0% | 0 / 44 | 0.0% | :x: 
| XanimeResource.o | 0.0% | 0 / 18 | 0.0% | :x: 


# AnmPlayer.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| setEuler__Q29JGeometry9TQuat4&lt;f&gt;Ffff | :x: |
| setRotate__Q29JGeometry9TQuat4&lt;f&gt;FRCQ29JGeometry8TVec3&lt;f&gt;RCQ29JGeometry8TVec3&lt;f&gt; | :x: |
| slerp__Q29JGeometry9TQuat4&lt;f&gt;FRCQ29JGeometry9TQuat4&lt;f&gt;f | :x: |
| normalize__Q29JGeometry9TQuat4&lt;f&gt;FRCQ29JGeometry9TQuat4&lt;f&gt; | :x: |
| getEuler__Q29JGeometry9TQuat4&lt;f&gt;CFRQ29JGeometry8TVec3&lt;f&gt; | :x: |
| __ct__Q29JGeometry64TPosition3&lt;Q29JGeometry38TMatrix34&lt;Q29JGeometry13SMatrix34C&lt;f&gt;&gt;&gt;Fv | :x: |
| identity__Q29JGeometry38TMatrix34&lt;Q29JGeometry13SMatrix34C&lt;f&gt;&gt;Fv | :x: |
| mult__Q29JGeometry38TMatrix34&lt;Q29JGeometry13SMatrix34C&lt;f&gt;&gt;CFRCQ29JGeometry8TVec3&lt;f&gt;RQ29JGeometry8TVec3&lt;f&gt; | :x: |
| multTranspose__Q29JGeometry38TMatrix34&lt;Q29JGeometry13SMatrix34C&lt;f&gt;&gt;CFRCQ29JGeometry8TVec3&lt;f&gt;RQ29JGeometry8TVec3&lt;f&gt; | :x: |
| invert__Q29JGeometry38TMatrix34&lt;Q29JGeometry13SMatrix34C&lt;f&gt;&gt;FRCQ29JGeometry38TMatrix34&lt;Q29JGeometry13SMatrix34C&lt;f&gt;&gt; | :x: |
| concat__Q29JGeometry38TMatrix34&lt;Q29JGeometry13SMatrix34C&lt;f&gt;&gt;FRCQ29JGeometry13SMatrix34C&lt;f&gt;RCQ29JGeometry13SMatrix34C&lt;f&gt; | :x: |
| concat__Q29JGeometry38TMatrix34&lt;Q29JGeometry13SMatrix34C&lt;f&gt;&gt;FRCQ29JGeometry13SMatrix34C&lt;f&gt; | :x: |
| __ct__Q29JGeometry8TVec3&lt;f&gt;Ff | :x: |
| set&lt;f&gt;__Q29JGeometry8TVec3&lt;f&gt;Ffff_v | :x: |
| epsilonEquals__Q29JGeometry8TUtil&lt;f&gt;Ffff | :x: |
| sqrt__Q29JGeometry8TUtil&lt;f&gt;Ff | :x: |
| set__Q29JGeometry13SMatrix34C&lt;f&gt;Fffffffffffff | :x: |
| getQuat__Q29JGeometry64TRotation3&lt;Q29JGeometry38TMatrix34&lt;Q29JGeometry13SMatrix34C&lt;f&gt;&gt;&gt;CFRQ29JGeometry9TQuat4&lt;f&gt; | :x: |
| set&lt;f&gt;__Q29JGeometry8TVec4&lt;f&gt;Fffff_v | :x: |
| __as__Q29JGeometry9TQuat4&lt;f&gt;FRCQ29JGeometry9TQuat4&lt;f&gt; | :x: |
| __ct__13AnmPlayerBaseFPC8ResTable | :white_check_mark: |
| update__13AnmPlayerBaseFv | :white_check_mark: |
| reflectFrame__13AnmPlayerBaseFv | :x: |
| start__13AnmPlayerBaseFPCc | :x: |
| stop__13AnmPlayerBaseFv | :white_check_mark: |
| isPlaying__13AnmPlayerBaseCFPCc | :white_check_mark: |
| __ct__21MaterialAnmPlayerBaseFPC8ResTableP12J3DModelData | :white_check_mark: |
| beginDiff__21MaterialAnmPlayerBaseFv | :white_check_mark: |
| endDiff__21MaterialAnmPlayerBaseFv | :white_check_mark: |
| __dt__12J3DFrameCtrlFv | :x: |
| changeAnimation__13AnmPlayerBaseFP10J3DAnmBase | :white_check_mark: |
| stopAnimation__13AnmPlayerBaseFv | :white_check_mark: |


# BckCtrl.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| getValue&lt;l&gt;__12JMapInfoIterCFPCcPl_b | :x: |
| __ct__7BckCtrlFP14ResourceHolderPCc | :x: |
| add__7BckCtrlFRC11BckCtrlData | :x: |
| __as__11BckCtrlDataFRC11BckCtrlData | :x: |
| overWrite__7BckCtrlFRC11BckCtrlData | :white_check_mark: |
| changeBckSetting__7BckCtrlCFPCcP12XanimePlayer | :white_check_mark: |
| find__7BckCtrlCFPCc | :x: |
| reflectBckCtrlData__15BckCtrlFunctionFRC11BckCtrlDataP12XanimePlayer | :x: |
| __ct__11BckCtrlDataFv | :white_check_mark: |


# BpkPlayer.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__9BpkPlayerFPC14ResourceHolderP12J3DModelData | :x: |
| attach__9BpkPlayerFP10J3DAnmBaseP12J3DModelData | :x: |
| detach__9BpkPlayerFP10J3DAnmBaseP12J3DModelData | :x: |


# BrkPlayer.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__9BrkPlayerFPC14ResourceHolderP12J3DModelData | :x: |
| attach__9BrkPlayerFP10J3DAnmBaseP12J3DModelData | :x: |
| detach__9BrkPlayerFP10J3DAnmBaseP12J3DModelData | :x: |


# BtkPlayer.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__9BtkPlayerFPC14ResourceHolderP12J3DModelData | :x: |
| attach__9BtkPlayerFP10J3DAnmBaseP12J3DModelData | :x: |
| detach__9BtkPlayerFP10J3DAnmBaseP12J3DModelData | :x: |


# BtpPlayer.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__9BtpPlayerFPC14ResourceHolderP12J3DModelData | :x: |
| attach__9BtpPlayerFP10J3DAnmBaseP12J3DModelData | :x: |
| detach__9BtpPlayerFP10J3DAnmBaseP12J3DModelData | :x: |


# BvaPlayer.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__9BvaPlayerFPC8ResTableP8J3DModel | :x: |
| calc__9BvaPlayerFv | :x: |
| getAnmVisibility__9BvaPlayerFv | :x: |


# LayoutAnmPlayer.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__15LayoutAnmPlayerFPC13LayoutManager | :x: |
| movement__15LayoutAnmPlayerFv | :x: |
| reflectFrame__15LayoutAnmPlayerFv | :x: |
| start__15LayoutAnmPlayerFPCc | :x: |
| stop__15LayoutAnmPlayerFv | :x: |


# MaterialAnmBuffer.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__17MaterialAnmBufferFPC14ResourceHolderP12J3DModelDatab | :x: |
| getDiffFlag__17MaterialAnmBufferCFl | :x: |
| getAllocMaterialAnmNum__17MaterialAnmBufferCFP12J3DModelDatab | :x: |
| searchUpdateMaterialID__17MaterialAnmBufferFPC14ResourceHolderP12J3DModelData | :x: |
| setDiffFlag__17MaterialAnmBufferFPC14ResourceHolder | :x: |
| getDifferedMaterialNum__17MaterialAnmBufferCFPC12J3DModelData | :x: |
| attachMaterialAnmBuffer__17MaterialAnmBufferFP12J3DModelDatab | :x: |
| modifyDiffFlagBrk__FPUlPC15J3DAnmTevRegKeybPCc | :x: |
| onDiffFlagBpk__2MRFPUlPC14J3DAnmColorKeyPCc | :x: |
| modifyDiffFlag&lt;14J3DAnmColorKey&gt;__FPUlPC14J3DAnmColorKey11J3DDiffFlagbPCcPCc_v | :x: |
| offDiffFlagBpk__2MRFPUlPC14J3DAnmColorKeyPCc | :x: |
| onDiffFlagBtp__2MRFPUlPC16J3DAnmTexPatternPCc | :x: |
| modifyDiffFlag&lt;16J3DAnmTexPattern&gt;__FPUlPC16J3DAnmTexPattern11J3DDiffFlagbPCcPCc_v | :x: |
| offDiffFlagBtp__2MRFPUlPC16J3DAnmTexPatternPCc | :x: |
| onDiffFlagBtk__2MRFPUlPC19J3DAnmTextureSRTKeyPCc | :x: |
| modifyDiffFlag&lt;19J3DAnmTextureSRTKey&gt;__FPUlPC19J3DAnmTextureSRTKey11J3DDiffFlagbPCcPCc_v | :x: |
| offDiffFlagBtk__2MRFPUlPC19J3DAnmTextureSRTKeyPCc | :x: |
| onDiffFlagBrk__2MRFPUlPC15J3DAnmTevRegKeyPCc | :x: |
| offDiffFlagBrk__2MRFPUlPC15J3DAnmTevRegKeyPCc | :x: |
| __ct__14J3DMaterialAnmFv | :x: |
| __ct__14J3DMatColorAnmFv | :x: |
| __dt__14J3DMatColorAnmFv | :x: |
| __ct__12J3DTexMtxAnmFv | :x: |
| __dt__12J3DTexMtxAnmFv | :x: |
| __ct__11J3DTexNoAnmFv | :x: |
| __dt__11J3DTexNoAnmFv | :x: |
| __ct__14J3DTevColorAnmFv | :x: |
| __dt__14J3DTevColorAnmFv | :x: |
| __ct__15J3DTevKColorAnmFv | :x: |
| __dt__15J3DTevKColorAnmFv | :x: |
| __dt__14J3DMaterialAnmFv | :x: |
| calc__11J3DTexNoAnmCFPUs | :x: |


# XanimeCore.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__Q29JGeometry8TVec3&lt;f&gt;Fv | :x: |
| zero__Q29JGeometry8TVec3&lt;f&gt;Fv | :x: |
| __as__14XtransformInfoFRC14XtransformInfo | :x: |
| __as__Q29JGeometry8TVec3&lt;f&gt;FRCQ29JGeometry8TVec3&lt;f&gt; | :x: |
| __ml__Q29JGeometry8TVec3&lt;f&gt;CFf | :x: |
| __ct__Q29JGeometry8TVec3&lt;f&gt;FRCQ29JGeometry8TVec3&lt;f&gt; | :x: |
| __apl__Q29JGeometry8TVec3&lt;f&gt;FRCQ29JGeometry8TVec3&lt;f&gt; | :x: |
| __ct&lt;f&gt;__Q29JGeometry8TVec3&lt;f&gt;Ffff_Pv | :x: |
| __mi__Q29JGeometry8TVec3&lt;f&gt;CFRCQ29JGeometry8TVec3&lt;f&gt; | :x: |
| __as__10QuaternionFRC10Quaternion | :x: |
| init__11XanimeTrackFv | :white_check_mark: |
| __ct__10XjointInfoFv | :white_check_mark: |
| __ct__15XjointTransformFv | :x: |
| shareJointTransform__10XanimeCoreFPC10XanimeCore | :x: |
| enableJointTransform__10XanimeCoreFP12J3DModelData | :x: |
| reconfigJointTransform__10XanimeCoreFP12J3DModelData | :x: |
| initMember__10XanimeCoreFUl | :x: |
| __ct__10XanimeCoreFUlUlUc | :white_check_mark: |
| __ct__10XanimeCoreFUlP10XanimeCore | :white_check_mark: |
| doFreeze__10XanimeCoreFv | :x: |
| setBck__10XanimeCoreFUlP15J3DAnmTransform | :x: |
| setWeight__10XanimeCoreFUcf | :white_check_mark: |
| calcBlend__10XanimeCoreFPQ29JGeometry8TVec3&lt;f&gt;PQ29JGeometry8TVec3&lt;f&gt; | :x: |
| calcSingle__10XanimeCoreFPQ29JGeometry8TVec3&lt;f&gt;PQ29JGeometry8TVec3&lt;f&gt; | :x: |
| calcBlendSpecial__10XanimeCoreFv | :x: |
| updateFrame__10XanimeCoreFv | :x: |
| calcScaleBlendMaya__10XanimeCoreFRCQ29JGeometry8TVec3&lt;f&gt;RCQ29JGeometry8TVec3&lt;f&gt; | :x: |
| calcScaleBlendMayaNoTransform__10XanimeCoreFRCQ29JGeometry8TVec3&lt;f&gt;RCQ29JGeometry8TVec3&lt;f&gt; | :x: |
| calcScaleBlendSI__10XanimeCoreFRCQ29JGeometry8TVec3&lt;f&gt;RCQ29JGeometry8TVec3&lt;f&gt; | :x: |
| calcScaleBlendBasic__10XanimeCoreFRCQ29JGeometry8TVec3&lt;f&gt;RCQ29JGeometry8TVec3&lt;f&gt; | :x: |
| calcScaleBlendSpecial__10XanimeCoreFv | :x: |
| freezeCopy__10XanimeCoreFP12J3DModelDataP10XanimeCoreUlUl | :x: |
| initT__10XanimeCoreFP12J3DModelData | :x: |
| fixT__10XanimeCoreFPQ29JGeometry8TVec3&lt;f&gt; | :x: |
| __dt__10XanimeCoreFv | :x: |
| __ct__14XtransformInfoFv | :x: |
| calc__10XanimeCoreFv | :x: |
| init__10XanimeCoreFRC3VecRA3_A4_Cf | :x: |
| __as__16J3DTransformInfoFRC16J3DTransformInfo | :x: |
| getWeight__10J3DMtxCalcCFUc | :x: |
| getAnmTransform__10J3DMtxCalcFv | :x: |
| getAnmTransform__10J3DMtxCalcFUc | :x: |
| setAnmTransform__10J3DMtxCalcFP15J3DAnmTransform | :x: |
| setAnmTransform__10J3DMtxCalcFUcP15J3DAnmTransform | :x: |
| setWeight__10J3DMtxCalcFUcf | :x: |
| fastReciprocal__5JMathFf | :x: |


# XanimePlayer.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| init__12XanimePlayerFv | :x: |
| __ct__12XanimePlayerFP8J3DModelP19XanimeResourceTable | :x: |
| __ct__12XanimePlayerFP8J3DModelP19XanimeResourceTableP12XanimePlayer | :x: |
| stopAnimation__12XanimePlayerFv | :x: |
| stopAnimation__12XanimePlayerFPCc | :x: |
| changeTrackAnimation__12XanimePlayerFUcPCc | :x: |
| changeTrackAnimation__12XanimePlayerFUcUl | :x: |
| changeAnimationBck__12XanimePlayerFPCc | :x: |
| changeSimpleBck__12XanimePlayerFPCc | :x: |
| changeAnimation__12XanimePlayerFPCc | :x: |
| changeAnimationByHash__12XanimePlayerFUl | :x: |
| swapFrameCtrl__12XanimePlayerFPC15XanimeGroupInfo | :x: |
| changeAnimation__12XanimePlayerFPC15XanimeGroupInfo | :x: |
| prepareAnimation__12XanimePlayerFPC15XanimeGroupInfo | :x: |
| runNextAnimation__12XanimePlayerFv | :x: |
| changeAnimationSimple__12XanimePlayerFP15J3DAnmTransform | :x: |
| changeSpeed__12XanimePlayerFf | :x: |
| changeInterpoleFrame__12XanimePlayerFl | :x: |
| changeTrackWeight__12XanimePlayerFUlf | :x: |
| calcAnm__12XanimePlayerFUs | :x: |
| setModel__12XanimePlayerFP8J3DModel | :x: |
| overWriteMtxCalc__12XanimePlayerFUs | :x: |
| clearMtxCalc__12XanimePlayerFUs | :x: |
| clearAnm__12XanimePlayerFUs | :x: |
| updateBeforeMovement__12XanimePlayerFv | :x: |
| updateAfterMovement__12XanimePlayerFv | :x: |
| updateInterpoleRatio__12XanimePlayerFv | :x: |
| getMainAnimationTrans__12XanimePlayerCFUlPQ29JGeometry8TVec3&lt;f&gt; | :x: |
| isRun__12XanimePlayerCFPCc | :x: |
| isTerminate__12XanimePlayerCFPCc | :x: |
| isTerminate__12XanimePlayerCFv | :x: |
| setDefaultAnimation__12XanimePlayerFPCc | :x: |
| runDefaultAnimation__12XanimePlayerFv | :x: |
| isAnimationRunSimple__12XanimePlayerCFv | :x: |
| getCurrentAnimationName__12XanimePlayerCFv | :x: |
| getDefaultAnimationName__12XanimePlayerCFv | :x: |
| getCurrentBckName__12XanimePlayerCFv | :x: |
| getNameStringPointer__12XanimePlayerCFPCc | :x: |
| checkPass__12XanimePlayerCFf | :x: |
| getSimpleGroup__12XanimePlayerCFv | :x: |
| duplicateSimpleGroup__12XanimePlayerFv | :x: |
| __ct__15XanimeFrameCtrlFv | :x: |
| changeCurrentAnimation__12XanimePlayerFPC15XanimeGroupInfo | :x: |
| __dt__15XanimeFrameCtrlFv | :x: |


# XanimeResource.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| init__19XanimeResourceTableFv | :x: |
| getGroupInfo__19XanimeResourceTableCFPCc | :x: |
| getSimpleIndex__19XanimeResourceTableCFPCc | :x: |
| getIndexFromHash__19XanimeResourceTableCFUl | :x: |
| getGroupInfoFromHash__19XanimeResourceTableCFUl | :x: |
| getGroupIndex__19XanimeResourceTableCFPCc | :x: |
| getSingleIndex__19XanimeResourceTableCFPCc | :x: |
| search__19XanimeResourceTableCFPP14XanimeBckTablePCcUl | :x: |
| __ct__19XanimeResourceTableFP14ResourceHolder | :x: |
| swapBckName__19XanimeResourceTableCFPCcP15XanimeSwapTable | :x: |
| initGroupInfo__19XanimeResourceTableFP14ResourceHolderP15XanimeGroupInfoP13XanimeAuxInfoP13XanimeOfsInfoP14XanimeBckTableP15XanimeBckTable2P15XanimeBckTable3P15XanimeBckTable4P15XanimeSwapTable | :x: |
| __ct__19XanimeResourceTableFP14ResourceHolderP15XanimeGroupInfoP13XanimeAuxInfoP13XanimeOfsInfoP14XanimeBckTableP15XanimeBckTable2P15XanimeBckTable3P15XanimeBckTable4P15XanimeSwapTable | :x: |
| getIndex__19XanimeResourceTableCFP15XanimeDirectoryPCc | :x: |
| getGroupInfo__19XanimeResourceTableCFPCcP15XanimeDirectory | :x: |
| createSortTable__19XanimeResourceTableFv | :x: |
| init__15XanimeGroupInfoFv | :x: |
| findResMotion__19XanimeResourceTableCFPCc | :x: |
| findStringMotion__19XanimeResourceTableCFPCc | :x: |


