# dvd
| Symbol | Meaning 
| ------------- | ------------- 
| :x: | Object has not yet been started. 
| :eight_pointed_black_star: | Object is in progress. 
| :white_check_mark: | Object is completed. 


| Object | Percentage (of Bytes) | Functions Done / Total Functions | Percentage (Functions) | Status 
| ------------- | ------------- | ------------- | ------------- | ------------- 
| dvdfs.o | 100.0% | 13 / 13 | 100.0% | :white_check_mark: 
| dvd.o | 100.0% | 54 / 54 | 100.0% | :white_check_mark: 
| dvdqueue.o | 100.0% | 6 / 6 | 100.0% | :white_check_mark: 
| dvderror.o | 100.0% | 13 / 13 | 100.0% | :white_check_mark: 
| dvdidutils.o | 100.0% | 1 / 1 | 100.0% | :white_check_mark: 
| dvdFatal.o | 100.0% | 4 / 4 | 100.0% | :white_check_mark: 
| dvdDeviceError.o | 100.0% | 2 / 2 | 100.0% | :white_check_mark: 
| dvd_broadway.o | 100.0% | 30 / 30 | 100.0% | :white_check_mark: 


# dvdfs.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __DVDFSInit | :white_check_mark: |
| DVDConvertPathToEntrynum | :white_check_mark: |
| DVDFastOpen | :white_check_mark: |
| DVDOpen | :white_check_mark: |
| DVDClose | :white_check_mark: |
| entryToPath | :white_check_mark: |
| DVDReadAsyncPrio | :white_check_mark: |
| cbForReadAsync | :white_check_mark: |
| DVDReadPrio | :white_check_mark: |
| cbForReadSync | :white_check_mark: |
| DVDOpenDir | :white_check_mark: |
| DVDReadDir | :white_check_mark: |
| DVDCloseDir | :white_check_mark: |


# dvd.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| StampCommand | :white_check_mark: |
| defaultOptionalCommandChecker | :white_check_mark: |
| DVDInit | :white_check_mark: |
| stateReadingFST | :white_check_mark: |
| cbForStateReadingFST | :white_check_mark: |
| FatalAlarmHandler | :white_check_mark: |
| cbForStateError | :white_check_mark: |
| cbForStoreErrorCode1 | :white_check_mark: |
| cbForStoreErrorCode2 | :white_check_mark: |
| CategorizeError | :white_check_mark: |
| cbForStoreErrorCode3 | :white_check_mark: |
| cbForStateGettingError | :white_check_mark: |
| cbForUnrecoveredError | :white_check_mark: |
| cbForUnrecoveredErrorRetry | :white_check_mark: |
| cbForStateGoToRetry | :white_check_mark: |
| stateCheckID | :white_check_mark: |
| cbForStateReadingTOC | :white_check_mark: |
| cbForStateReadingPartitionInfo | :white_check_mark: |
| cbForStateOpenPartition | :white_check_mark: |
| cbForStateOpenPartition2 | :white_check_mark: |
| cbForStateCheckID1 | :white_check_mark: |
| cbForStateCheckID2 | :white_check_mark: |
| stateCoverClosed | :white_check_mark: |
| ResetAlarmHandler | :white_check_mark: |
| cbForStateReset | :white_check_mark: |
| stateDownRotation | :white_check_mark: |
| cbForStateDownRotation | :white_check_mark: |
| stateCoverClosed_CMD | :white_check_mark: |
| cbForStateCoverClosed | :white_check_mark: |
| cbForPrepareCoverRegister | :white_check_mark: |
| CoverAlarmHandler | :white_check_mark: |
| stateReady | :white_check_mark: |
| stateBusy | :white_check_mark: |
| cbForStateBusy | :white_check_mark: |
| DVDReadAbsAsyncPrio | :white_check_mark: |
| DVDInquiryAsync | :white_check_mark: |
| DVDGetCommandBlockStatus | :white_check_mark: |
| DVDGetDriveStatus | :white_check_mark: |
| DVDSetAutoInvalidation | :white_check_mark: |
| DVDResume | :white_check_mark: |
| DVDCancelAsync | :white_check_mark: |
| DVDCancel | :white_check_mark: |
| cbForCancelSync | :white_check_mark: |
| DVDGetCurrentDiskID | :white_check_mark: |
| __BS2DVDLowCallback | :white_check_mark: |
| __DVDGetCoverStatus | :white_check_mark: |
| __DVDResetWithNoSpinup | :white_check_mark: |
| DVDCheckDiskAsync | :white_check_mark: |
| __DVDPrepareResetAsync | :white_check_mark: |
| Callback | :white_check_mark: |
| __DVDPrepareReset | :white_check_mark: |
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
| cbForNandClose | :white_check_mark: |
| cbForNandWrite | :white_check_mark: |
| cbForNandSeek | :white_check_mark: |
| cbForNandWrite0 | :white_check_mark: |
| cbForNandSeek2 | :white_check_mark: |
| cbForNandRead | :white_check_mark: |
| cbForNandSeek0 | :white_check_mark: |
| cbForNandSeek1 | :white_check_mark: |
| cbForNandOpen | :white_check_mark: |
| cbForNandCreate | :white_check_mark: |
| cbForNandCreateDir | :white_check_mark: |
| cbForPrepareStatusRegister | :white_check_mark: |
| __DVDStoreErrorCode | :white_check_mark: |


# dvdidutils.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| DVDCompareDiskID | :white_check_mark: |


# dvdFatal.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __DVDShowFatalMessage | :white_check_mark: |
| DVDSetAutoFatalMessaging | :white_check_mark: |
| __DVDGetAutoFatalMessaging | :white_check_mark: |
| __DVDPrintFatalMessage | :white_check_mark: |


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


