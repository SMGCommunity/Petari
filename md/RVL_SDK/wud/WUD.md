# main/RVL_SDK/wud/WUD

**Source Path:** `src/RVL_SDK/wud/WUD.c`

## Functions

| Name | Address | Match % |
|------|---------|---------|
| `App_MEMalloc` | `0x804E5DA0` | :white_check_mark: (100.0%) |
| `App_MEMfree` | `0x804E5DF0` | :white_check_mark: (100.0%) |
| `SyncFlushCallback` | `0x804E5E40` | :white_check_mark: (100.0%) |
| `DeleteFlushCallback` | `0x804E5EB8` | :white_check_mark: (100.0%) |
| `ShutFlushCallback` | `0x804E5F10` | :x: (80.4%) |
| `InitFlushCallback` | `0x804E5F5C` | :x: (0.0%) |
| `WUDiIsSyncDisabled` | `0x804E5FA8` | :x: (96.8%) |
| `WUDiSaveDeviceToNand` | `0x804E6108` | :white_check_mark: (100.0%) |
| `WUDiSyncDone` | `0x804E6214` | :x: (96.7%) |
| `SyncHandler` | `0x804E6384` | :x: (76.7%) |
| `SyncHandler0` | `0x804E6CC4` | :white_check_mark: (100.0%) |
| `WUDiTerminateDevice` | `0x804E6CE4` | :white_check_mark: (100.0%) |
| `WUDiDeleteDevice` | `0x804E6D7C` | :white_check_mark: (100.0%) |
| `DeleteAllHandler` | `0x804E6E30` | :x: (83.3%) |
| `DeleteAllHandler0` | `0x804E6F8C` | :white_check_mark: (100.0%) |
| `WUDiClearUnregisteredDevice` | `0x804E6FAC` | :white_check_mark: (100.0%) |
| `EnableStackHandler` | `0x804E70C0` | :x: (93.1%) |
| `EnableStackHandler0` | `0x804E7294` | :white_check_mark: (100.0%) |
| `WUDiGetRegisteredDevice` | `0x804E72B4` | :x: (25.2%) |
| `InitHandler` | `0x804E7670` | :x: (96.3%) |
| `InitHandler0` | `0x804E7754` | :white_check_mark: (100.0%) |
| `ShutdownHandler` | `0x804E7774` | :x: (58.2%) |
| `ShutdownHandler0` | `0x804E7838` | :white_check_mark: (100.0%) |
| `InitCore` | `0x804E7858` | :x: (100.0%) |
| `WUDInit` | `0x804E7BE4` | :white_check_mark: (100.0%) |
| `WUDRegisterAllocator` | `0x804E7CDC` | :white_check_mark: (100.0%) |
| `WUDGetAllocatedMemSize` | `0x804E7D28` | :white_check_mark: (100.0%) |
| `WUDShutdown` | `0x804E7D2C` | :x: (56.4%) |
| `WUDGetStatus` | `0x804E7F4C` | :white_check_mark: (100.0%) |
| `WUDGetBufferStatus` | `0x804E7F88` | :white_check_mark: (100.0%) |
| `WUDSetSniffMode` | `0x804E7FD0` | :white_check_mark: (100.0%) |
| `WUDSetSyncSimpleCallback` | `0x804E803C` | :white_check_mark: (100.0%) |
| `StartSyncDevice` | `0x804E8098` | :x: (86.0%) |
| `WUDStartFastSyncSimple` | `0x804E81E4` | :x: (0.0%) |
| `WUDCancelSyncDevice` | `0x804E8224` | :x: (0.0%) |
| `WUDStopSyncSimple` | `0x804E82C4` | :white_check_mark: (100.0%) |
| `WUDSetDisableChannel` | `0x804E835C` | :white_check_mark: (100.0%) |
| `WUDSetHidRecvCallback` | `0x804E8448` | :white_check_mark: (100.0%) |
| `WUDSetHidConnCallback` | `0x804E84A4` | :white_check_mark: (100.0%) |
| `WUDSetVisibility` | `0x804E8500` | :white_check_mark: (100.0%) |
| `reset_again_cb` | `0x804E8558` | :white_check_mark: (100.0%) |
| `remove_patch_cb` | `0x804E855C` | :white_check_mark: (100.0%) |
| `write_patch_cb` | `0x804E86D4` | :white_check_mark: (100.0%) |
| `install_patch_cb` | `0x804E8904` | :white_check_mark: (100.0%) |
| `WUDiInitSub` | `0x804E89D8` | :x: (79.2%) |
| `WUDiRegisterDevice` | `0x804E8B68` | :x: (84.6%) |
| `WUDiRemoveDevice` | `0x804E8C98` | :x: (92.8%) |
| `WUDiGetDevInfo` | `0x804E8DF4` | :white_check_mark: (100.0%) |
| `WUDiGetNewDevInfo` | `0x804E8ECC` | :white_check_mark: (100.0%) |
| `WUDiMoveTopSmpDevInfoPtr` | `0x804E8F8C` | :white_check_mark: (100.0%) |
| `WUDiMoveBottomSmpDevInfoPtr` | `0x804E90A4` | :white_check_mark: (100.0%) |
| `WUDiMoveTopOfDisconnectedSmpDevice` | `0x804E91BC` | :white_check_mark: (100.0%) |
| `WUDiMoveTopStdDevInfoPtr` | `0x804E9314` | :white_check_mark: (100.0%) |
| `WUDiMoveBottomStdDevInfoPtr` | `0x804E942C` | :white_check_mark: (100.0%) |
| `WUDiMoveTopOfDisconnectedStdDevice` | `0x804E9544` | :white_check_mark: (100.0%) |
| `WUDIsBusy` | `0x804E969C` | :white_check_mark: (100.0%) |
| `CleanupCallback` | `0x804E9710` | :white_check_mark: (100.0%) |
| `WUDSecurityCallback` | `0x804E9744` | :x: (99.9%) |
| `WUDSearchCallback` | `0x804E9C98` | :x: (88.3%) |
| `WUDVendorSpecificCallback` | `0x804E9EA4` | :x: (85.9%) |
| `WUDDeviceStatusCallback` | `0x804EA198` | :white_check_mark: (100.0%) |
| `WUDStoredLinkKeyCallback` | `0x804EA1E8` | :x: (99.9%) |
| `WUDPowerManagerCallback` | `0x804EA4D4` | :x: (69.4%) |
| `_WUDGetDevAddr` | `0x804EA610` | :white_check_mark: (100.0%) |
| `_WUDGetQueuedSize` | `0x804EA664` | :white_check_mark: (100.0%) |
| `_WUDGetNotAckedSize` | `0x804EA6C0` | :white_check_mark: (100.0%) |
| `_WUDGetLinkNumber` | `0x804EA71C` | :white_check_mark: (100.0%) |