# dvd
| Symbol | Meaning 
| ------------- | ------------- 
| :x: | Object has not yet been started. 
| :eight_pointed_black_star: | Object is in progress. 
| :white_check_mark: | Object is completed. 


| Object | Percentage (of Bytes) | Functions Done / Total Functions | Percentage (Functions) | Status 
| ------------- | ------------- | ------------- | ------------- | ------------- 
| dvdfs.o | 0.0% | 0 / 13 | 0.0% | :x: 
| dvd.o | 0.22303325223033255% | 3 / 54 | 5.555555555555555% | :eight_pointed_black_star: 
| dvdqueue.o | 100.0% | 6 / 6 | 100.0% | :white_check_mark: 
| dvderror.o | 0.0% | 0 / 13 | 0.0% | :x: 
| dvdidutils.o | 0.0% | 0 / 1 | 0.0% | :x: 
| dvdFatal.o | 0.0% | 0 / 4 | 0.0% | :x: 
| dvdDeviceError.o | 100.0% | 2 / 2 | 100.0% | :white_check_mark: 
| dvd_broadway.o | 100.0% | 30 / 30 | 100.0% | :white_check_mark: 


# dvdfs.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __DVDFSInit | :x: |
| DVDConvertPathToEntrynum | :x: |
| DVDFastOpen | :x: |
| DVDOpen | :x: |
| DVDClose | :x: |
| entryToPath | :x: |
| DVDReadAsyncPrio | :x: |
| cbForReadAsync | :x: |
| DVDReadPrio | :x: |
| cbForReadSync | :x: |
| DVDOpenDir | :x: |
| DVDReadDir | :x: |
| DVDCloseDir | :x: |


# dvd.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| StampCommand | :x: |
| defaultOptionalCommandChecker | :x: |
| DVDInit | :x: |
| stateReadingFST | :x: |
| cbForStateReadingFST | :x: |
| FatalAlarmHandler | :x: |
| cbForStateError | :x: |
| cbForStoreErrorCode1 | :x: |
| cbForStoreErrorCode2 | :x: |
| CategorizeError | :x: |
| cbForStoreErrorCode3 | :x: |
| cbForStateGettingError | :x: |
| cbForUnrecoveredError | :x: |
| cbForUnrecoveredErrorRetry | :x: |
| cbForStateGoToRetry | :x: |
| stateCheckID | :x: |
| cbForStateReadingTOC | :x: |
| cbForStateReadingPartitionInfo | :x: |
| cbForStateOpenPartition | :x: |
| cbForStateOpenPartition2 | :x: |
| cbForStateCheckID1 | :x: |
| cbForStateCheckID2 | :x: |
| stateCoverClosed | :x: |
| ResetAlarmHandler | :x: |
| cbForStateReset | :x: |
| stateDownRotation | :x: |
| cbForStateDownRotation | :x: |
| stateCoverClosed_CMD | :x: |
| cbForStateCoverClosed | :x: |
| cbForPrepareCoverRegister | :x: |
| CoverAlarmHandler | :x: |
| stateReady | :x: |
| stateBusy | :x: |
| cbForStateBusy | :x: |
| DVDReadAbsAsyncPrio | :x: |
| DVDInquiryAsync | :x: |
| DVDGetCommandBlockStatus | :x: |
| DVDGetDriveStatus | :x: |
| DVDSetAutoInvalidation | :x: |
| DVDResume | :x: |
| DVDCancelAsync | :x: |
| DVDCancel | :x: |
| cbForCancelSync | :x: |
| DVDGetCurrentDiskID | :x: |
| __BS2DVDLowCallback | :x: |
| __DVDGetCoverStatus | :x: |
| __DVDResetWithNoSpinup | :x: |
| DVDCheckDiskAsync | :x: |
| __DVDPrepareResetAsync | :x: |
| Callback | :x: |
| __DVDPrepareReset | :x: |
| __DVDTestAlarm | :white_check_mark: |
| __DVDStopMotorAsync | :white_check_mark: |
| __DVDRestartMotor | :white_check_mark: |


# dvdqueue.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __DVDClearWaitingQueue | :white_check_mark: |
| __DVDPushWaitingQueue | :white_check_mark: |
| __DVDPopWaitingQueue | :white_check_mark: |
| __DVDCheckWaitingQueue | :white_check_mark: |
| __DVDGetNextWaitingQueue | :white_check_mark: |
| __DVDDequeueWaitingQueue | :white_check_mark: |


# dvderror.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| cbForNandClose | :x: |
| cbForNandWrite | :x: |
| cbForNandSeek | :x: |
| cbForNandWrite0 | :x: |
| cbForNandSeek2 | :x: |
| cbForNandRead | :x: |
| cbForNandSeek0 | :x: |
| cbForNandSeek1 | :x: |
| cbForNandOpen | :x: |
| cbForNandCreate | :x: |
| cbForNandCreateDir | :x: |
| cbForPrepareStatusRegister | :x: |
| __DVDStoreErrorCode | :x: |


# dvdidutils.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| DVDCompareDiskID | :x: |


# dvdFatal.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __DVDShowFatalMessage | :x: |
| DVDSetAutoFatalMessaging | :x: |
| __DVDGetAutoFatalMessaging | :x: |
| __DVDPrintFatalMessage | :x: |


# dvdDeviceError.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| lowCallback | :white_check_mark: |
| __DVDCheckDevice | :white_check_mark: |


# dvd_broadway.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| doTransactionCallback | :white_check_mark: |
| doPrepareCoverRegisterCallback | :white_check_mark: |
| DVDLowFinalize | :white_check_mark: |
| DVDLowInit | :white_check_mark: |
| DVDLowReadDiskID | :white_check_mark: |
| DVDLowOpenPartition | :white_check_mark: |
| DVDLowOpenPartitionWithTmdAndTicketView | :white_check_mark: |
| DVDLowGetNoDiscBufferSizes | :white_check_mark: |
| DVDLowGetNoDiscOpenPartitionParams | :white_check_mark: |
| DVDLowClosePartition | :white_check_mark: |
| DVDLowUnencryptedRead | :white_check_mark: |
| DVDLowStopMotor | :white_check_mark: |
| DVDLowInquiry | :white_check_mark: |
| DVDLowRequestError | :white_check_mark: |
| DVDLowSetSpinupFlag | :white_check_mark: |
| DVDLowReset | :white_check_mark: |
| DVDLowAudioBufferConfig | :white_check_mark: |
| DVDLowReportKey | :white_check_mark: |
| DVDLowSetMaximumRotation | :white_check_mark: |
| DVDLowRead | :white_check_mark: |
| DVDLowSeek | :white_check_mark: |
| DVDLowGetCoverRegister | :white_check_mark: |
| DVDLowGetStatusRegister | :white_check_mark: |
| DVDLowPrepareCoverRegister | :white_check_mark: |
| DVDLowPrepareStatusRegister | :white_check_mark: |
| DVDLowGetImmBufferReg | :white_check_mark: |
| DVDLowUnmaskStatusInterrupts | :white_check_mark: |
| DVDLowMaskCoverInterrupt | :white_check_mark: |
| DVDLowClearCoverInterrupt | :white_check_mark: |
| __DVDLowTestAlarm | :white_check_mark: |


