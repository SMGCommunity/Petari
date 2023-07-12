# sc
| Symbol | Meaning 
| ------------- | ------------- 
| :x: | Object has not yet been started. 
| :eight_pointed_black_star: | Object is in progress. 
| :white_check_mark: | Object is completed. 


| Object | Percentage (of Bytes) | Functions Done / Total Functions | Percentage (Functions) | Status 
| ------------- | ------------- | ------------- | ------------- | ------------- 
| scsystem.o | 74.46153846153845% | 18 / 22 | 81.81818181818183% | :eight_pointed_black_star: 
| scapi.o | 6.5743944636678195% | 2 / 19 | 10.526315789473683% | :eight_pointed_black_star: 
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
| DeleteItemByID | :white_check_mark: |
| CreateItemByID | :white_check_mark: |
| SCFindByteArrayItem | :white_check_mark: |
| SCReplaceByteArrayItem | :white_check_mark: |
| SCReplaceIntegerItem | :white_check_mark: |
| SCFindU8Item | :white_check_mark: |
| SCFindS8Item | :white_check_mark: |
| SCFindU32Item | :white_check_mark: |
| SCReplaceU8Item | :white_check_mark: |
| __SCFlushSyncCallback | :x: |
| SCFlushAsync | :x: |
| MyNandCallback | :x: |


# scapi.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| SCGetAspectRatio | :x: |
| SCGetDisplayOffsetH | :x: |
| SCGetEuRgb60Mode | :x: |
| SCGetIdleMode | :white_check_mark: |
| SCGetLanguage | :x: |
| SCGetProgressiveMode | :x: |
| SCGetScreenSaverMode | :x: |
| SCGetSoundMode | :x: |
| SCGetCounterBias | :white_check_mark: |
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


