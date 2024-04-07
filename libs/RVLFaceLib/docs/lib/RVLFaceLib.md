# RVLFaceLib
| Symbol | Meaning 
| ------------- | ------------- 
| :x: | Object has not yet been started. 
| :eight_pointed_black_star: | Object is in progress. 
| :white_check_mark: | Object is completed. 


| Object | Percentage (of Bytes) | Functions Done / Total Functions | Percentage (Functions) | Status 
| ------------- | ------------- | ------------- | ------------- | ------------- 
| RFL_System.o | 100.0% | 22 / 22 | 100.0% | :white_check_mark: 
| RFL_NANDLoader.o | 100.0% | 17 / 17 | 100.0% | :white_check_mark: 
| RFL_NANDAccess.o | 100.0% | 16 / 16 | 100.0% | :white_check_mark: 
| RFL_Model.o | 100.0% | 18 / 18 | 100.0% | :white_check_mark: 
| RFL_MakeTex.o | 100.0% | 10 / 10 | 100.0% | :white_check_mark: 
| RFL_Icon.o | 10.875331564986737% | 2 / 3 | 66.66666666666666% | :eight_pointed_black_star: 
| RFL_HiddenDatabase.o | 100.0% | 1 / 1 | 100.0% | :white_check_mark: 
| RFL_Database.o | 100.0% | 19 / 19 | 100.0% | :white_check_mark: 
| RFL_Controller.o | 100.0% | 2 / 2 | 100.0% | :white_check_mark: 
| RFL_MiddleDatabase.o | 100.0% | 3 / 3 | 100.0% | :white_check_mark: 
| RFL_DefaultDatabase.o | 100.0% | 1 / 1 | 100.0% | :white_check_mark: 
| RFL_DataUtility.o | 100.0% | 5 / 5 | 100.0% | :white_check_mark: 
| RFL_Format.o | 100.0% | 1 / 1 | 100.0% | :white_check_mark: 


# RFL_System.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| RFLGetWorkSize | :white_check_mark: |
| RFLInitResAsync | :white_check_mark: |
| RFLExit | :white_check_mark: |
| bootloadDB2Res_ | :white_check_mark: |
| RFLAvailable | :white_check_mark: |
| RFLiAlloc32 | :white_check_mark: |
| RFLiFree | :white_check_mark: |
| RFLiGetDBManager | :white_check_mark: |
| RFLiGetHDBManager | :white_check_mark: |
| RFLiGetLoader | :white_check_mark: |
| RFLiGetWorking | :white_check_mark: |
| RFLiSetWorking | :white_check_mark: |
| RFLiGetManager | :white_check_mark: |
| RFLGetAsyncStatus | :white_check_mark: |
| RFLGetLastReason | :white_check_mark: |
| RFLiGetAccInfo | :white_check_mark: |
| RFLiGetCtrlBufManager | :white_check_mark: |
| RFLiGetUseDeluxTex | :white_check_mark: |
| RFLiGetLastReason | :white_check_mark: |
| RFLiSetFileBroken | :white_check_mark: |
| RFLiNotFoundError | :white_check_mark: |
| RFLiNeedRepairError | :white_check_mark: |


# RFL_NANDLoader.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| RFLiInitLoader | :white_check_mark: |
| parseOnmemoryRes_ | :white_check_mark: |
| loadResRead2ndcallback_ | :white_check_mark: |
| errclosecallback_ | :white_check_mark: |
| loadResRead1stcallback_ | :white_check_mark: |
| loadResGetlengthcallback_ | :white_check_mark: |
| loadResOpencallback_ | :white_check_mark: |
| RFLiLoadResourceHeaderAsync | :white_check_mark: |
| getNANDFile_ | :white_check_mark: |
| RFLiGetTexSize | :white_check_mark: |
| RFLiLoadTexture | :white_check_mark: |
| RFLiGetShpTexSize | :white_check_mark: |
| RFLiLoadShpTexture | :white_check_mark: |
| RFLiGetShapeSize | :white_check_mark: |
| RFLiLoadShape | :white_check_mark: |
| RFLFreeCachedResource | :white_check_mark: |
| RFLIsResourceCached | :white_check_mark: |


