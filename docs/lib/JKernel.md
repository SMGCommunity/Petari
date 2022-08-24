# JKernel
| Symbol | Meaning 
| ------------- | ------------- 
| :x: | Object has not yet been started. 
| :eight_pointed_black_star: | Object is in progress. 
| :white_check_mark: | Object is completed. 


| Object | Percentage (of Bytes) | Functions Done / Total Functions | Percentage (Functions) | Status 
| ------------- | ------------- | ------------- | ------------- | ------------- 
| JKRHeap.o | 26.504065040650403% | 7 / 48 | 14.583333333333334% | :eight_pointed_black_star: 
| JKRExpHeap.o | 1.825442099258414% | 1 / 40 | 2.5% | :eight_pointed_black_star: 
| JKRSolidHeap.o | 0.0% | 0 / 20 | 0.0% | :x: 
| JKRUnitHeap.o | 0.0% | 0 / 26 | 0.0% | :x: 
| JKRDisposer.o | 100.0% | 3 / 3 | 100.0% | :white_check_mark: 
| JKRThread.o | 25.063938618925828% | 4 / 19 | 21.052631578947366% | :eight_pointed_black_star: 
| JKRAram.o | 0.0% | 0 / 14 | 0.0% | :x: 
| JKRAramHeap.o | 0.0% | 0 / 9 | 0.0% | :x: 
| JKRAramBlock.o | 0.0% | 0 / 5 | 0.0% | :x: 
| JKRAramPiece.o | 0.0% | 0 / 10 | 0.0% | :x: 
| JKRAramStream.o | 0.0% | 0 / 8 | 0.0% | :x: 
| JKRFileLoader.o | 75.0% | 8 / 11 | 72.72727272727273% | :eight_pointed_black_star: 
| JKRFileFinder.o | 83.50515463917526% | 3 / 4 | 75.0% | :eight_pointed_black_star: 
| JKRArchivePub.o | 96.25407166123779% | 20 / 21 | 95.23809523809523% | :eight_pointed_black_star: 
| JKRArchivePri.o | 59.01287553648069% | 12 / 16 | 75.0% | :eight_pointed_black_star: 
| JKRMemArchive.o | 62.98701298701299% | 10 / 13 | 76.92307692307693% | :eight_pointed_black_star: 
| JKRAramArchive.o | 6.128550074738415% | 1 / 10 | 10.0% | :eight_pointed_black_star: 
| JKRDvdArchive.o | 5.6865464632454925% | 1 / 8 | 12.5% | :eight_pointed_black_star: 
| JKRCompArchive.o | 0.0% | 0 / 8 | 0.0% | :x: 
| JKRDvdFile.o | 0.0% | 0 / 17 | 0.0% | :x: 
| JKRDvdRipper.o | 0.0% | 0 / 8 | 0.0% | :x: 
| JKRDvdAramRipper.o | 0.0% | 0 / 13 | 0.0% | :x: 
| JKRDecomp.o | 56.235827664399096% | 10 / 13 | 76.92307692307693% | :eight_pointed_black_star: 


