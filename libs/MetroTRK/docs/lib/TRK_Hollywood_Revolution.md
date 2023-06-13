# TRK_Hollywood_Revolution
| Symbol | Meaning 
| ------------- | ------------- 
| :x: | Object has not yet been started. 
| :eight_pointed_black_star: | Object is in progress. 
| :white_check_mark: | Object is completed. 


| Object | Percentage (of Bytes) | Functions Done / Total Functions | Percentage (Functions) | Status 
| ------------- | ------------- | ------------- | ------------- | ------------- 
| mainloop.o | 0.0% | 0 / 1 | 0.0% | :x: 
| nubevent.o | 100.0% | 5 / 5 | 100.0% | :white_check_mark: 
| nubinit.o | 0.0% | 0 / 3 | 0.0% | :x: 
| msg.o | 100.0% | 1 / 1 | 100.0% | :white_check_mark: 
| msgbuf.o | 100.0% | 14 / 14 | 100.0% | :white_check_mark: 
| serpoll.o | 51.4018691588785% | 4 / 5 | 80.0% | :eight_pointed_black_star: 
| usr_put.o | 100.0% | 2 / 2 | 100.0% | :white_check_mark: 
| dispatch.o | 0.0% | 0 / 2 | 0.0% | :x: 
| msghndlr.o | 0.0% | 0 / 16 | 0.0% | :x: 
| support.o | 0.0% | 0 / 5 | 0.0% | :x: 
| mutex_TRK.o | 100.0% | 3 / 3 | 100.0% | :white_check_mark: 
| notify.o | 0.0% | 0 / 1 | 0.0% | :x: 
| flush_cache.o | 0.0% | 0 / 1 | 0.0% | :x: 
| mem_TRK.o | 0.0% | 0 / 1 | 0.0% | :x: 
| string_TRK.o | 0.0% | 0 / 1 | 0.0% | :x: 
| targimpl.o | 0.0% | 0 / 28 | 0.0% | :x: 
| mpc_7xx_603e.o | 0.0% | 0 / 2 | 0.0% | :x: 
| mslsupp.o | 0.0% | 0 / 2 | 0.0% | :x: 
| dolphin_trk.o | 0.0% | 0 / 6 | 0.0% | :x: 
| main_TRK.o | 0.0% | 0 / 1 | 0.0% | :x: 
| dolphin_trk_glue.o | 0.0% | 0 / 13 | 0.0% | :x: 
| targcont.o | 0.0% | 0 / 1 | 0.0% | :x: 
| target_options.o | 0.0% | 0 / 2 | 0.0% | :x: 
| UDP_Stubs.o | 0.0% | 0 / 9 | 0.0% | :x: 
| main.o | 0.0% | 0 / 11 | 0.0% | :x: 
| CircleBuffer.o | 0.0% | 0 / 4 | 0.0% | :x: 
| MWCriticalSection_gc.o | 0.0% | 0 / 3 | 0.0% | :x: 


# mainloop.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| TRKNubMainLoop | :x: |


# nubevent.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| TRKDestructEvent | :white_check_mark: |
| TRKConstructEvent | :white_check_mark: |
| TRKPostEvent | :white_check_mark: |
| TRKGetNextEvent | :white_check_mark: |
| TRKInitializeEventQueue | :white_check_mark: |


# nubinit.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| TRKNubWelcome | :x: |
| TRKTerminateNub | :x: |
| TRKInitializeNub | :x: |


# msg.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| TRKMessageSend | :white_check_mark: |


# msgbuf.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| TRKReadBuffer_ui32 | :white_check_mark: |
| TRKReadBuffer_ui8 | :white_check_mark: |
| TRKReadBuffer1_ui64 | :white_check_mark: |
| TRKAppendBuffer_ui32 | :white_check_mark: |
| TRKAppendBuffer_ui8 | :white_check_mark: |
| TRKAppendBuffer1_ui64 | :white_check_mark: |
| TRKReadBuffer | :white_check_mark: |
| TRKAppendBuffer | :white_check_mark: |
| TRKSetBufferPosition | :white_check_mark: |
| TRKResetBuffer | :white_check_mark: |
| TRKReleaseBuffer | :white_check_mark: |
| TRKGetBuffer | :white_check_mark: |
| TRKGetFreeBuffer | :white_check_mark: |
| TRKInitializeMessageBuffers | :white_check_mark: |


# serpoll.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| TRKTerminateSerialHandler | :white_check_mark: |
| TRKInitializeSerialHandler | :white_check_mark: |
| TRKProcessInput | :white_check_mark: |
| TRKGetInput | :white_check_mark: |
| TRKTestForPacket | :x: |


# usr_put.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| usr_put_initialize | :white_check_mark: |
| usr_puts_serial | :white_check_mark: |


# dispatch.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| TRKDispatchMessage | :x: |
| TRKInitializeDispatcher | :x: |


