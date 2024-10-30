#include <RVLFaceLibInternal.h>
#include <mem.h>

#define MAGIC_OFFICIAL_DB 'RNOD'
#define MAGIC_HIDDEN_DB 'RNHD'

#define TABLE_DATA_STEP 1250
// TODO: How is this size calculated?
#define TEMP_BUFFER_SIZE (TABLE_DATA_STEP * sizeof(RFLiHiddenCharData) + 0x30E0)

static void RFLiClearTableData(RFLiTableData* data) {
    memset(data, 0, sizeof(RFLiTableData));
    data->next = -1;
    data->prev = -1;
}

void RFLiClearDBBuffer(void) {
    RFLiDatabase* db;
    RFLiHiddenDB* header;
    int i;

    db = RFLiGetDBManager()->database;
    header = &db->hidden;

    // @bug Four-byte buffer overrun
    memset((u8*)db + sizeof(db->identifier), 0, sizeof(RFLiDatabase));
    db->identifier = MAGIC_OFFICIAL_DB;
    db->isolation = TRUE;

    header->identifier = MAGIC_HIDDEN_DB;
    header->head = -1;
    header->tail = -1;

    for (i = 0; i < RFLi_HDB_DATA_MAX; i++) {
        RFLiClearTableData(&header->data[i]);
    }
}
