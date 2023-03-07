# JSupport
| Symbol | Meaning 
| ------------- | ------------- 
| :x: | Object has not yet been started. 
| :eight_pointed_black_star: | Object is in progress. 
| :white_check_mark: | Object is completed. 


| Object | Percentage (of Bytes) | Functions Done / Total Functions | Percentage (Functions) | Status 
| ------------- | ------------- | ------------- | ------------- | ------------- 
| JSUList.o | 89.61538461538461% | 9 / 10 | 90.0% | :eight_pointed_black_star: 
| JSUInputStream.o | 46.22641509433962% | 2 / 5 | 40.0% | :eight_pointed_black_star: 
| JSUOutputStream.o | 0.0% | 0 / 4 | 0.0% | :x: 
| JSUMemoryStream.o | 100.0% | 9 / 9 | 100.0% | :white_check_mark: 
| JSUFileStream.o | 0.0% | 0 / 5 | 0.0% | :x: 


# JSUList.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__10JSUPtrLinkFPv | :white_check_mark: |
| __dt__10JSUPtrLinkFv | :white_check_mark: |
| __ct__10JSUPtrListFb | :white_check_mark: |
| __dt__10JSUPtrListFv | :x: |
| initiate__10JSUPtrListFv | :white_check_mark: |
| setFirst__10JSUPtrListFP10JSUPtrLink | :white_check_mark: |
| append__10JSUPtrListFP10JSUPtrLink | :white_check_mark: |
| prepend__10JSUPtrListFP10JSUPtrLink | :white_check_mark: |
| insert__10JSUPtrListFP10JSUPtrLinkP10JSUPtrLink | :white_check_mark: |
| remove__10JSUPtrListFP10JSUPtrLink | :white_check_mark: |


# JSUInputStream.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| read__14JSUInputStreamFPvl | :white_check_mark: |
| skip__14JSUInputStreamFl | :x: |
| skip__20JSURandomInputStreamFl | :white_check_mark: |
| seek__20JSURandomInputStreamFl17JSUStreamSeekFrom | :x: |
| setState__10JSUIosBaseFQ210JSUIosBase8EIoState | :x: |


# JSUOutputStream.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| write__15JSUOutputStreamFPCvl | :x: |
| skip__15JSUOutputStreamFlSc | :x: |
| seek__21JSURandomOutputStreamFl17JSUStreamSeekFrom | :x: |
| getAvailable__21JSURandomOutputStreamCFv | :x: |


# JSUMemoryStream.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| setBuffer__20JSUMemoryInputStreamFPCvl | :white_check_mark: |
| readData__20JSUMemoryInputStreamFPvl | :white_check_mark: |
| seekPos__20JSUMemoryInputStreamFl17JSUStreamSeekFrom | :white_check_mark: |
| setBuffer__21JSUMemoryOutputStreamFPvl | :white_check_mark: |
| writeData__21JSUMemoryOutputStreamFPCvl | :white_check_mark: |
| seekPos__21JSUMemoryOutputStreamFl17JSUStreamSeekFrom | :white_check_mark: |
| getLength__21JSUMemoryOutputStreamCFv | :white_check_mark: |
| getPosition__20JSUMemoryInputStreamCFv | :white_check_mark: |
| getLength__20JSUMemoryInputStreamCFv | :white_check_mark: |


# JSUFileStream.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__18JSUFileInputStreamFP7JKRFile | :x: |
| readData__18JSUFileInputStreamFPvl | :x: |
| seekPos__18JSUFileInputStreamFl17JSUStreamSeekFrom | :x: |
| getPosition__18JSUFileInputStreamCFv | :x: |
| getLength__18JSUFileInputStreamCFv | :x: |


