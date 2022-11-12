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
| mem_allocator.o | 100.0% | 5 / 5 | 100.0% | :white_check_mark: 
| mem_list.o | 100.0% | 4 / 4 | 100.0% | :white_check_mark: 


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
| AllocatorAllocForExpHeap_ | :white_check_mark: |
| AllocatorFreeForExpHeap_ | :white_check_mark: |
| MEMAllocFromAllocator | :white_check_mark: |
| MEMFreeToAllocator | :white_check_mark: |
| MEMInitAllocatorForExpHeap | :white_check_mark: |


# mem_list.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| MEMInitList | :white_check_mark: |
| MEMAppendListObject | :white_check_mark: |
| MEMRemoveListObject | :white_check_mark: |
| MEMGetNextListObject | :white_check_mark: |