# JKRHeap.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__7JKRHeapFPvUlP7JKRHeapb | :x: |
| __dt__17JSULink&lt;7JKRHeap&gt;Fv | :white_check_mark: |
| __dt__17JSUTree&lt;7JKRHeap&gt;Fv | :x: |
| __dt__22JSUList&lt;11JKRDisposer&gt;Fv | :x: |
| __dt__7JKRHeapFv | :white_check_mark: |
| initArena__7JKRHeapFPPcPUli | :x: |
| becomeSystemHeap__7JKRHeapFv | :x: |
| becomeCurrentHeap__7JKRHeapFv | :white_check_mark: |
| destroy__7JKRHeapFP7JKRHeap | :x: |
| alloc__7JKRHeapFUliP7JKRHeap | :white_check_mark: |
| alloc__7JKRHeapFUli | :x: |
| free__7JKRHeapFPvP7JKRHeap | :white_check_mark: |
| free__7JKRHeapFPv | :x: |
| callAllDisposer__7JKRHeapFv | :white_check_mark: |
| freeAll__7JKRHeapFv | :x: |
| freeTail__7JKRHeapFv | :x: |
| resize__7JKRHeapFPvUl | :x: |
| getFreeSize__7JKRHeapFv | :x: |
| getMaxFreeBlock__7JKRHeapFv | :x: |
| getTotalFreeSize__7JKRHeapFv | :x: |
| findFromRoot__7JKRHeapFPv | :white_check_mark: |
| find__7JKRHeapCFPv | :x: |
| findAllHeap__7JKRHeapCFPv | :x: |
| dispose_subroutine__7JKRHeapFUlUl | :x: |
| dispose__7JKRHeapFPvUl | :x: |
| dispose__7JKRHeapFPvPv | :x: |
| dispose__7JKRHeapFv | :x: |
| copyMemory__7JKRHeapFPvPvUl | :x: |
| JKRDefaultMemoryErrorRoutine__FPvUli | :x: |
| setErrorHandler__7JKRHeapFPFPvUli_v | :x: |
| __nw__FUl | :x: |
| __nw__FUli | :x: |
| __nw__FUlP7JKRHeapi | :x: |
| __nwa__FUl | :x: |
| __nwa__FUli | :x: |
| __nwa__FUlP7JKRHeapi | :x: |
| __dl__FPv | :x: |
| __dla__FPv | :x: |
| state_register__7JKRHeapCFPQ27JKRHeap6TStateUl | :x: |
| state_compare__7JKRHeapCFRCQ27JKRHeap6TStateRCQ27JKRHeap6TState | :x: |
| state_dump__7JKRHeapCFRCQ27JKRHeap6TState | :x: |
| setAltAramStartAdr__7JKRHeapFUl | :x: |
| getAltAramStartAdr__7JKRHeapFv | :x: |
| do_changeGroupID__7JKRHeapFUc | :x: |
| do_getCurrentGroupId__7JKRHeapFv | :x: |
| dump_sort__7JKRHeapFv | :x: |
| __pp__25JSUTreeIterator&lt;7JKRHeap&gt;Fv | :x: |
| __pp__30JSUListIterator&lt;11JKRDisposer&gt;Fi | :x: |