# RFL_NANDAccess.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| RFLiInitAccessInfo | :white_check_mark: |
| RFLiExitAccessInfo | :white_check_mark: |
| RFLiIsWorking | :white_check_mark: |
| RFLiStartWorking | :white_check_mark: |
| RFLiEndWorking | :white_check_mark: |
| opencallback_ | :white_check_mark: |
| createcallback_ | :white_check_mark: |
| close2opencallback_ | :white_check_mark: |
| RFLiOpenAsync | :white_check_mark: |
| readcallback_ | :white_check_mark: |
| readseekcallback_ | :white_check_mark: |
| RFLiReadAsync | :white_check_mark: |
| closecallback_ | :white_check_mark: |
| RFLiCloseAsync | :white_check_mark: |
| getlengthcallback_ | :white_check_mark: |
| RFLiGetLengthAsync | :white_check_mark: |


# RFL_Model.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| RFLGetModelBufferSize | :white_check_mark: |
| RFLInitCharModel | :white_check_mark: |
| RFLiInitCharModel | :white_check_mark: |
| RFLSetMtx | :white_check_mark: |
| RFLSetExpression | :white_check_mark: |
| RFLGetFavoriteColor | :white_check_mark: |
| RFLiGetFacelineColor | :white_check_mark: |
| RFLLoadDrawSetting | :white_check_mark: |
| RFLDrawOpa | :white_check_mark: |
| RFLDrawXlu | :white_check_mark: |
| RFLLoadVertexSetting | :white_check_mark: |
| RFLLoadMaterialSetting | :white_check_mark: |
| RFLDrawOpaCore | :white_check_mark: |
| RFLDrawXluCore | :white_check_mark: |
| RFLiInitCharModelRes | :white_check_mark: |
| RFLiInitShapeRes | :white_check_mark: |
| RFLiInitTexRes | :white_check_mark: |
| RFLSetModelDrawDoneCallback | :white_check_mark: |


# RFL_MakeTex.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| RFLiSetupCopyTex | :white_check_mark: |
| RFLiMakeTexture | :white_check_mark: |
| RFLiSetup2DCameraAndParam | :white_check_mark: |
| RFLiSetTev4Mouth | :white_check_mark: |
| RFLiSetTev4Eye | :white_check_mark: |
| RFLiSetFaceParts | :white_check_mark: |
| RFLiCapture | :white_check_mark: |
| RFLiDrawQuad | :white_check_mark: |
| RFLiGetMaxMaskRsl | :white_check_mark: |
| RFLiGetMaskBufSize | :white_check_mark: |


# RFL_Icon.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| RFLMakeIcon | :white_check_mark: |
| RFLiMakeIcon | :x: |
| RFLSetIconDrawDoneCallback | :white_check_mark: |


# RFL_HiddenDatabase.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| RFLiInitHiddenDatabase | :white_check_mark: |


# RFL_Database.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| RFLiInitDatabase | :white_check_mark: |
| bootloadCheckCRCCb_ | :white_check_mark: |
| bootloadDBcallback_ | :white_check_mark: |
| bootloadDBopencallback_ | :white_check_mark: |
| RFLiBootLoadDatabaseAsync | :white_check_mark: |
| RFLiGetCharData | :white_check_mark: |
| convertRaw2InfoCore_ | :white_check_mark: |
| RFLiConvertRaw2Info | :white_check_mark: |
| RFLiConvertHRaw2Info | :white_check_mark: |
| convertInfo2RawCore_ | :white_check_mark: |
| RFLiGetCharInfo | :white_check_mark: |
| RFLIsAvailableOfficialData | :white_check_mark: |
| RFLiIsSameID | :white_check_mark: |
| RFLiIsValidID | :white_check_mark: |
| RFLiIsSpecialID | :white_check_mark: |
| RFLiIsTemporaryID | :white_check_mark: |
| RFLSearchOfficialData | :white_check_mark: |
| RFLiIsValidName2 | :white_check_mark: |
| alarmCheckCb_ | :white_check_mark: |


# RFL_Controller.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| RFLiInitCtrlBuf | :white_check_mark: |
| RFLiGetControllerData | :white_check_mark: |


# RFL_MiddleDatabase.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| RFLGetMiddleDBBufferSize | :white_check_mark: |
| RFLInitMiddleDB | :white_check_mark: |
| RFLiGetCharInfoMiddleDB | :white_check_mark: |


# RFL_DefaultDatabase.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| RFLiGetDefaultData | :white_check_mark: |


# RFL_DataUtility.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| RFLiCheckValidInfo | :white_check_mark: |
| RFLiIsValidOnNAND | :white_check_mark: |
| RFLiPickupCharInfo | :white_check_mark: |
| copyChar2Additional_ | :white_check_mark: |
| RFLGetAdditionalInfo | :white_check_mark: |


# RFL_Format.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| RFLiClearDBBuffer | :white_check_mark: |


