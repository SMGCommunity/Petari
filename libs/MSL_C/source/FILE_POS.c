#include "file_struct.h"
#include "buffer_io.h"
#include <cerrno>

long _ftell(FILE *pFile) {
    int buf = 0;
    long pos;

    unsigned char file_kind = pFile->mode.file;

    if (!(file_kind == file_disk || file_kind == file_console) || pFile->state.error) {
        errno = 40;
        return -1;
    }

    if (pFile->state.io_state == neutral) {
        return pFile->pos;
    }

    pos = pFile->buffer_pos + (pFile->buffer_ptr - pFile->buffer);

    if (pFile->state.io_state >= rereading) {
        buf = pFile->state.io_state - rereading + 1;
        pos -= buf;
    }

    if (!pFile->mode.binary) {
        long dist = pFile->buffer_ptr - pFile->buffer - buf;
        unsigned char* cur_buf = pFile->buffer;

        while (dist--) {
            if (*cur_buf++ == '\n') {
                pos++;
            }
        }
    }

    return pos;
}

long ftell(FILE *pFile) {
    return _ftell(pFile);
}

int _fseek(FILE *pFile, long offs, int file_mode) {
    __pos_proc position_proc_func;
    unsigned char file_kind = pFile->mode.file;

    if (!(file_kind == file_disk) || pFile->state.error) {
        errno = 40;
        return -1;
    }

    if (pFile->state.io_state == writing) {
        if (__flush_buffer(pFile, 0) != 0) {
            pFile->state.error = 1;
            pFile->buffer_len = 0;
            errno = 40;
            return -1;
        }
    }

    if (file_mode == 1) {
        file_mode = 0;
        offs += _ftell(pFile);
    }

    if ((file_mode != 2) && (pFile->mode.io != 3) && ((pFile->state.io_state == reading) || (pFile->state.io_state == rereading))) {
        if ((offs >= pFile->pos || offs < pFile->buffer_pos)) {
            pFile->state.io_state = neutral;
        }
        else {
            pFile->buffer_ptr = pFile->buffer + (offs - pFile->buffer_pos);
            pFile->buffer_len = pFile->pos - offs;
            pFile->state.io_state = reading;
        }
    }
    else {
        pFile->state.io_state = neutral;
    }

    if (pFile->state.io_state == neutral) {
        position_proc_func = pFile->pos_proc;

        if (position_proc_func != 0 && (*position_proc_func)(pFile->handle, &offs, file_mode, pFile->ref)) {
            pFile->state.error = 1;
            pFile->buffer_len = 0;
            errno = 40;
            return -1;
        }

        pFile->state.eof = 0;
        pFile->pos = offs;
        pFile->buffer_len = 0;
    }

    return 0;
}