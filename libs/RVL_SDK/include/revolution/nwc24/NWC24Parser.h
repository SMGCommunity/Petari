#ifndef RVL_SDK_NWC24_PARSER_H
#define RVL_SDK_NWC24_PARSER_H
#include <revolution/types.h>
#include <cstdlib>

#include <revolution/nwc24/NWC24Types.h>

#ifdef __cplusplus
extern "C" {
#endif

const char* NWC24GetMIMETypeStr(NWC24MIMEType type);
const char* NWC24GetCharsetStr(NWC24Charset charset);
const char* NWC24GetEncodingStr(NWC24Encoding encoding);

#ifdef __cplusplus
}
#endif
#endif
