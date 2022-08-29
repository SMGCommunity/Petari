#ifndef RFL_DATAUTILITY_H
#define RFL_DATAUTILITY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "RFL_Types.h"

RFLErrcode RFLGetAdditionalInfo(RFLAdditionalInfo *, RFLDataSource, const RFLMiddleDB *, u16);

#ifdef __cplusplus
}
#endif

#endif // RFL_DATAUTILITY_H