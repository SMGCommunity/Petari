#include "file_struct.h"

void __prep_buffer(FILE *pFile) {
    pFile->buffer_ptr = pFile->buffer;
    pFile->buffer_len = pFile->buffer_size;
    pFile->buffer_len -= pFile->pos & pFile->buffer_alignment;
    pFile->buffer_pos = pFile->pos;
}

int __flush_buffer(FILE *pFile, size_t *pFlushed) {
    size_t len;
    int res;

    len = pFile->buffer_ptr - pFile->buffer;

    if (len != 0) {
        pFile->buffer_len = len;

        res = (*pFile->write_proc)(pFile->handle, pFile->buffer, (size_t*)&pFile->buffer_len, pFile->ref);
    
        if (pFlushed != 0) {
            *pFlushed = pFile->buffer_len;
        }

        if (res != 0) {
            return res;
        }

        pFile->pos += pFile->buffer_len;
    }

    __prep_buffer(pFile);
    return 0;
}