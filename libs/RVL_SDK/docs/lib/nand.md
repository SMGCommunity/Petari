# nand
| Symbol | Meaning 
| ------------- | ------------- 
| :x: | Object has not yet been started. 
| :eight_pointed_black_star: | Object is in progress. 
| :white_check_mark: | Object is completed. 


| Object | Percentage (of Bytes) | Functions Done / Total Functions | Percentage (Functions) | Status 
| ------------- | ------------- | ------------- | ------------- | ------------- 
| nand.o | 0.0% | 0 / 30 | 0.0% | :x: 
| NANDOpenClose.o | 0.0% | 0 / 17 | 0.0% | :x: 
| NANDCore.o | 100.0% | 21 / 21 | 100.0% | :white_check_mark: 
| NANDSecret.o | 0.0% | 0 / 1 | 0.0% | :x: 
| NANDCheck.o | 0.0% | 0 / 2 | 0.0% | :x: 
| NANDLogging.o | 0.0% | 0 / 3 | 0.0% | :x: 


# nand.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| nandCreate | :x: |
| NANDCreate | :x: |
| NANDPrivateCreate | :x: |
| NANDPrivateCreateAsync | :x: |
| NANDDelete | :x: |
| NANDPrivateDelete | :x: |
| NANDPrivateDeleteAsync | :x: |
| NANDRead | :x: |
| NANDReadAsync | :x: |
| NANDWrite | :x: |
| NANDWriteAsync | :x: |
| NANDSeek | :x: |
| NANDSeekAsync | :x: |
| nandCreateDir | :x: |
| NANDPrivateCreateDir | :x: |
| NANDPrivateCreateDirAsync | :x: |
| nandMove | :x: |
| NANDMove | :x: |
| NANDGetLength | :x: |
| nandGetFileStatusAsyncCallback | :x: |
| NANDGetLengthAsync | :x: |
| nandComposePerm | :x: |
| nandSplitPerm | :x: |
| nandGetStatus | :x: |
| nandGetStatusCallback | :x: |
| NANDGetStatus | :x: |
| NANDPrivateGetStatus | :x: |
| NANDPrivateGetStatusAsync | :x: |
| NANDSetUserData | :x: |
| NANDGetUserData | :x: |


# NANDOpenClose.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| nandOpen | :x: |
| NANDOpen | :x: |
| NANDPrivateOpen | :x: |
| NANDOpenAsync | :x: |
| NANDPrivateOpenAsync | :x: |
| nandOpenCallback | :x: |
| NANDClose | :x: |
| NANDCloseAsync | :x: |
| NANDPrivateSafeOpenAsync | :x: |
| nandSafeOpenAsync | :x: |
| nandSafeOpenCallback | :x: |
| nandReadOpenCallback | :x: |
| NANDSafeCloseAsync | :x: |
| nandSafeCloseAsync | :x: |
| nandSafeCloseCallback | :x: |
| nandReadCloseCallback | :x: |
| nandCloseCallback | :x: |


# NANDCore.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| nandRemoveTailToken | :white_check_mark: |
| nandGetHeadToken | :white_check_mark: |
| nandGetRelativeName | :white_check_mark: |
| nandConvertPath | :white_check_mark: |
| nandIsPrivatePath | :white_check_mark: |
| nandIsUnderPrivatePath | :white_check_mark: |
| nandIsInitialized | :white_check_mark: |
| nandReportErrorCode | :white_check_mark: |
| nandConvertErrorCode | :white_check_mark: |
| nandGenerateAbsPath | :white_check_mark: |
| nandGetParentDirectory | :white_check_mark: |
| NANDInit | :white_check_mark: |
| nandOnShutdown | :white_check_mark: |
| nandShutdownCallback | :white_check_mark: |
| NANDGetHomeDir | :white_check_mark: |
| nandCallback | :white_check_mark: |
| nandGetType | :white_check_mark: |
| NANDPrivateGetTypeAsync | :white_check_mark: |
| nandGetTypeCallback | :white_check_mark: |
| nandGetHomeDir | :white_check_mark: |
| NANDInitBanner | :white_check_mark: |


# NANDSecret.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| NANDSecretGetUsage | :x: |


# NANDCheck.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| nandCalcUsage | :x: |
| NANDCheck | :x: |


# NANDLogging.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| reserveFileDescriptor | :x: |
| NANDLoggingAddMessageAsync | :x: |
| asyncRoutine | :x: |


