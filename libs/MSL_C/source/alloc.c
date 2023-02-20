typedef struct Block {
    struct Block* prev;
    struct Block* next;
    unsigned long _8;
    unsigned long _10;
    unsigned long max_size;
    unsigned long size;
} Block;

typedef struct SubBlock {
    unsigned long size;
    Block* block;
    struct SubBlock* prev;
    struct SubBlock* next;
} SubBlock;

/*
static void Block_link(Block *pBlock, SubBlock *pSubBlock) {
    SubBlock** st;
}

static SubBlock* SubBlock_merge_prev(SubBlock *pBlock, SubBlock **pStart) {

}
*/

static void SubBlock_merge_next(SubBlock *pBlock, SubBlock **pStart) {
    SubBlock* next_sub_block;
    unsigned long this_cur_size;

    next_sub_block = (SubBlock*)((char*)pBlock + (pBlock->size & 0xFFFFFFF8));

    if (!(next_sub_block->size & 2)) {
        this_cur_size = (pBlock->size & 0xFFFFFFF8) + (next_sub_block->size & 0xFFFFFFF8);

        pBlock->size &= ~0xFFFFFFF8;
        pBlock->size |= this_cur_size & 0xFFFFFFF8;

        if (!(pBlock->size & 2)) {
            *(unsigned long*)((char*)(pBlock) + (this_cur_size) - 4) = (this_cur_size);
        }

        if (!(pBlock->size & 2)) {
            *(unsigned long *)((char*)pBlock + this_cur_size) &= ~4;
        }
        else {
            *(unsigned long *)((char*)pBlock + this_cur_size) |= 4;
        }

        if (*pStart == next_sub_block) {
            *pStart = (*pStart)->next;
        }

        if (*pStart == next_sub_block) {
            *pStart = 0;
        }

        next_sub_block->next->prev = next_sub_block->prev;
        next_sub_block->prev->next = next_sub_block->next;
    }
}