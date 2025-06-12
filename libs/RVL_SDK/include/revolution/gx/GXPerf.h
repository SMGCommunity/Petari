#ifndef GXPERF_H
#define GXPERF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>
#include <revolution/gx/GXEnum.h>

void GXSetGPMetric(GXPerf0, GXPerf1);
void GXClearGPMetric(void);
void GXReadXfRasMetric(u32*, u32*, u32*, u32*);

#ifdef __cplusplus
}
#endif

#endif // GXPERF_H
