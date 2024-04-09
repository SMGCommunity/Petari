# TRK_Hollywood_Revolution
| Symbol | Meaning 
| ------------- | ------------- 
| :x: | Object has not yet been started. 
| :eight_pointed_black_star: | Object is in progress. 
| :white_check_mark: | Object is completed. 


| Object | Percentage (of Bytes) | Functions Done / Total Functions | Percentage (Functions) | Status 
| ------------- | ------------- | ------------- | ------------- | ------------- 
| mainloop.o | 100.0% | 1 / 1 | 100.0% | :white_check_mark: 
| nubevent.o | 100.0% | 5 / 5 | 100.0% | :white_check_mark: 
| nubinit.o | 100.0% | 3 / 3 | 100.0% | :white_check_mark: 
| msg.o | 100.0% | 1 / 1 | 100.0% | :white_check_mark: 
| msgbuf.o | 100.0% | 14 / 14 | 100.0% | :white_check_mark: 
| serpoll.o | 100.0% | 5 / 5 | 100.0% | :white_check_mark: 
| usr_put.o | 100.0% | 2 / 2 | 100.0% | :white_check_mark: 
| dispatch.o | 100.0% | 2 / 2 | 100.0% | :white_check_mark: 
| msghndlr.o | 0.0% | 0 / 16 | 0.0% | :x: 
| support.o | 0.0% | 0 / 5 | 0.0% | :x: 
| mutex_TRK.o | 100.0% | 3 / 3 | 100.0% | :white_check_mark: 
| notify.o | 100.0% | 1 / 1 | 100.0% | :white_check_mark: 
| flush_cache.o | 100.0% | 1 / 1 | 100.0% | :white_check_mark: 
| mem_TRK.o | 100.0% | 1 / 1 | 100.0% | :white_check_mark: 
| string_TRK.o | 100.0% | 1 / 1 | 100.0% | :white_check_mark: 
| targimpl.o | 89.88023952095809% | 27 / 28 | 96.42857142857143% | :eight_pointed_black_star: 
| mpc_7xx_603e.o | 0.0% | 0 / 2 | 0.0% | :x: 
| mslsupp.o | 100.0% | 2 / 2 | 100.0% | :white_check_mark: 
| dolphin_trk.o | 0.0% | 0 / 6 | 0.0% | :x: 
| main_TRK.o | 0.0% | 0 / 1 | 0.0% | :x: 
| dolphin_trk_glue.o | 28.289473684210524% | 6 / 13 | 46.15384615384615% | :eight_pointed_black_star: 
| targcont.o | 100.0% | 1 / 1 | 100.0% | :white_check_mark: 
| target_options.o | 42.857142857142854% | 1 / 2 | 50.0% | :eight_pointed_black_star: 
| UDP_Stubs.o | 100.0% | 9 / 9 | 100.0% | :white_check_mark: 
| main.o | 0.0% | 0 / 11 | 0.0% | :x: 
| CircleBuffer.o | 0.0% | 0 / 4 | 0.0% | :x: 
| MWCriticalSection_gc.o | 0.0% | 0 / 3 | 0.0% | :x: 


# mainloop.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| TRKNubMainLoop | :white_check_mark: |


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
| TRKNubWelcome | :white_check_mark: |
| TRKTerminateNub | :white_check_mark: |
| TRKInitializeNub | :white_check_mark: |


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
| TRKTestForPacket | :white_check_mark: |


# usr_put.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| usr_put_initialize | :white_check_mark: |
| usr_puts_serial | :white_check_mark: |


# dispatch.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| TRKDispatchMessage | :white_check_mark: |
| TRKInitializeDispatcher | :white_check_mark: |


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
| TRKDoNotifyStopped | :white_check_mark: |


# flush_cache.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| TRK_flush_cache | :white_check_mark: |


# mem_TRK.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| TRK_fill_mem | :white_check_mark: |


# string_TRK.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| TRK_strlen | :white_check_mark: |


# targimpl.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __TRK_get_MSR | :white_check_mark: |
| __TRK_set_MSR | :white_check_mark: |
| TRK_ppc_memcpy | :white_check_mark: |
| TRKInterruptHandler | :white_check_mark: |
| TRKExceptionHandler | :white_check_mark: |
| TRKSwapAndGo | :white_check_mark: |
| TRKInterruptHandlerEnableInterrupts | :white_check_mark: |
| ReadFPSCR | :white_check_mark: |
| WriteFPSCR | :white_check_mark: |
| TRKTargetSetInputPendingPtr | :white_check_mark: |
| TRKTargetStop | :white_check_mark: |
| TRKTargetSetStopped | :white_check_mark: |
| TRKTargetStopped | :white_check_mark: |
| TRKTargetSupportRequest | :white_check_mark: |
| TRKTargetGetPC | :white_check_mark: |
| TRKTargetStepOutOfRange | :white_check_mark: |
| TRKTargetSingleStep | :white_check_mark: |
| TRKTargetAddExceptionInfo | :white_check_mark: |
| TRKTargetAddStopInfo | :white_check_mark: |
| TRKTargetInterrupt | :white_check_mark: |
| TRKPostInterruptEvent | :white_check_mark: |
| TRKTargetAccessExtended2 | :white_check_mark: |
| TRKTargetAccessExtended1 | :white_check_mark: |
| TRKTargetAccessFP | :white_check_mark: |
| TRKTargetAccessDefault | :white_check_mark: |
| TRKTargetReadInstruction | :white_check_mark: |
| TRKTargetAccessMemory | :white_check_mark: |
| TRKValidMemory32 | :x: |


# mpc_7xx_603e.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| TRKSaveExtended1Block | :x: |
| TRKRestoreExtended1Block | :x: |


# mslsupp.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __TRK_write_console | :white_check_mark: |
| __read_console | :white_check_mark: |


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
| TRK_board_display | :white_check_mark: |
| UnreserveEXI2Port | :x: |
| ReserveEXI2Port | :x: |
| TRKWriteUARTN | :white_check_mark: |
| TRKReadUARTN | :white_check_mark: |
| TRKPollUART | :white_check_mark: |
| EnableEXI2Interrupts | :white_check_mark: |
| TRKInitializeIntDrivenUART | :x: |
| InitMetroTRKCommTable | :x: |
| TRKEXICallBack | :white_check_mark: |


# targcont.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| TRKTargetContinue | :white_check_mark: |


# target_options.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| GetUseSerialIO | :x: |
| SetUseSerialIO | :white_check_mark: |


# UDP_Stubs.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| udp_cc_post_stop | :white_check_mark: |
| udp_cc_pre_continue | :white_check_mark: |
| udp_cc_peek | :white_check_mark: |
| udp_cc_write | :white_check_mark: |
| udp_cc_read | :white_check_mark: |
| udp_cc_close | :white_check_mark: |
| udp_cc_open | :white_check_mark: |
| udp_cc_shutdown | :white_check_mark: |
| udp_cc_initialize | :white_check_mark: |


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


