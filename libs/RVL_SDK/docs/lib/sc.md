# sc
| Symbol | Meaning 
| ------------- | ------------- 
| :x: | Object has not yet been started. 
| :eight_pointed_black_star: | Object is in progress. 
| :white_check_mark: | Object is completed. 


| Object | Percentage (of Bytes) | Functions Done / Total Functions | Percentage (Functions) | Status 
| ------------- | ------------- | ------------- | ------------- | ------------- 
| scsystem.o | 35.87692307692308% | 9 / 22 | 40.909090909090914% | :eight_pointed_black_star: 
| scapi.o | 0.0% | 0 / 19 | 0.0% | :x: 
| scapi_prdinfo.o | 0.0% | 0 / 3 | 0.0% | :x: 


# scsystem.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| SCInit | :white_check_mark: |
| SCCheckStatus | :x: |
| SCReloadConfFileAsync | :white_check_mark: |
| OpenCallbackFromReload | :white_check_mark: |
| ReadCallbackFromReload | :white_check_mark: |
| CloseCallbackFromReload | :white_check_mark: |
| FinishFromReload | :white_check_mark: |
| CloseCallbackFromReloadError | :white_check_mark: |
| ParseConfBuf | :white_check_mark: |
| UnpackItem | :white_check_mark: |
| DeleteItemByID | :x: |
| CreateItemByID | :x: |
| SCFindByteArrayItem | :x: |
| SCReplaceByteArrayItem | :x: |
| SCReplaceIntegerItem | :x: |
| SCFindU8Item | :x: |
| SCFindS8Item | :x: |
| SCFindU32Item | :x: |
| SCReplaceU8Item | :x: |
| __SCFlushSyncCallback | :x: |
| SCFlushAsync | :x: |
| MyNandCallback | :x: |


# scapi.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| SCGetAspectRatio | :x: |
| SCGetDisplayOffsetH | :x: |
| SCGetEuRgb60Mode | :x: |
| SCGetIdleMode | :x: |
| SCGetLanguage | :x: |
| SCGetProgressiveMode | :x: |
| SCGetScreenSaverMode | :x: |
| SCGetSoundMode | :x: |
| SCGetCounterBias | :x: |
| SCGetBtDeviceInfoArray | :x: |
| SCSetBtDeviceInfoArray | :x: |
| SCGetBtCmpDevInfoArray | :x: |
| SCSetBtCmpDevInfoArray | :x: |
| SCGetBtDpdSensibility | :x: |
| SCGetWpadMotorMode | :x: |
| SCSetWpadMotorMode | :x: |
| SCGetWpadSensorBarPosition | :x: |
| SCGetWpadSpeakerVolume | :x: |
| SCSetWpadSpeakerVolume | :x: |


# scapi_prdinfo.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __SCF1 | :x: |
| SCGetProductArea | :x: |
| SCGetProductGameRegion | :x: |


