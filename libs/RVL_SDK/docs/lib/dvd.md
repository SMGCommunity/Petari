# dvd
| Symbol | Meaning 
| ------------- | ------------- 
| :x: | Object has not yet been started. 
| :eight_pointed_black_star: | Object is in progress. 
| :white_check_mark: | Object is completed. 


| Object | Percentage (of Bytes) | Functions Done / Total Functions | Percentage (Functions) | Status 
| ------------- | ------------- | ------------- | ------------- | ------------- 
| dvdfs.o | 0.0% | 0 / 13 | 0.0% | :x: 
| dvd.o | 0.0% | 0 / 54 | 0.0% | :x: 
| dvdqueue.o | 0.0% | 0 / 6 | 0.0% | :x: 
| dvderror.o | 0.0% | 0 / 13 | 0.0% | :x: 
| dvdidutils.o | 0.0% | 0 / 1 | 0.0% | :x: 
| dvdFatal.o | 0.0% | 0 / 4 | 0.0% | :x: 
| dvdDeviceError.o | 0.0% | 0 / 2 | 0.0% | :x: 
| dvd_broadway.o | 0.0% | 0 / 30 | 0.0% | :x: 


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
| __DVDTestAlarm | :x: |
| __DVDStopMotorAsync | :x: |
| __DVDRestartMotor | :x: |


# dvdqueue.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __DVDClearWaitingQueue | :x: |
| __DVDPushWaitingQueue | :x: |
| __DVDPopWaitingQueue | :x: |
| __DVDCheckWaitingQueue | :x: |
| __DVDGetNextWaitingQueue | :x: |
| __DVDDequeueWaitingQueue | :x: |


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
| lowCallback | :x: |
| __DVDCheckDevice | :x: |


# dvd_broadway.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| doTransactionCallback | :x: |
| doPrepareCoverRegisterCallback | :x: |
| DVDLowFinalize | :x: |
| DVDLowInit | :x: |
| DVDLowReadDiskID | :x: |
| DVDLowOpenPartition | :x: |
| DVDLowOpenPartitionWithTmdAndTicketView | :x: |
| DVDLowGetNoDiscBufferSizes | :x: |
| DVDLowGetNoDiscOpenPartitionParams | :x: |
| DVDLowClosePartition | :x: |
| DVDLowUnencryptedRead | :x: |
| DVDLowStopMotor | :x: |
| DVDLowInquiry | :x: |
| DVDLowRequestError | :x: |
| DVDLowSetSpinupFlag | :x: |
| DVDLowReset | :x: |
| DVDLowAudioBufferConfig | :x: |
| DVDLowReportKey | :x: |
| DVDLowSetMaximumRotation | :x: |
| DVDLowRead | :x: |
| DVDLowSeek | :x: |
| DVDLowGetCoverRegister | :x: |
| DVDLowGetStatusRegister | :x: |
| DVDLowPrepareCoverRegister | :x: |
| DVDLowPrepareStatusRegister | :x: |
| DVDLowGetImmBufferReg | :x: |
| DVDLowUnmaskStatusInterrupts | :x: |
| DVDLowMaskCoverInterrupt | :x: |
| DVDLowClearCoverInterrupt | :x: |
| __DVDLowTestAlarm | :x: |


