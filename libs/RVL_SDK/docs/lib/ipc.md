# ipc
| Symbol | Meaning 
| ------------- | ------------- 
| :x: | Object has not yet been started. 
| :eight_pointed_black_star: | Object is in progress. 
| :white_check_mark: | Object is completed. 


| Object | Percentage (of Bytes) | Functions Done / Total Functions | Percentage (Functions) | Status 
| ------------- | ------------- | ------------- | ------------- | ------------- 
| ipcMain.o | 0.0% | 0 / 7 | 0.0% | :x: 
| ipcclt.o | 50.73710073710074% | 12 / 22 | 54.54545454545454% | :eight_pointed_black_star: 
| memory.o | 0.0% | 0 / 4 | 0.0% | :x: 
| ipcProfile.o | 0.0% | 0 / 4 | 0.0% | :x: 


# ipcMain.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| IPCInit | :x: |
| IPCReInit | :x: |
| IPCReadReg | :x: |
| IPCWriteReg | :x: |
| IPCGetBufferHi | :x: |
| IPCGetBufferLo | :x: |
| IPCSetBufferLo | :x: |


# ipcclt.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| strnlen | :x: |
| IpcReplyHandler | :x: |
| IPCInterruptHandler | :x: |
| IPCCltInit | :x: |
| IPCCltReInit | :x: |
| __ios_Ipc2 | :white_check_mark: |
| IOS_OpenAsync | :white_check_mark: |
| IOS_Open | :white_check_mark: |
| IOS_CloseAsync | :white_check_mark: |
| IOS_Close | :white_check_mark: |
| IOS_ReadAsync | :white_check_mark: |
| IOS_Read | :white_check_mark: |
| IOS_WriteAsync | :white_check_mark: |
| IOS_Write | :white_check_mark: |
| IOS_SeekAsync | :white_check_mark: |
| IOS_Seek | :white_check_mark: |
| IOS_IoctlAsync | :x: |
| IOS_Ioctl | :white_check_mark: |
| __ios_Ioctlv | :x: |
| IOS_IoctlvAsync | :x: |
| IOS_Ioctlv | :x: |
| IOS_IoctlvReboot | :x: |


# memory.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| iosCreateHeap | :x: |
| __iosAlloc | :x: |
| iosAllocAligned | :x: |
| iosFree | :x: |


# ipcProfile.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| IPCiProfInit | :x: |
| IPCiProfQueueReq | :x: |
| IPCiProfAck | :x: |
| IPCiProfReply | :x: |


