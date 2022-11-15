# os
| Symbol | Meaning 
| ------------- | ------------- 
| :x: | Object has not yet been started. 
| :eight_pointed_black_star: | Object is in progress. 
| :white_check_mark: | Object is completed. 


| Object | Percentage (of Bytes) | Functions Done / Total Functions | Percentage (Functions) | Status 
| ------------- | ------------- | ------------- | ------------- | ------------- 
| OS.o | 0.7009345794392523% | 2 / 20 | 10.0% | :eight_pointed_black_star: 
| OSAlarm.o | 0.0% | 0 / 12 | 0.0% | :x: 
| OSAlloc.o | 0.0% | 0 / 6 | 0.0% | :x: 
| OSArena.o | 0.0% | 0 / 13 | 0.0% | :x: 
| OSAudioSystem.o | 0.0% | 0 / 3 | 0.0% | :x: 
| OSCache.o | 100.0% | 18 / 18 | 100.0% | :white_check_mark: 
| OSContext.o | 12.238325281803544% | 4 / 16 | 25.0% | :eight_pointed_black_star: 
| OSError.o | 24.07809110629067% | 3 / 5 | 60.0% | :eight_pointed_black_star: 
| OSExec.o | 0.0% | 0 / 11 | 0.0% | :x: 
| OSFatal.o | 0.0% | 0 / 4 | 0.0% | :x: 
| OSFont.o | 0.0% | 0 / 8 | 0.0% | :x: 
| OSInterrupt.o | 33.471933471933475% | 9 / 11 | 81.81818181818183% | :eight_pointed_black_star: 
| OSLink.o | 100.0% | 1 / 1 | 100.0% | :white_check_mark: 
| OSMessage.o | 0.0% | 0 / 4 | 0.0% | :x: 
| OSMemory.o | 0.0% | 0 / 18 | 0.0% | :x: 
| OSMutex.o | 0.0% | 0 / 5 | 0.0% | :x: 
| OSReboot.o | 0.0% | 0 / 2 | 0.0% | :x: 
| OSReset.o | 0.0% | 0 / 14 | 0.0% | :x: 
| OSRtc.o | 0.0% | 0 / 9 | 0.0% | :x: 
| OSSync.o | 0.0% | 0 / 2 | 0.0% | :x: 
| OSThread.o | 2.0053475935828877% | 2 / 28 | 7.142857142857142% | :eight_pointed_black_star: 
| OSTime.o | 0.0% | 0 / 6 | 0.0% | :x: 
| OSUtf.o | 0.0% | 0 / 4 | 0.0% | :x: 
| OSIpc.o | 100.0% | 3 / 3 | 100.0% | :white_check_mark: 
| OSStateTM.o | 0.0% | 0 / 12 | 0.0% | :x: 
| OSPlayRecord.o | 0.0% | 0 / 4 | 0.0% | :x: 
| OSStateFlags.o | 0.0% | 0 / 2 | 0.0% | :x: 
| OSNet.o | 100.0% | 1 / 1 | 100.0% | :white_check_mark: 
| OSNandbootInfo.o | 0.0% | 0 / 2 | 0.0% | :x: 
| OSPlayTime.o | 0.0% | 0 / 8 | 0.0% | :x: 
| OSLaunch.o | 0.0% | 0 / 8 | 0.0% | :x: 
| __ppc_eabi_init.o | 0.0% | 0 / 3 | 0.0% | :x: 


# OS.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __OSFPRInit | :x: |
| __OSGetIOSRev | :x: |
| OSGetConsoleType | :x: |
| ClearArena | :x: |
| ClearMEM2Arena | :x: |
| InquiryCallback | :x: |
| ReportOSInfo | :x: |
| OSInit | :x: |
| OSExceptionInit | :x: |
| __OSDBIntegrator | :x: |
| __OSDBJump | :x: |
| __OSSetExceptionHandler | :white_check_mark: |
| __OSGetExceptionHandler | :white_check_mark: |
| OSExceptionVector | :x: |
| OSDefaultExceptionHandler | :x: |
| __OSPSInit | :x: |
| __OSGetDIConfig | :x: |
| OSRegisterVersion | :x: |
| OSGetAppGamename | :x: |
| OSGetAppType | :x: |


# OSAlarm.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __OSInitAlarm | :x: |
| OSCreateAlarm | :x: |
| InsertAlarm | :x: |
| OSSetAlarm | :x: |
| OSSetPeriodicAlarm | :x: |
| OSCancelAlarm | :x: |
| DecrementerExceptionCallback | :x: |
| DecrementerExceptionHandler | :x: |
| OSSetAlarmTag | :x: |
| OnReset | :x: |
| OSSetAlarmUserData | :x: |
| OSGetAlarmUserData | :x: |


# OSAlloc.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| DLInsert | :x: |
| OSAllocFromHeap | :x: |
| OSFreeToHeap | :x: |
| OSSetCurrentHeap | :x: |
| OSInitAlloc | :x: |
| OSCreateHeap | :x: |