# msghndlr.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| TRKDoSetOption | :x: |
| TRKDoStop | :x: |
| TRKDoStep | :x: |
| TRKDoContinue | :x: |
| TRKDoWriteRegisters | :x: |
| TRKDoReadRegisters | :x: |
| TRKDoWriteMemory | :x: |
| TRKDoReadMemory | :x: |
| TRKDoSupportMask | :x: |
| TRKDoVersions | :x: |
| TRKDoOverride | :x: |
| TRKDoReset | :x: |
| TRKDoDisconnect | :x: |
| TRKDoConnect | :x: |
| SetTRKConnected | :x: |
| GetTRKConnected | :x: |


# support.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| HandlePositionFileSupportRequest | :x: |
| HandleCloseFileSupportRequest | :x: |
| HandleOpenFileSupportRequest | :x: |
| TRKRequestSend | :x: |
| TRKSuppAccessFile | :x: |


# mutex_TRK.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| TRKReleaseMutex | :white_check_mark: |
| TRKAcquireMutex | :white_check_mark: |
| TRKInitializeMutex | :white_check_mark: |


# notify.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| TRKDoNotifyStopped | :x: |


# flush_cache.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| TRK_flush_cache | :x: |


# mem_TRK.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| TRK_fill_mem | :x: |


# string_TRK.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| TRK_strlen | :x: |


# targimpl.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __TRK_get_MSR | :x: |
| __TRK_set_MSR | :x: |
| TRK_ppc_memcpy | :x: |
| TRKInterruptHandler | :x: |
| TRKExceptionHandler | :x: |
| TRKSwapAndGo | :x: |
| TRKInterruptHandlerEnableInterrupts | :x: |
| ReadFPSCR | :x: |
| WriteFPSCR | :x: |
| TRKTargetSetInputPendingPtr | :x: |
| TRKTargetStop | :x: |
| TRKTargetSetStopped | :x: |
| TRKTargetStopped | :x: |
| TRKTargetSupportRequest | :x: |
| TRKTargetGetPC | :x: |
| TRKTargetStepOutOfRange | :x: |
| TRKTargetSingleStep | :x: |
| TRKTargetAddExceptionInfo | :x: |
| TRKTargetAddStopInfo | :x: |
| TRKTargetInterrupt | :x: |
| TRKPostInterruptEvent | :x: |
| TRKTargetAccessExtended2 | :x: |
| TRKTargetAccessExtended1 | :x: |
| TRKTargetAccessFP | :x: |
| TRKTargetAccessDefault | :x: |
| TRKTargetReadInstruction | :x: |
| TRKTargetAccessMemory | :x: |
| TRKValidMemory32 | :x: |


# mpc_7xx_603e.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| TRKSaveExtended1Block | :x: |
| TRKRestoreExtended1Block | :x: |


# mslsupp.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __TRK_write_console | :x: |
| __read_console | :x: |


# dolphin_trk.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| InitMetroTRK | :x: |
| InitMetroTRK_BBA | :x: |
| TRKInitializeTarget | :x: |
| __TRK_copy_vectors | :x: |
| TRKTargetTranslate | :x: |
| EnableMetroTRKInterrupts | :x: |


# main_TRK.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| TRK_main | :x: |


# dolphin_trk_glue.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| TRKLoadContext | :x: |
| TRKUARTInterruptHandler | :x: |
| InitializeProgramEndTrap | :x: |
| TRK_board_display | :x: |
| UnreserveEXI2Port | :x: |
| ReserveEXI2Port | :x: |
| TRKWriteUARTN | :x: |
| TRKReadUARTN | :x: |
| TRKPollUART | :x: |
| EnableEXI2Interrupts | :x: |
| TRKInitializeIntDrivenUART | :x: |
| InitMetroTRKCommTable | :x: |
| TRKEXICallBack | :x: |


# targcont.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| TRKTargetContinue | :x: |


# target_options.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| GetUseSerialIO | :x: |
| SetUseSerialIO | :x: |


# UDP_Stubs.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| udp_cc_post_stop | :x: |
| udp_cc_pre_continue | :x: |
| udp_cc_peek | :x: |
| udp_cc_write | :x: |
| udp_cc_read | :x: |
| udp_cc_close | :x: |
| udp_cc_open | :x: |
| udp_cc_shutdown | :x: |
| udp_cc_initialize | :x: |


# main.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| gdev_cc_initinterrupts | :x: |
| gdev_cc_peek | :x: |
| gdev_cc_post_stop | :x: |
| gdev_cc_pre_continue | :x: |
| gdev_cc_write | :x: |
| gdev_cc_read | :x: |
| gdev_cc_close | :x: |
| gdev_cc_open | :x: |
| gdev_cc_shutdown | :x: |
| gdev_cc_initialize | :x: |
| OutputData | :x: |


# CircleBuffer.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| CircleBufferReadBytes | :x: |
| CircleBufferWriteBytes | :x: |
| CircleBufferInitialize | :x: |
| CBGetBytesAvailableForRead | :x: |


# MWCriticalSection_gc.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| MWExitCriticalSection | :x: |
| MWEnterCriticalSection | :x: |
| MWInitializeCriticalSection | :x: |