# JKRExpHeap.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| createRoot__10JKRExpHeapFib | :white_check_mark: |
| create__10JKRExpHeapFUlP7JKRHeapb | :x: |
| create__10JKRExpHeapFPvUlP7JKRHeapb | :x: |
| do_destroy__10JKRExpHeapFv | :x: |
| __ct__10JKRExpHeapFPvUlP7JKRHeapb | :x: |
| __dt__10JKRExpHeapFv | :x: |
| do_alloc__10JKRExpHeapFUli | :x: |
| allocFromHead__10JKRExpHeapFUli | :x: |
| allocFromHead__10JKRExpHeapFUl | :x: |
| allocFromTail__10JKRExpHeapFUli | :x: |
| allocFromTail__10JKRExpHeapFUl | :x: |
| do_free__10JKRExpHeapFPv | :x: |
| do_freeAll__10JKRExpHeapFv | :x: |
| do_freeTail__10JKRExpHeapFv | :x: |
| do_fillFreeArea__10JKRExpHeapFv | :x: |
| do_changeGroupID__10JKRExpHeapFUc | :x: |
| do_resize__10JKRExpHeapFPvUl | :x: |
| do_getSize__10JKRExpHeapFPv | :x: |
| do_getFreeSize__10JKRExpHeapFv | :x: |
| do_getMaxFreeBlock__10JKRExpHeapFv | :x: |
| do_getTotalFreeSize__10JKRExpHeapFv | :x: |
| getUsedSize__10JKRExpHeapCFUc | :x: |
| isEmpty__10JKRExpHeapFv | :x: |
| appendUsedList__10JKRExpHeapFPQ210JKRExpHeap9CMemBlock | :x: |
| setFreeBlock__10JKRExpHeapFPQ210JKRExpHeap9CMemBlockPQ210JKRExpHeap9CMemBlockPQ210JKRExpHeap9CMemBlock | :x: |
| removeFreeBlock__10JKRExpHeapFPQ210JKRExpHeap9CMemBlock | :x: |
| removeUsedBlock__10JKRExpHeapFPQ210JKRExpHeap9CMemBlock | :x: |
| recycleFreeBlock__10JKRExpHeapFPQ210JKRExpHeap9CMemBlock | :x: |
| joinTwoBlocks__10JKRExpHeapFPQ210JKRExpHeap9CMemBlock | :x: |
| check__10JKRExpHeapFv | :x: |
| dump__10JKRExpHeapFv | :x: |
| dump_sort__10JKRExpHeapFv | :x: |
| initiate__Q210JKRExpHeap9CMemBlockFPQ210JKRExpHeap9CMemBlockPQ210JKRExpHeap9CMemBlockUlUcUc | :x: |
| allocFore__Q210JKRExpHeap9CMemBlockFUlUcUcUcUc | :x: |
| allocBack__Q210JKRExpHeap9CMemBlockFUlUcUcUcUc | :x: |
| getHeapBlock__Q210JKRExpHeap9CMemBlockFPv | :x: |
| state_register__10JKRExpHeapCFPQ27JKRHeap6TStateUl | :x: |
| state_compare__10JKRExpHeapCFRCQ27JKRHeap6TStateRCQ27JKRHeap6TState | :x: |
| do_getCurrentGroupId__10JKRExpHeapFv | :x: |
| getHeapType__10JKRExpHeapFv | :x: |


# JKRSolidHeap.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| create__12JKRSolidHeapFUlP7JKRHeapb | :x: |
| do_destroy__12JKRSolidHeapFv | :x: |
| __ct__12JKRSolidHeapFPvUlP7JKRHeapb | :x: |
| __dt__12JKRSolidHeapFv | :x: |
| do_alloc__12JKRSolidHeapFUli | :x: |
| allocFromHead__12JKRSolidHeapFUli | :x: |
| allocFromTail__12JKRSolidHeapFUli | :x: |
| do_freeAll__12JKRSolidHeapFv | :x: |
| do_freeTail__12JKRSolidHeapFv | :x: |
| do_fillFreeArea__12JKRSolidHeapFv | :x: |
| do_resize__12JKRSolidHeapFPvUl | :x: |
| do_getSize__12JKRSolidHeapFPv | :x: |
| check__12JKRSolidHeapFv | :x: |
| dump__12JKRSolidHeapFv | :x: |
| state_register__12JKRSolidHeapCFPQ27JKRHeap6TStateUl | :x: |
| state_compare__12JKRSolidHeapCFRCQ27JKRHeap6TStateRCQ27JKRHeap6TState | :x: |
| do_getTotalFreeSize__12JKRSolidHeapFv | :x: |
| do_getMaxFreeBlock__12JKRSolidHeapFv | :x: |
| do_getFreeSize__12JKRSolidHeapFv | :x: |
| getHeapType__12JKRSolidHeapFv | :x: |


