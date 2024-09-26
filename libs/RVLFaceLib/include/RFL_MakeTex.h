#ifndef RFL_MAKETEX_H
#define RFL_MAKETEX_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	RFLResolution_64	= 0x0040,
	RFLResolution_128	= 0x0080,
	RFLResolution_256	= 0x0100,
	RFLResolution_64M	= 0x0060,
	RFLResolution_128M	= 0x00E0,
	RFLResolution_256M	= 0x01E0
} RFLResolution;

#ifdef __cplusplus
}
#endif

#endif // RFL_MAKETEX_H