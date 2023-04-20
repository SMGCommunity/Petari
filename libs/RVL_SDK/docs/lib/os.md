# os
| Symbol | Meaning 
| ------------- | ------------- 
| :x: | Object has not yet been started. 
| :eight_pointed_black_star: | Object is in progress. 
| :white_check_mark: | Object is completed. 


| Object | Percentage (of Bytes) | Functions Done / Total Functions | Percentage (Functions) | Status 
| ------------- | ------------- | ------------- | ------------- | ------------- 
| OS.o | 54.67289719626168% | 14 / 20 | 70.0% | :eight_pointed_black_star: 
| OSAlarm.o | 100.0% | 12 / 12 | 100.0% | :white_check_mark: 
| OSAlloc.o | 100.0% | 6 / 6 | 100.0% | :white_check_mark: 
| OSArena.o | 100.0% | 13 / 13 | 100.0% | :white_check_mark: 
| OSAudioSystem.o | 100.0% | 3 / 3 | 100.0% | :white_check_mark: 
| OSCache.o | 100.0% | 18 / 18 | 100.0% | :white_check_mark: 
| OSContext.o | 56.19967793880838% | 11 / 16 | 68.75% | :eight_pointed_black_star: 
| OSError.o | 59.219088937093275% | 4 / 5 | 80.0% | :eight_pointed_black_star: 
| OSExec.o | 21.309192200557103% | 4 / 11 | 36.36363636363637% | :eight_pointed_black_star: 
| OSFatal.o | 0.0% | 0 / 4 | 0.0% | :x: 
| OSFont.o | 0.0% | 0 / 8 | 0.0% | :x: 
| OSInterrupt.o | 100.0% | 11 / 11 | 100.0% | :white_check_mark: 
| OSLink.o | 100.0% | 1 / 1 | 100.0% | :white_check_mark: 
| OSMessage.o | 0.0% | 0 / 4 | 0.0% | :x: 
| OSMemory.o | 100.0% | 18 / 18 | 100.0% | :white_check_mark: 
| OSMutex.o | 0.0% | 0 / 5 | 0.0% | :x: 
| OSReboot.o | 0.0% | 0 / 2 | 0.0% | :x: 
| OSReset.o | 7.744107744107744% | 2 / 14 | 14.285714285714285% | :eight_pointed_black_star: 
| OSRtc.o | 100.0% | 9 / 9 | 100.0% | :white_check_mark: 
| OSSync.o | 100.0% | 2 / 2 | 100.0% | :white_check_mark: 
| OSThread.o | 35.360962566844925% | 14 / 28 | 50.0% | :eight_pointed_black_star: 
| OSTime.o | 100.0% | 6 / 6 | 100.0% | :white_check_mark: 
| OSUtf.o | 0.0% | 0 / 4 | 0.0% | :x: 
| OSIpc.o | 100.0% | 3 / 3 | 100.0% | :white_check_mark: 
| OSStateTM.o | 11.627906976744185% | 3 / 12 | 25.0% | :eight_pointed_black_star: 
| OSPlayRecord.o | 0.0% | 0 / 4 | 0.0% | :x: 
| OSStateFlags.o | 0.0% | 0 / 2 | 0.0% | :x: 
| OSNet.o | 100.0% | 1 / 1 | 100.0% | :white_check_mark: 
| OSNandbootInfo.o | 0.0% | 0 / 2 | 0.0% | :x: 
| OSPlayTime.o | 0.0% | 0 / 8 | 0.0% | :x: 
| OSLaunch.o | 0.0% | 0 / 8 | 0.0% | :x: 
| __ppc_eabi_init.o | 100.0% | 3 / 3 | 100.0% | :white_check_mark: 


# OS.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __OSFPRInit | :white_check_mark: |
| __OSGetIOSRev | :white_check_mark: |
| OSGetConsoleType | :white_check_mark: |
| ClearArena | :x: |
| ClearMEM2Arena | :x: |
| InquiryCallback | :x: |
| ReportOSInfo | :white_check_mark: |
| OSInit | :x: |
| OSExceptionInit | :white_check_mark: |
| __OSDBIntegrator | :white_check_mark: |
| __OSDBJump | :white_check_mark: |
| __OSSetExceptionHandler | :white_check_mark: |
| __OSGetExceptionHandler | :white_check_mark: |
| OSExceptionVector | :white_check_mark: |
| OSDefaultExceptionHandler | :white_check_mark: |
| __OSPSInit | :white_check_mark: |
| __OSGetDIConfig | :white_check_mark: |
| OSRegisterVersion | :white_check_mark: |
| OSGetAppGamename | :x: |
| OSGetAppType | :x: |


# OSAlarm.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __OSInitAlarm | :white_check_mark: |
| OSCreateAlarm | :white_check_mark: |
| InsertAlarm | :white_check_mark: |
| OSSetAlarm | :white_check_mark: |
| OSSetPeriodicAlarm | :white_check_mark: |
| OSCancelAlarm | :white_check_mark: |
| DecrementerExceptionCallback | :white_check_mark: |
| DecrementerExceptionHandler | :white_check_mark: |
| OSSetAlarmTag | :white_check_mark: |
| OnReset | :white_check_mark: |
| OSSetAlarmUserData | :white_check_mark: |
| OSGetAlarmUserData | :white_check_mark: |


# OSAlloc.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| DLInsert | :white_check_mark: |
| OSAllocFromHeap | :white_check_mark: |
| OSFreeToHeap | :white_check_mark: |
| OSSetCurrentHeap | :white_check_mark: |
| OSInitAlloc | :white_check_mark: |
| OSCreateHeap | :white_check_mark: |


