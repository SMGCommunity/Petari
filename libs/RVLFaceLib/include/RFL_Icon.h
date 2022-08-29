#ifndef RFL_ICON_H
#define RFL_ICON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>
#include <revolution/gx.h>
#include "RFL_Types.h"

typedef enum {
	RFLIconBG_Favorite = 0,
	RFLIconBG_Direct
} RFLIconBGType;

typedef struct {
    u16 width;
    u16 height;
    RFLIconBGType bgType;
    GXColor bgColor;
    BOOL drawXluOnly;
} RFLIconSetting;

RFLErrcode RFLMakeIcon(void *, RFLDataSource, RFLMiddleDB *, u16, RFLExpression, const RFLIconSetting *);

#ifdef __cplusplus
}
#endif

#endif // RFL_ICON_H