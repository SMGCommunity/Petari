# vi
| Symbol | Meaning 
| ------------- | ------------- 
| :x: | Object has not yet been started. 
| :eight_pointed_black_star: | Object is in progress. 
| :white_check_mark: | Object is completed. 


| Object | Percentage (of Bytes) | Functions Done / Total Functions | Percentage (Functions) | Status 
| ------------- | ------------- | ------------- | ------------- | ------------- 
| vi.o | 22.463768115942027% | 12 / 28 | 42.857142857142854% | :eight_pointed_black_star: 
| i2c.o | 100.0% | 3 / 3 | 100.0% | :white_check_mark: 
| vi3in1.o | 1.2354152367879203% | 1 / 14 | 7.142857142857142% | :eight_pointed_black_star: 


# vi.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| OnShutdown | :x: |
| __VIRetraceHandler | :x: |
| VISetPreRetraceCallback | :white_check_mark: |
| VISetPostRetraceCallback | :white_check_mark: |
| getTiming | :white_check_mark: |
| __VIInit | :x: |
| VIInit | :x: |
| VIWaitForRetrace | :white_check_mark: |
| setFbbRegs | :white_check_mark: |
| setHorizontalRegs | :white_check_mark: |
| setVerticalRegs | :white_check_mark: |
| VIConfigure | :x: |
| VIConfigurePan | :x: |
| VIFlush | :white_check_mark: |
| VISetNextFrameBuffer | :x: |
| VIGetNextFrameBuffer | :x: |
| VIGetCurrentFrameBuffer | :x: |
| VISetBlack | :white_check_mark: |
| VIGetRetraceCount | :x: |
| VIGetCurrentLine | :white_check_mark: |
| VIGetTvFormat | :white_check_mark: |
| VIGetDTVStatus | :x: |
| __VIDisplayPositionToXY | :x: |
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
| __VISetYUVSEL | :x: |
| __VISetFilter4EURGB60 | :x: |
| __VISetCGMS | :x: |
| __VISetWSS | :x: |
| __VISetClosedCaption | :x: |
| __VISetMacrovision | :x: |
| __VISetGammaImm | :x: |
| __VISetGamma | :x: |
| __VISetTrapFilter | :x: |
| VISetTrapFilter | :x: |
| __VISetRGBOverDrive | :x: |
| VISetRGBModeImm | :x: |
| __VISetRGBModeImm | :white_check_mark: |
| __VISetRevolutionModeSimple | :x: |


