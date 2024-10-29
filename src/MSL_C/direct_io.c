#include "buffer_io.h"
#include "file_struct.h"
#include "size_t.h"
#include "string_api.h"
#include "wchar_io.h"

void __stdio_atexit(void);
void* memcpy(void *, const void *, unsigned long);
int _fseek(FILE *, long, int);

size_t __fwrite(const void *pPtr, size_t memb_size, size_t num_memb, FILE *pFile) {
    unsigned char* cur_ptr;
    size_t num_bytes, rem_bytes, bytes_written;
    int res, buff;

    if (fwide(pFile, 0) == 0) {
        fwide(pFile, -1);   
    }

    rem_bytes = memb_size * num_memb;

    if (rem_bytes == 0 || pFile->state.error || pFile->mode.file == 0) {
        return 0;
    }

    if (pFile->mode.file == 2) {
        __stdio_atexit();
    }

    buff = (!pFile->mode.binary || pFile->mode.buffer == 2 || pFile->mode.buffer == 1);

    if (pFile->state.io_state == 0 && pFile->mode.io & 2) {
        if (pFile->mode.io & 4) {
            if (_fseek(pFile, 0, 2)) {
                return 0;
            }
        }

        pFile->state.io_state = 1;
        __prep_buffer(pFile);
    }

    if (pFile->state.io_state != 1) {
        pFile->state.error = 1;
        pFile->buffer_len = 0;
        return 0;
    }

    cur_ptr = (unsigned char*)pPtr;
    bytes_written = 0;

    if (rem_bytes && (pFile->buffer_ptr != pFile->buffer || buff)) {
        pFile->buffer_len = pFile->buffer_size - (pFile->buffer_ptr - pFile->buffer);

        do {
            unsigned char* nw = 0;
            num_bytes = pFile->buffer_len;

            if (num_bytes > rem_bytes) {
                num_bytes = rem_bytes;
            }

            if (pFile->mode.buffer == 1 && num_bytes) {
                if ((nw = (unsigned char*) __memrchr(cur_ptr, '\n', num_bytes)) != 0) {
                    num_bytes = nw + 1 - cur_ptr;
                }
            }

            if (num_bytes != 0) {
                memcpy(pFile->buffer_ptr, cur_ptr, num_bytes);
                cur_ptr += num_bytes;
                rem_bytes -= num_bytes;
                pFile->buffer_ptr += num_bytes;
                pFile->buffer_len -= num_bytes;
            }

            if (pFile->buffer_len == 0 || nw != 0 || (!pFile->mode.buffer)) {
                res = __flush_buffer(pFile, 0);

                if (res != 0) {
                    pFile->state.error = 1;
                    pFile->buffer_len = 0;
                    rem_bytes = 0;
                    break;
                }
            }

            bytes_written += num_bytes;

        } while(rem_bytes && buff);
    }

    if (rem_bytes && buff == 0) {
        unsigned char* save_buf = pFile->buffer;
        size_t save_size = pFile->buffer_size;

        pFile->buffer = cur_ptr;
        pFile->buffer_size = rem_bytes;
        pFile->buffer_ptr = cur_ptr + rem_bytes;

        if (__flush_buffer(pFile, &num_bytes) != 0) {
            pFile->state.error = 1;
            pFile->buffer_len = 0;
        }
        else {
            bytes_written += num_bytes;
        }
        
        pFile->buffer = save_buf;
        pFile->buffer_size = save_size;
        __prep_buffer(pFile);
        pFile->buffer_len = 0;
    }

    if (pFile->mode.buffer != 2) {
        pFile->buffer_len = 0;
    }

    return bytes_written / memb_size;
}