# JKRUnitHeap.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| create__11JKRUnitHeapFUlUlUlP7JKRHeapb | :x: |
| do_destroy__11JKRUnitHeapFv | :x: |
| __ct__11JKRUnitHeapFPUcPUcUlUlUlUlP7JKRHeapb | :x: |
| __dt__11JKRUnitHeapFv | :x: |
| clearBatArea__11JKRUnitHeapFv | :x: |
| find1FreeBlock__11JKRUnitHeapFi | :x: |
| findFreeBlock__11JKRUnitHeapFiUl | :x: |
| findFreeBlock_fromHead__11JKRUnitHeapFUl | :x: |
| findFreeBlock_fromTail__11JKRUnitHeapFUl | :x: |
| do_freeAll__11JKRUnitHeapFv | :x: |
| do_freeTail__11JKRUnitHeapFv | :x: |
| do_fillFreeArea__11JKRUnitHeapFv | :x: |
| do_resize__11JKRUnitHeapFPvUl | :x: |
| do_getSize__11JKRUnitHeapFPv | :x: |
| do_getFreeSize__11JKRUnitHeapFv | :x: |
| do_getMaxFreeBlock__11JKRUnitHeapFv | :x: |
| do_getTotalFreeSize__11JKRUnitHeapFv | :x: |
| indexToAddress__11JKRUnitHeapFi | :x: |
| addressToIndex__11JKRUnitHeapFPv | :x: |
| isUnitUsed__11JKRUnitHeapCFi | :x: |
| setUnitUsed__11JKRUnitHeapFi | :x: |
| check__11JKRUnitHeapFv | :x: |
| dump__11JKRUnitHeapFv | :x: |
| state_register__11JKRUnitHeapCFPQ27JKRHeap6TStateUl | :x: |
| state_compare__11JKRUnitHeapCFRCQ27JKRHeap6TStateRCQ27JKRHeap6TState | :x: |
| getHeapType__11JKRUnitHeapFv | :x: |


# JKRDisposer.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__11JKRDisposerFv | :white_check_mark: |
| __dt__22JSULink&lt;11JKRDisposer&gt;Fv | :white_check_mark: |
| __dt__11JKRDisposerFv | :white_check_mark: |


# JKRThread.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__9JKRThreadFUlii | :x: |
| __ct__9JKRThreadFP7JKRHeapUlii | :x: |
| __dt__9JKRThreadFv | :white_check_mark: |
| setCommon_mesgQueue__9JKRThreadFP7JKRHeapi | :white_check_mark: |
| setCommon_heapSpecified__9JKRThreadFP7JKRHeapUli | :x: |
| start__9JKRThreadFPv | :white_check_mark: |
| searchThread__9JKRThreadFP8OSThread | :white_check_mark: |
| enter__15JKRThreadSwitchFP9JKRThreadi | :x: |
| __as__Q28JUtility6TColorFRCQ28JUtility6TColor | :x: |
| __sinit_\JKRThread_cpp | :x: |
| __ct__Q29JKRThread5TLoadFv | :x: |
| clear__Q29JKRThread5TLoadFv | :x: |
| __dt__19JSULink&lt;9JKRThread&gt;Fv | :x: |
| run__9JKRThreadFv | :x: |
| __ct__Q28JUtility6TColorFUcUcUcUc | :x: |
| __ct__19JSUList&lt;9JKRThread&gt;Fb | :x: |
| __dt__19JSUList&lt;9JKRThread&gt;Fv | :x: |
| __ct__17JSUList&lt;7JKRTask&gt;Fv | :x: |
| __dt__17JSUList&lt;7JKRTask&gt;Fv | :x: |


# JKRAram.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| create__7JKRAramFUlUllll | :x: |
| __dt__7JKRAramFv | :x: |
| run__7JKRAramFv | :x: |
| checkOkAddress__7JKRAramFPUcUlP12JKRAramBlockUl | :x: |
| changeGroupIdIfNeed__7JKRAramFPUci | :x: |
| mainRamToAram__7JKRAramFPUcUlUl15JKRExpandSwitchUlP7JKRHeapiPUl | :x: |
| aramToMainRam__7JKRAramFUlPUcUl15JKRExpandSwitchUlP7JKRHeapiPUl | :x: |
| JKRDecompressFromAramToMainRam__FUlPvUlUlUlPUl | :x: |
| __sinit_\JKRAram_cpp | :x: |
| JKRCheckCompressed_noASR__FPUc | :x: |
| JKRDecompExpandSize__FPUc | :x: |
| decideAramGroupId__7JKRAramFi | :x: |
| __ct__23JSUList&lt;12JKRAMCommand&gt;Fv | :x: |
| __dt__23JSUList&lt;12JKRAMCommand&gt;Fv | :x: |