# OSArena.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| OSGetMEM1ArenaHi | :x: |
| OSGetMEM2ArenaHi | :x: |
| OSGetArenaHi | :x: |
| OSGetMEM1ArenaLo | :x: |
| OSGetMEM2ArenaLo | :x: |
| OSGetArenaLo | :x: |
| OSSetMEM1ArenaHi | :x: |
| OSSetMEM2ArenaHi | :x: |
| OSSetArenaHi | :x: |
| OSSetMEM1ArenaLo | :x: |
| OSSetMEM2ArenaLo | :x: |
| OSSetArenaLo | :x: |
| OSAllocFromMEM1ArenaLo | :x: |


# OSAudioSystem.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __AIClockInit | :x: |
| __OSInitAudioSystem | :x: |
| __OSStopAudioSystem | :x: |


# OSCache.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| DCEnable | :white_check_mark: |
| DCInvalidateRange | :white_check_mark: |
| DCFlushRange | :white_check_mark: |
| DCStoreRange | :white_check_mark: |
| DCFlushRangeNoSync | :white_check_mark: |
| DCStoreRangeNoSync | :white_check_mark: |
| DCZeroRange | :white_check_mark: |
| ICInvalidateRange | :white_check_mark: |
| ICFlashInvalidate | :white_check_mark: |
| ICEnable | :white_check_mark: |
| __LCEnable | :white_check_mark: |
| LCEnable | :white_check_mark: |
| LCDisable | :white_check_mark: |
| LCStoreBlocks | :white_check_mark: |
| LCStoreData | :white_check_mark: |
| LCQueueWait | :white_check_mark: |
| DMAErrorHandler | :white_check_mark: |
| __OSCacheInit | :white_check_mark: |


# OSContext.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __OSLoadFPUContext | :x: |
| __OSSaveFPUContext | :x: |
| OSSaveFPUContext | :white_check_mark: |
| OSSetCurrentContext | :white_check_mark: |
| OSGetCurrentContext | :x: |
| OSSaveContext | :x: |
| OSLoadContext | :x: |
| OSGetStackPointer | :x: |
| OSSwitchFiber | :x: |
| OSSwitchFiberEx | :x: |
| OSClearContext | :x: |
| OSInitContext | :x: |
| OSDumpContext | :x: |
| OSSwitchFPUContext | :white_check_mark: |
| __OSContextInit | :white_check_mark: |
| OSFillFPUContext | :x: |


# OSError.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| OSReport | :white_check_mark: |
| OSVReport | :white_check_mark: |
| OSPanic | :white_check_mark: |
| OSSetErrorHandler | :x: |
| __OSUnhandledException | :x: |


# OSExec.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| PackArgs | :x: |
| Utf16ToArg | :x: |
| PackInstallerArgs | :x: |
| Run | :x: |
| Callback | :x: |
| __OSGetExecParams | :x: |
| callback | :x: |
| __OSLaunchNextFirmware | :x: |
| __OSLaunchMenu | :x: |
| __OSBootDolSimple | :x: |
| __OSBootDol | :x: |


# OSFatal.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| ScreenReport | :x: |
| ConfigureVideo | :x: |
| OSFatal | :x: |
| Halt | :x: |


# OSFont.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| GetFontCode | :x: |
| Decode | :x: |
| OSSetFontEncode | :x: |
| ReadFont | :x: |
| OSLoadFont | :x: |
| ParseStringS | :x: |
| ParseStringW | :x: |
| OSGetFontTexel | :x: |


# OSInterrupt.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| OSDisableInterrupts | :white_check_mark: |
| OSEnableInterrupts | :white_check_mark: |
| OSRestoreInterrupts | :white_check_mark: |
| __OSSetInterruptHandler | :white_check_mark: |
| __OSGetInterruptHandler | :white_check_mark: |
| __OSInterruptInit | :white_check_mark: |
| SetInterruptMask | :x: |
| __OSMaskInterrupts | :white_check_mark: |
| __OSUnmaskInterrupts | :white_check_mark: |
| __OSDispatchInterrupt | :x: |
| ExternalInterruptHandler | :white_check_mark: |


# OSLink.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __OSModuleInit | :white_check_mark: |


# OSMessage.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| OSInitMessageQueue | :x: |
| OSSendMessage | :x: |
| OSReceiveMessage | :x: |
| OSJamMessage | :x: |


# OSMemory.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| OSGetPhysicalMem1Size | :x: |
| OSGetPhysicalMem2Size | :x: |
| OSGetConsoleSimulatedMem1Size | :x: |
| OSGetConsoleSimulatedMem2Size | :x: |
| OnShutdown | :x: |
| MEMIntrruptHandler | :x: |
| OSProtectRange | :x: |
| ConfigMEM1_24MB | :x: |
| ConfigMEM1_48MB | :x: |
| ConfigMEM2_52MB | :x: |
| ConfigMEM2_56MB | :x: |
| ConfigMEM2_64MB | :x: |
| ConfigMEM2_112MB | :x: |
| ConfigMEM2_128MB | :x: |
| ConfigMEM_ES1_0 | :x: |
| RealMode | :x: |
| BATConfig | :x: |
| __OSInitMemoryProtection | :x: |


