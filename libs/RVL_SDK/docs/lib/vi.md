# vi
| Symbol | Meaning 
| ------------- | ------------- 
| :x: | Object has not yet been started. 
| :eight_pointed_black_star: | Object is in progress. 
| :white_check_mark: | Object is completed. 


| Object | Percentage (of Bytes) | Functions Done / Total Functions | Percentage (Functions) | Status 
| ------------- | ------------- | ------------- | ------------- | ------------- 
| vi.o | 95.68840579710145% | 21 / 28 | 75.0% | :eight_pointed_black_star: 
| i2c.o | 100.0% | 3 / 3 | 100.0% | :white_check_mark: 
| vi3in1.o | 100.0% | 14 / 14 | 100.0% | :white_check_mark: 


# vi.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| OnShutdown | :white_check_mark: |
| __VIRetraceHandler | :white_check_mark: |
| VISetPreRetraceCallback | :white_check_mark: |
| VISetPostRetraceCallback | :white_check_mark: |
| getTiming | :white_check_mark: |
| __VIInit | :white_check_mark: |
| VIInit | :white_check_mark: |
| VIWaitForRetrace | :white_check_mark: |
| setFbbRegs | :white_check_mark: |
| setHorizontalRegs | :white_check_mark: |
| setVerticalRegs | :white_check_mark: |
| VIConfigure | :white_check_mark: |
| VIConfigurePan | :white_check_mark: |
| VIFlush | :white_check_mark: |
| VISetNextFrameBuffer | :white_check_mark: |
| VIGetNextFrameBuffer | :x: |
| VIGetCurrentFrameBuffer | :x: |
| VISetBlack | :white_check_mark: |
| VIGetRetraceCount | :white_check_mark: |
| VIGetCurrentLine | :white_check_mark: |
| VIGetTvFormat | :white_check_mark: |
| VIGetDTVStatus | :x: |
| __VIDisplayPositionToXY | :white_check_mark: |
| VIGetDimmingCount | :x: |
| VIEnableDimming | :x: |
| VISetTimeToDimming | :x: |
| VIResetDimmingCount | :white_check_mark: |
| __VIResetRFIdle | :x: |


# i2c.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| WaitMicroTime | :white_check_mark: |
| sendSlaveAddr | :white_check_mark: |
| __VISendI2CData | :white_check_mark: |


# vi3in1.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __VISetYUVSEL | :white_check_mark: |
| __VISetFilter4EURGB60 | :white_check_mark: |
| __VISetCGMS | :white_check_mark: |
| __VISetWSS | :white_check_mark: |
| __VISetClosedCaption | :white_check_mark: |
| __VISetMacrovision | :white_check_mark: |
| __VISetGammaImm | :white_check_mark: |
| __VISetGamma | :white_check_mark: |
| __VISetTrapFilter | :white_check_mark: |
| VISetTrapFilter | :white_check_mark: |
| __VISetRGBOverDrive | :white_check_mark: |
| VISetRGBModeImm | :white_check_mark: |
| __VISetRGBModeImm | :white_check_mark: |
| __VISetRevolutionModeSimple | :white_check_mark: |


