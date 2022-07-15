# mem
| Symbol | Meaning 
| ------------- | ------------- 
| :x: | Object has not yet been started. 
| :eight_pointed_black_star: | Object is in progress. 
| :white_check_mark: | Object is completed. 


| Object | Percentage (of Bytes) | Functions Done / Total Functions | Percentage (Functions) | Status 
| ------------- | ------------- | ------------- | ------------- | ------------- 
| mem_heapCommon.o | 0.0% | 0 / 3 | 0.0% | :x: 
| mem_expHeap.o | 0.0% | 0 / 9 | 0.0% | :x: 
| mem_allocator.o | 0.0% | 0 / 5 | 0.0% | :x: 
| mem_list.o | 0.0% | 0 / 4 | 0.0% | :x: 


# mem_heapCommon.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| FindContainHeap_ | :x: |
| MEMiInitHeapHead | :x: |
| MEMiFinalizeHeap | :x: |


# mem_expHeap.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| AllocUsedBlockFromFreeBlock_ | :x: |
| AllocFromHead_ | :x: |
| AllocFromTail_ | :x: |
| RecycleRegion_ | :x: |
| MEMCreateExpHeapEx | :x: |
| MEMDestroyExpHeap | :x: |
| MEMAllocFromExpHeapEx | :x: |
| MEMFreeToExpHeap | :x: |
| MEMGetAllocatableSizeForExpHeapEx | :x: |


# mem_allocator.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| AllocatorAllocForExpHeap_ | :x: |
| AllocatorFreeForExpHeap_ | :x: |
| MEMAllocFromAllocator | :x: |
| MEMFreeToAllocator | :x: |
| MEMInitAllocatorForExpHeap | :x: |


# mem_list.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| MEMInitList | :x: |
| MEMAppendListObject | :x: |
| MEMRemoveListObject | :x: |
| MEMGetNextListObject | :x: |


