# nand
| Symbol | Meaning 
| ------------- | ------------- 
| :x: | Object has not yet been started. 
| :eight_pointed_black_star: | Object is in progress. 
| :white_check_mark: | Object is completed. 


| Object | Percentage (of Bytes) | Functions Done / Total Functions | Percentage (Functions) | Status 
| ------------- | ------------- | ------------- | ------------- | ------------- 
| nand.o | 100.0% | 30 / 30 | 100.0% | :white_check_mark: 
| NANDOpenClose.o | 100.0% | 17 / 17 | 100.0% | :white_check_mark: 
| NANDCore.o | 100.0% | 21 / 21 | 100.0% | :white_check_mark: 
| NANDSecret.o | 100.0% | 1 / 1 | 100.0% | :white_check_mark: 
| NANDCheck.o | 100.0% | 2 / 2 | 100.0% | :white_check_mark: 
| NANDLogging.o | 100.0% | 3 / 3 | 100.0% | :white_check_mark: 


# nand.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| nandCreate | :white_check_mark: |
| NANDCreate | :white_check_mark: |
| NANDPrivateCreate | :white_check_mark: |
| NANDPrivateCreateAsync | :white_check_mark: |
| NANDDelete | :white_check_mark: |
| NANDPrivateDelete | :white_check_mark: |
| NANDPrivateDeleteAsync | :white_check_mark: |
| NANDRead | :white_check_mark: |
| NANDReadAsync | :white_check_mark: |
| NANDWrite | :white_check_mark: |
| NANDWriteAsync | :white_check_mark: |
| NANDSeek | :white_check_mark: |
| NANDSeekAsync | :white_check_mark: |
| nandCreateDir | :white_check_mark: |
| NANDPrivateCreateDir | :white_check_mark: |
| NANDPrivateCreateDirAsync | :white_check_mark: |
| nandMove | :white_check_mark: |
| NANDMove | :white_check_mark: |
| NANDGetLength | :white_check_mark: |
| nandGetFileStatusAsyncCallback | :white_check_mark: |
| NANDGetLengthAsync | :white_check_mark: |
| nandComposePerm | :white_check_mark: |
| nandSplitPerm | :white_check_mark: |
| nandGetStatus | :white_check_mark: |
| nandGetStatusCallback | :white_check_mark: |
| NANDGetStatus | :white_check_mark: |
| NANDPrivateGetStatus | :white_check_mark: |
| NANDPrivateGetStatusAsync | :white_check_mark: |
| NANDSetUserData | :white_check_mark: |
| NANDGetUserData | :white_check_mark: |


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