# OSArena.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| OSGetMEM1ArenaHi | :white_check_mark: |
| OSGetMEM2ArenaHi | :white_check_mark: |
| OSGetArenaHi | :white_check_mark: |
| OSGetMEM1ArenaLo | :white_check_mark: |
| OSGetMEM2ArenaLo | :white_check_mark: |
| OSGetArenaLo | :white_check_mark: |
| OSSetMEM1ArenaHi | :white_check_mark: |
| OSSetMEM2ArenaHi | :white_check_mark: |
| OSSetArenaHi | :white_check_mark: |
| OSSetMEM1ArenaLo | :white_check_mark: |
| OSSetMEM2ArenaLo | :white_check_mark: |
| OSSetArenaLo | :white_check_mark: |
| OSAllocFromMEM1ArenaLo | :white_check_mark: |


# OSAudioSystem.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __AIClockInit | :white_check_mark: |
| __OSInitAudioSystem | :white_check_mark: |
| __OSStopAudioSystem | :white_check_mark: |


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
| OSSaveContext | :white_check_mark: |
| OSLoadContext | :white_check_mark: |
| OSGetStackPointer | :white_check_mark: |
| OSSwitchFiber | :white_check_mark: |
| OSSwitchFiberEx | :white_check_mark: |
| OSClearContext | :white_check_mark: |
| OSInitContext | :x: |
| OSDumpContext | :white_check_mark: |
| OSSwitchFPUContext | :white_check_mark: |
| __OSContextInit | :white_check_mark: |
| OSFillFPUContext | :x: |


# OSError.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| OSReport | :white_check_mark: |
| OSVReport | :white_check_mark: |
| OSPanic | :white_check_mark: |
| OSSetErrorHandler | :white_check_mark: |
| __OSUnhandledException | :x: |


# OSExec.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| PackArgs | :white_check_mark: |
| Utf16ToArg | :white_check_mark: |
| PackInstallerArgs | :white_check_mark: |
| Run | :x: |
| Callback | :x: |
| __OSGetExecParams | :white_check_mark: |
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
| SetInterruptMask | :white_check_mark: |
| __OSMaskInterrupts | :white_check_mark: |
| __OSUnmaskInterrupts | :white_check_mark: |
| __OSDispatchInterrupt | :white_check_mark: |
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
| OSGetPhysicalMem1Size | :white_check_mark: |
| OSGetPhysicalMem2Size | :white_check_mark: |
| OSGetConsoleSimulatedMem1Size | :white_check_mark: |
| OSGetConsoleSimulatedMem2Size | :white_check_mark: |
| OnShutdown | :white_check_mark: |
| MEMIntrruptHandler | :white_check_mark: |
| OSProtectRange | :white_check_mark: |
| ConfigMEM1_24MB | :white_check_mark: |
| ConfigMEM1_48MB | :white_check_mark: |
| ConfigMEM2_52MB | :white_check_mark: |
| ConfigMEM2_56MB | :white_check_mark: |
| ConfigMEM2_64MB | :white_check_mark: |
| ConfigMEM2_112MB | :white_check_mark: |
| ConfigMEM2_128MB | :white_check_mark: |
| ConfigMEM_ES1_0 | :white_check_mark: |
| RealMode | :white_check_mark: |
| BATConfig | :white_check_mark: |
| __OSInitMemoryProtection | :white_check_mark: |


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
| OSRegisterShutdownFunction | :white_check_mark: |
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
| OSGetResetCode | :white_check_mark: |
| OSResetSystem | :x: |


# OSRtc.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| WriteSramCallback | :white_check_mark: |
| __OSInitSram | :white_check_mark: |
| UnlockSram | :white_check_mark: |
| __OSSyncSram | :white_check_mark: |
| __OSReadROM | :white_check_mark: |
| OSGetWirelessID | :white_check_mark: |
| OSSetWirelessID | :white_check_mark: |
| __OSGetRTCFlags | :white_check_mark: |
| __OSClearRTCFlags | :white_check_mark: |


# OSSync.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| SystemCallVector | :white_check_mark: |
| __OSInitSystemCall | :white_check_mark: |


# OSThread.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| DefaultSwitchThreadCallback | :white_check_mark: |
| __OSThreadInit | :white_check_mark: |
| OSInitThreadQueue | :white_check_mark: |
| OSGetCurrentThread | :white_check_mark: |
| OSIsThreadSuspended | :white_check_mark: |
| OSIsThreadTerminated | :white_check_mark: |
| OSDisableScheduler | :white_check_mark: |
| OSEnableScheduler | :white_check_mark: |
| UnsetRun | :white_check_mark: |
| __OSGetEffectivePriority | :white_check_mark: |
| SetEffectivePriority | :white_check_mark: |
| __OSPromoteThread | :white_check_mark: |
| SelectThread | :white_check_mark: |
| __OSReschedule | :white_check_mark: |
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
| OSGetTime | :white_check_mark: |
| OSGetTick | :white_check_mark: |
| __OSGetSystemTime | :white_check_mark: |
| __OSTimeToSystemTime | :white_check_mark: |
| GetDates | :white_check_mark: |
| OSTicksToCalendarTime | :white_check_mark: |


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
| __OSSetIdleLEDMode | :white_check_mark: |
| __OSUnRegisterStateEvent | :white_check_mark: |
| __OSVIDimReplyHandler | :white_check_mark: |
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
| __init_user | :white_check_mark: |
| __init_cpp | :white_check_mark: |
| exit | :white_check_mark: |


