#include "revolution/nwc24/NWC24Parser.h"

static NWC24MIMEType MIMETypeConv[10] = {
    NWC24_APPLICATION_OCTET_STREAM, NWC24_X_WII_MINIDATA,  NWC24_X_WII_MSGBOARD,    NWC24_IMAGE_JPEG, NWC24_X_WII_PICTURE,
    NWC24_MULTIPART_ALTERNATIVE,    NWC24_MULTIPART_MIXED, NWC24_MULTIPART_RELATED, NWC24_TEXT_HTML,  NWC24_TEXT_PLAIN};

static char* MIMETypeStr[10] = {"application/octet-stream", "x-wii-minidata",  "x-wii-msgboard",    "image/jpeg", "x-wii-picture",
                                "multipart/alternative",    "multipart/mixed", "multipart/related", "text/html",  "text/plain"};

static char* MIMETypeSuffix[10] = {"dat", "wii", "wii", "jpg", "???", "???", "???", "related", "htm", "txt"};

static char* CharsetStr[16] = {"euc-jp",     "iso-2022-jp", "iso-8859-1", "iso-8859-10", "iso-8859-15", "iso-8859-2", "iso-8859-3", "iso-8859-5",
                               "iso-8859-7", "iso-8859-9",  "shift_jis",  "us-ascii",    "utf-16be",    "utf-32be",   "utf-8",      "windows-1252"};

static NWC24Charset CharsetConv[16];

static char* EncodingStr[4] = {"7bit", "8bit", "base64", "quoted-printable"};

static NWC24Encoding EncodingConv[4];

char* NWC24GetMIMETypeStr(NWC24MIMEType mimeType) {
    u32 i;

    for (i = 0; i < 0xA; i++) {
        if (MIMETypeConv[i] == mimeType) {
            return MIMETypeStr[i];
        }
    }

    return NULL;
}

char* NWC24iGetMIMETypeSuffix(NWC24MIMEType mimeType) {
    u32 i;

    for (i = 0; i < 0xA; i++) {
        if (MIMETypeConv[i] == mimeType) {
            return MIMETypeSuffix[i];
        }
    }

    return NULL;
}

char* NWC24GetCharsetStr(NWC24Charset charset) {
    u32 i;

    for (i = 0; i < 0xA; i++) {
        if (CharsetConv[i] == charset) {
            return CharsetStr[i];
        }
    }

    return NULL;
}

char* NWC24GetEncodingStr(NWC24Encoding encoding) {
    u32 i;

    for (i = 0; i < 4; i++) {
        if (EncodingConv[i] == encoding) {
            return EncodingStr[i];
        }
    }

    return NULL;
}