# JKRAramHeap.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__11JKRAramHeapFUlUl | :x: |
| __dt__11JKRAramHeapFv | :x: |
| alloc__11JKRAramHeapFUlQ211JKRAramHeap10EAllocMode | :x: |
| allocFromHead__11JKRAramHeapFUl | :x: |
| allocFromTail__11JKRAramHeapFUl | :x: |
| __sinit_\JKRAramHeap_cpp | :x: |
| __pp__31JSUListIterator&lt;12JKRAramBlock&gt;Fi | :x: |
| __ct__23JSUList&lt;12JKRAramBlock&gt;Fv | :x: |
| __dt__23JSUList&lt;12JKRAramBlock&gt;Fv | :x: |


# JKRAramBlock.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__12JKRAramBlockFUlUlUlUcb | :x: |
| __dt__23JSULink&lt;12JKRAramBlock&gt;Fv | :x: |
| __dt__12JKRAramBlockFv | :x: |
| allocHead__12JKRAramBlockFUlUcP11JKRAramHeap | :x: |
| allocTail__12JKRAramBlockFUlUcP11JKRAramHeap | :x: |


# JKRAramPiece.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| prepareCommand__12JKRAramPieceFiUlUlUlP12JKRAramBlockPFUl_v | :x: |
| sendCommand__12JKRAramPieceFP12JKRAMCommand | :x: |
| orderAsync__12JKRAramPieceFiUlUlUlP12JKRAramBlockPFUl_v | :x: |
| sync__12JKRAramPieceFP12JKRAMCommandi | :x: |
| orderSync__12JKRAramPieceFiUlUlUlP12JKRAramBlock | :x: |
| doneDMA__12JKRAramPieceFUl | :x: |
| __ct__12JKRAMCommandFv | :x: |
| __dt__23JSULink&lt;12JKRAMCommand&gt;Fv | :x: |
| __dt__12JKRAMCommandFv | :x: |
| __sinit_\JKRAramPiece_cpp | :x: |


# JKRAramStream.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| create__13JKRAramStreamFl | :x: |
| __ct__13JKRAramStreamFl | :x: |
| __dt__13JKRAramStreamFv | :x: |
| run__13JKRAramStreamFv | :x: |
| writeToAram__13JKRAramStreamFP20JKRAramStreamCommand | :x: |
| write_StreamToAram_Async__13JKRAramStreamFP18JSUFileInputStreamUlUlUlPUl | :x: |
| sync__13JKRAramStreamFP20JKRAramStreamCommandi | :x: |
| setTransBuffer__13JKRAramStreamFPUcUlP7JKRHeap | :x: |


# JKRFileLoader.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__13JKRFileLoaderFv | :white_check_mark: |
| __dt__24JSULink&lt;13JKRFileLoader&gt;Fv | :x: |
| __dt__13JKRFileLoaderFv | :white_check_mark: |
| unmount__13JKRFileLoaderFv | :white_check_mark: |
| getGlbResource__13JKRFileLoaderFPCcP13JKRFileLoader | :white_check_mark: |
| initializeVolumeList__13JKRFileLoaderFv | :white_check_mark: |
| prependVolumeList__13JKRFileLoaderFP24JSULink&lt;13JKRFileLoader&gt; | :white_check_mark: |
| removeVolumeList__13JKRFileLoaderFP24JSULink&lt;13JKRFileLoader&gt; | :white_check_mark: |
| __sinit_\JKRFileLoader_cpp | :white_check_mark: |
| __ct__24JSUList&lt;13JKRFileLoader&gt;Fv | :x: |
| __dt__24JSUList&lt;13JKRFileLoader&gt;Fv | :x: |


