#ifndef NWC24PARSER_H
#define NWC24PARSER_H

#include "revolution/nwc24.h"

#ifdef __cplusplus
extern "C" {
#endif

const char* NWC24GetMIMETypeStr(NWC24MIMEType type);
const char* NWC24iGetMIMETypeSuffix(NWC24MIMEType type);
const char* NWC24GetCharsetStr(NWC24Charset set);
const char* NWC24GetEncodingStr(NWC24Encoding enc);

#ifdef __cplusplus
}
#endif

#endif  // NWC24PARSER_H
