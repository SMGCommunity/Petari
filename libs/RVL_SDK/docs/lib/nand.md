# nand
| Symbol | Meaning 
| ------------- | ------------- 
| :x: | Object has not yet been started. 
| :eight_pointed_black_star: | Object is in progress. 
| :white_check_mark: | Object is completed. 


| Object | Percentage (of Bytes) | Functions Done / Total Functions | Percentage (Functions) | Status 
| ------------- | ------------- | ------------- | ------------- | ------------- 
| nand.o | 0.0% | 0 / 30 | 0.0% | :x: 
| NANDOpenClose.o | 100.0% | 17 / 17 | 100.0% | :white_check_mark: 
| NANDCore.o | 100.0% | 21 / 21 | 100.0% | :white_check_mark: 
| NANDSecret.o | 100.0% | 1 / 1 | 100.0% | :white_check_mark: 
| NANDCheck.o | 100.0% | 2 / 2 | 100.0% | :white_check_mark: 
| NANDLogging.o | 100.0% | 3 / 3 | 100.0% | :white_check_mark: 


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
| nandOpen | :white_check_mark: |
| NANDOpen | :white_check_mark: |
| NANDPrivateOpen | :white_check_mark: |
| NANDOpenAsync | :white_check_mark: |
| NANDPrivateOpenAsync | :white_check_mark: |
| nandOpenCallback | :white_check_mark: |
| NANDClose | :white_check_mark: |
| NANDCloseAsync | :white_check_mark: |
| NANDPrivateSafeOpenAsync | :white_check_mark: |
| nandSafeOpenAsync | :white_check_mark: |
| nandSafeOpenCallback | :white_check_mark: |
| nandReadOpenCallback | :white_check_mark: |
| NANDSafeCloseAsync | :white_check_mark: |
| nandSafeCloseAsync | :white_check_mark: |
| nandSafeCloseCallback | :white_check_mark: |
| nandReadCloseCallback | :white_check_mark: |
| nandCloseCallback | :white_check_mark: |


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
| NANDSecretGetUsage | :white_check_mark: |


# NANDCheck.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| nandCalcUsage | :white_check_mark: |
| NANDCheck | :white_check_mark: |


# NANDLogging.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| reserveFileDescriptor | :white_check_mark: |
| NANDLoggingAddMessageAsync | :white_check_mark: |
| asyncRoutine | :white_check_mark: |


