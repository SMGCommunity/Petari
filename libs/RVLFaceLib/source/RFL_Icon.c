#include <RVLFaceLibInternal.h>

RFLErrcode RFLMakeIcon(void *buf, RFLDataSource source, RFLMiddleDB *middleDB, u16 index, RFLExpression expression, const RFLIconSetting *setting) {
    RFLiCharInfo info;
    RFLErrcode err = RFLiPickupCharInfo(&info, source, middleDB, index);

    if (err == RFLErrcode_Success) {
        RFLiMakeIcon(buf, &info, expression, setting);
    }

    return err;
}

void RFLSetIconDrawDoneCallback(RFLCallback callback) {
    RFLiGetManager()->iconDrawCb = callback;
}