# JKRFileFinder.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__12JKRArcFinderFP10JKRArchivell | :white_check_mark: |
| findNextFile__12JKRArcFinderFv | :white_check_mark: |
| __ct__13JKRFileFinderFv | :white_check_mark: |
| __dt__12JKRArcFinderFv | :x: |


# JKRArchivePub.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| check_mount_already__10JKRArchiveFl | :white_check_mark: |
| check_mount_already__10JKRArchiveFlP7JKRHeap | :white_check_mark: |
| mount__10JKRArchiveFPCcQ210JKRArchive10EMountModeP7JKRHeapQ210JKRArchive15EMountDirection | :x: |
| mount__10JKRArchiveFlQ210JKRArchive10EMountModeP7JKRHeapQ210JKRArchive15EMountDirection | :white_check_mark: |
| becomeCurrent__10JKRArchiveFPCc | :white_check_mark: |
| getDirEntry__10JKRArchiveCFPQ210JKRArchive9SDirEntryUl | :white_check_mark: |
| getResource__10JKRArchiveFPCc | :white_check_mark: |
| getResource__10JKRArchiveFUlPCc | :white_check_mark: |
| getIdxResource__10JKRArchiveFUl | :white_check_mark: |
| getResource__10JKRArchiveFUs | :white_check_mark: |
| readResource__10JKRArchiveFPvUlUlPCc | :white_check_mark: |
| readResource__10JKRArchiveFPvUlPCc | :white_check_mark: |
| readResource__10JKRArchiveFPvUlUs | :white_check_mark: |
| removeResourceAll__10JKRArchiveFv | :white_check_mark: |
| removeResource__10JKRArchiveFPv | :white_check_mark: |
| detachResource__10JKRArchiveFPv | :white_check_mark: |
| getResSize__10JKRArchiveCFPCv | :white_check_mark: |
| countResource__10JKRArchiveCFv | :white_check_mark: |
| countFile__10JKRArchiveCFPCc | :white_check_mark: |
| getFirstFile__10JKRArchiveCFPCc | :white_check_mark: |
| getFileAttribute__10JKRArchiveCFUl | :white_check_mark: |


# JKRArchivePri.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__10JKRArchiveFv | :white_check_mark: |
| __ct__10JKRArchiveFlQ210JKRArchive10EMountMode | :white_check_mark: |
| __dt__10JKRArchiveFv | :white_check_mark: |
| isSameName__10JKRArchiveCFRQ210JKRArchive8CArcNameUlUs | :white_check_mark: |
| findResType__10JKRArchiveCFUl | :white_check_mark: |
| findDirectory__10JKRArchiveCFPCcUl | :x: |
| findTypeResource__10JKRArchiveCFUlPCc | :white_check_mark: |
| findFsResource__10JKRArchiveCFPCcUl | :x: |
| findIdxResource__10JKRArchiveCFUl | :white_check_mark: |
| findNameResource__10JKRArchiveCFPCc | :white_check_mark: |
| findPtrResource__10JKRArchiveCFPCv | :white_check_mark: |
| findIdResource__10JKRArchiveCFUs | :white_check_mark: |
| store__Q210JKRArchive8CArcNameFPCc | :x: |
| store__Q210JKRArchive8CArcNameFPCcc | :x: |
| setExpandSize__10JKRArchiveFPQ210JKRArchive12SDIFileEntryUl | :white_check_mark: |
| getExpandSize__10JKRArchiveCFPQ210JKRArchive12SDIFileEntry | :white_check_mark: |