# OSMutex.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| OSInitMutex | :x: |
| OSLockMutex | :x: |
| OSUnlockMutex | :x: |
| __OSUnlockAllMutex | :x: |
| OSTryLockMutex | :x: |


# OSReboot.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __OSReboot | :x: |
| OSGetSaveRegion | :x: |


# OSReset.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| OSRegisterShutdownFunction | :x: |
| __OSCallShutdownFunctions | :x: |
| __OSShutdownDevices | :x: |
| __OSGetDiscState | :x: |
| OSRebootSystem | :x: |
| OSShutdownSystem | :x: |
| OSRestart | :x: |
| __OSReturnToMenu | :x: |
| OSReturnToMenu | :x: |
| OSReturnToSetting | :x: |
| __OSReturnToMenuForError | :x: |
| __OSHotResetForError | :x: |
| OSGetResetCode | :x: |
| OSResetSystem | :x: |


# OSRtc.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| WriteSramCallback | :x: |
| __OSInitSram | :x: |
| UnlockSram | :x: |
| __OSSyncSram | :x: |
| __OSReadROM | :x: |
| OSGetWirelessID | :x: |
| OSSetWirelessID | :x: |
| __OSGetRTCFlags | :x: |
| __OSClearRTCFlags | :x: |


# OSSync.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| SystemCallVector | :x: |
| __OSInitSystemCall | :x: |


# OSThread.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| DefaultSwitchThreadCallback | :x: |
| __OSThreadInit | :x: |
| OSInitThreadQueue | :x: |
| OSGetCurrentThread | :x: |
| OSIsThreadSuspended | :x: |
| OSIsThreadTerminated | :x: |
| OSDisableScheduler | :white_check_mark: |
| OSEnableScheduler | :white_check_mark: |
| UnsetRun | :x: |
| __OSGetEffectivePriority | :x: |
| SetEffectivePriority | :x: |
| __OSPromoteThread | :x: |
| SelectThread | :x: |
| __OSReschedule | :x: |
| OSYieldThread | :x: |
| OSCreateThread | :x: |
| OSExitThread | :x: |
| OSCancelThread | :x: |
| OSJoinThread | :x: |
| OSDetachThread | :x: |
| OSResumeThread | :x: |
| OSSuspendThread | :x: |
| OSSleepThread | :x: |
| OSWakeupThread | :x: |
| OSSetThreadPriority | :x: |
| OSGetThreadPriority | :x: |
| SleepAlarmHandler | :x: |
| OSSleepTicks | :x: |


# OSTime.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| OSGetTime | :x: |
| OSGetTick | :x: |
| __OSGetSystemTime | :x: |
| __OSTimeToSystemTime | :x: |
| GetDates | :x: |
| OSTicksToCalendarTime | :x: |


# OSUtf.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| OSUTF8to32 | :x: |
| OSUTF16to32 | :x: |
| OSUTF32toANSI | :x: |
| OSUTF32toSJIS | :x: |


# OSIpc.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __OSGetIPCBufferHi | :white_check_mark: |
| __OSGetIPCBufferLo | :white_check_mark: |
| __OSInitIPCBuffer | :white_check_mark: |


# OSStateTM.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| OSSetPowerCallback | :x: |
| OSGetResetButtonState | :x: |
| __OSInitSTM | :x: |
| __OSShutdownToSBY | :x: |
| __OSHotReset | :x: |
| __OSSetVIForceDimming | :x: |
| __OSSetIdleLEDMode | :x: |
| __OSUnRegisterStateEvent | :x: |
| __OSVIDimReplyHandler | :x: |
| __OSDefaultResetCallback | :x: |
| __OSDefaultPowerCallback | :x: |
| __OSStateEventHandler | :x: |


# OSPlayRecord.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| PlayRecordAlarmCallback | :x: |
| PlayRecordCallback | :x: |
| __OSStartPlayRecord | :x: |
| __OSStopPlayRecord | :x: |


# OSStateFlags.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __OSWriteStateFlags | :x: |
| __OSReadStateFlags | :x: |


# OSNet.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __OSInitNet | :white_check_mark: |


# OSNandbootInfo.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __OSCreateNandbootInfo | :x: |
| __OSWriteNandbootInfo | :x: |


# OSPlayTime.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| OSPlayTimeIsLimited | :x: |
| __OSPlayTimeRebootCallback | :x: |
| __OSPlayTimeFadeLastAIDCallback | :x: |
| __OSWriteExpiredFlag | :x: |
| __OSPlayTimeRebootThread | :x: |
| __OSPlayTimeAlarmExpired | :x: |
| __OSGetPlayTime | :x: |
| __OSInitPlayTime | :x: |


# OSLaunch.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| PackArgs | :x: |
| __OSCheckCompanyCode | :x: |
| __OSCheckTmdSysVersion | :x: |
| __OSGetValidTicketIndex | :x: |
| __OSRelaunchTitle | :x: |
| __OSLaunchTitle | :x: |
| LaunchCommon | :x: |
| __OSReturnToMenul | :x: |


# __ppc_eabi_init.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __init_user | :x: |
| __init_cpp | :x: |
| exit | :x: |


