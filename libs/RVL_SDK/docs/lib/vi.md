# vi
| Symbol | Meaning 
| ------------- | ------------- 
| :x: | Object has not yet been started. 
| :eight_pointed_black_star: | Object is in progress. 
| :white_check_mark: | Object is completed. 


| Object | Percentage (of Bytes) | Functions Done / Total Functions | Percentage (Functions) | Status 
| ------------- | ------------- | ------------- | ------------- | ------------- 
| vi.o | 18.695652173913043% | 9 / 28 | 32.142857142857146% | :eight_pointed_black_star: 
| i2c.o | 0.0% | 0 / 3 | 0.0% | :x: 
| vi3in1.o | 0.0% | 0 / 14 | 0.0% | :x: 


# vi.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| OnShutdown | :x: |
| __VIRetraceHandler | :x: |
| VISetPreRetraceCallback | :x: |
| VISetPostRetraceCallback | :x: |
| getTiming | :x: |
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
| WaitMicroTime | :x: |
| sendSlaveAddr | :x: |
| __VISendI2CData | :x: |


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
| __VISetRGBModeImm | :x: |
| __VISetRevolutionModeSimple | :x: |