# JKRMemArchive.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__13JKRMemArchiveFv | :white_check_mark: |
| __ct__13JKRMemArchiveFlQ210JKRArchive15EMountDirection | :white_check_mark: |
| __dt__13JKRMemArchiveFv | :white_check_mark: |
| fixedInit__13JKRMemArchiveFl | :white_check_mark: |
| mountFixed__13JKRMemArchiveFPv15JKRMemBreakFlag | :white_check_mark: |
| open__13JKRMemArchiveFlQ210JKRArchive15EMountDirection | :x: |
| open__13JKRMemArchiveFPvUl15JKRMemBreakFlag | :x: |
| fetchResource__13JKRMemArchiveFPQ210JKRArchive12SDIFileEntryPUl | :white_check_mark: |
| fetchResource__13JKRMemArchiveFPvUlPQ210JKRArchive12SDIFileEntryPUl | :white_check_mark: |
| removeResourceAll__13JKRMemArchiveFv | :white_check_mark: |
| removeResource__13JKRMemArchiveFPv | :white_check_mark: |
| fetchResource_subroutine__13JKRMemArchiveFPUcUlPUcUli | :x: |
| getExpandedResSize__13JKRMemArchiveCFPCv | :white_check_mark: |


# JKRAramArchive.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__14JKRAramArchiveFlQ210JKRArchive15EMountDirection | :white_check_mark: |
| __dt__14JKRAramArchiveFv | :x: |
| open__14JKRAramArchiveFl | :x: |
| fetchResource__14JKRAramArchiveFPQ210JKRArchive12SDIFileEntryPUl | :x: |
| fetchResource__14JKRAramArchiveFPvUlPQ210JKRArchive12SDIFileEntryPUl | :x: |
| fetchResource_subroutine__14JKRAramArchiveFUlUlPUcUli | :x: |
| fetchResource_subroutine__14JKRAramArchiveFUlUlP7JKRHeapiPPUc | :x: |
| getExpandedResSize__14JKRAramArchiveCFPCv | :x: |
| __dt__7JKRFileFv | :x: |
| JKRConvertAttrToCompressionType__Fi | :x: |


# JKRDvdArchive.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__13JKRDvdArchiveFlQ210JKRArchive15EMountDirection | :white_check_mark: |
| __dt__13JKRDvdArchiveFv | :x: |
| open__13JKRDvdArchiveFl | :x: |
| fetchResource__13JKRDvdArchiveFPQ210JKRArchive12SDIFileEntryPUl | :x: |
| fetchResource__13JKRDvdArchiveFPvUlPQ210JKRArchive12SDIFileEntryPUl | :x: |
| fetchResource_subroutine__13JKRDvdArchiveFlUlUlPUcUlii | :x: |
| fetchResource_subroutine__13JKRDvdArchiveFlUlUlP7JKRHeapiiPPUc | :x: |
| getExpandedResSize__13JKRDvdArchiveCFPCv | :x: |


# JKRCompArchive.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__14JKRCompArchiveFlQ210JKRArchive15EMountDirection | :x: |
| __dt__14JKRCompArchiveFv | :x: |
| open__14JKRCompArchiveFl | :x: |
| fetchResource__14JKRCompArchiveFPQ210JKRArchive12SDIFileEntryPUl | :x: |
| fetchResource__14JKRCompArchiveFPvUlPQ210JKRArchive12SDIFileEntryPUl | :x: |
| removeResourceAll__14JKRCompArchiveFv | :x: |
| removeResource__14JKRCompArchiveFPv | :x: |
| getExpandedResSize__14JKRCompArchiveCFPCv | :x: |


