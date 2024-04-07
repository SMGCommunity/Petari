#ifndef RVL_FACE_LIBRARY_INTERNAL_FORMAT_H
#define RVL_FACE_LIBRARY_INTERNAL_FORMAT_H
#include <RFLi_Types.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct RFLiTableData {
    RFLCreateID createID; // at 0x0

    // at 0x8
    u16 sex : 1;
    s16 next : 15;

    // at 0xA
    s16 padding2 : 1;
    s16 prev : 15;
} RFLiTableData;

void RFLiClearTableData(RFLiTableData* data);
void RFLiClearDBBuffer(void);
RFLErrcode RFLiFormatAsync(RFLiCallback cb);

#ifdef __cplusplus
}
#endif
#endif