# JKRDvdFile.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__10JKRDvdFileFv | :x: |
| __ct__10JKRDvdFileFl | :x: |
| __dt__10JKRDvdFileFv | :x: |
| initiate__10JKRDvdFileFv | :x: |
| open__10JKRDvdFileFPCc | :x: |
| open__10JKRDvdFileFl | :x: |
| close__10JKRDvdFileFv | :x: |
| readData__10JKRDvdFileFPvll | :x: |
| writeData__10JKRDvdFileFPCvll | :x: |
| sync__10JKRDvdFileFv | :x: |
| doneProcess__10JKRDvdFileFlP11DVDFileInfo | :x: |
| __sinit_\JKRDvdFile_cpp | :x: |
| __ct__7JKRFileFv | :x: |
| __dt__21JSULink&lt;10JKRDvdFile&gt;Fv | :x: |
| getFileSize__10JKRDvdFileCFv | :x: |
| __ct__21JSUList&lt;10JKRDvdFile&gt;Fv | :x: |
| __dt__21JSUList&lt;10JKRDvdFile&gt;Fv | :x: |


# JKRDvdRipper.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| loadToMainRAM__12JKRDvdRipperFPCcPUc15JKRExpandSwitchUlP7JKRHeapQ212JKRDvdRipper15EAllocDirectionUlPiPUl | :x: |
| loadToMainRAM__12JKRDvdRipperFlPUc15JKRExpandSwitchUlP7JKRHeapQ212JKRDvdRipper15EAllocDirectionUlPiPUl | :x: |
| loadToMainRAM__12JKRDvdRipperFP10JKRDvdFilePUc15JKRExpandSwitchUlP7JKRHeapQ212JKRDvdRipper15EAllocDirectionUlPiPUl | :x: |
| JKRDecompressFromDVD__FP10JKRDvdFilePvUlUlUlUlPUl | :x: |
| __sinit_\JKRDvdRipper_cpp | :x: |
| __ct__23JSUList&lt;12JKRDMCommand&gt;Fv | :x: |
| __dt__23JSUList&lt;12JKRDMCommand&gt;Fv | :x: |
| isErrorRetry__12JKRDvdRipperFv | :x: |


# JKRDvdAramRipper.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| loadToAram__16JKRDvdAramRipperFlUl15JKRExpandSwitchUlUlPUl | :x: |
| loadToAram__16JKRDvdAramRipperFP10JKRDvdFileUl15JKRExpandSwitchUlUlPUl | :x: |
| loadToAram_Async__16JKRDvdAramRipperFP10JKRDvdFileUl15JKRExpandSwitchPFUl_vUlUlPUl | :x: |
| callCommand_Async__16JKRDvdAramRipperFP12JKRADCommand | :x: |
| syncAram__16JKRDvdAramRipperFP12JKRADCommandi | :x: |
| __ct__12JKRADCommandFv | :x: |
| __dt__12JKRADCommandFv | :x: |
| JKRDecompressFromDVDToAram__FP10JKRDvdFileUlUlUlUlUlPUl | :x: |
| dmaBufferFlush__FUl | :x: |
| __sinit_\JKRDvdAramRipper_cpp | :x: |
| __dt__18JSUFileInputStreamFv | :x: |
| __ct__23JSUList&lt;12JKRADCommand&gt;Fv | :x: |
| __dt__23JSUList&lt;12JKRADCommand&gt;Fv | :x: |


# JKRDecomp.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| create__9JKRDecompFl | :white_check_mark: |
| __ct__9JKRDecompFl | :white_check_mark: |
| __dt__9JKRDecompFv | :white_check_mark: |
| run__9JKRDecompFv | :white_check_mark: |
| prepareCommand__9JKRDecompFPUcPUcUlUlPFUl_v | :white_check_mark: |
| sendCommand__9JKRDecompFP16JKRDecompCommand | :white_check_mark: |
| sync__9JKRDecompFP16JKRDecompCommandi | :white_check_mark: |
| orderSync__9JKRDecompFPUcPUcUlUl | :x: |
| decode__9JKRDecompFPUcPUcUlUl | :white_check_mark: |
| decodeSZP__9JKRDecompFPUcPUcUlUl | :x: |
| decodeSZS__9JKRDecompFPUcPUcUlUl | :x: |
| checkCompressed__9JKRDecompFPUc | :white_check_mark: |
| __ct__16JKRDecompCommandFv | :white_check_mark: |


