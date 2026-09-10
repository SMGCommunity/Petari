#include <revolution/nwc24.h>
#include <revolution/nwc24/NWC24Internal.h>

static const char* MIMEEncStr = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

#define ALPHABET_LEN 64

#define MAX_OCTETS 4
#define DEFAULT_LINE_LEN 75

static NWC24Err Base64Encode(char* pData, s32 dataSize, u32* pDataRead, char* pText, s32 textSize, u32* pTextWritten) {
    NWC24Err result = NWC24_OK;
    s32 textIdx;
    s32 dataIdx = 0;
    BOOL hasThird;
    BOOL hasSecond;
    u32 combined;

    for (textIdx = 0; dataIdx < dataSize; dataIdx += 3) {
        if (textIdx + 3 >= textSize) {
            result = NWC24_ERR_OVERFLOW;
            break;
        }

        hasThird = FALSE;
        hasSecond = FALSE;

        combined = (u8)pData[dataIdx];
        combined <<= 8;

        if (dataIdx + 1 < dataSize) {
            hasSecond = TRUE;
            combined |= (u8)pData[dataIdx + 1];
        }

        combined <<= 8;

        if (dataIdx + 2 < dataSize) {
            hasThird = TRUE;
            combined |= (u8)pData[dataIdx + 2];
        }

        pText[textIdx + 0] = MIMEEncStr[combined >> 18 & (ALPHABET_LEN - 1)];
        pText[textIdx + 1] = MIMEEncStr[combined >> 12 & (ALPHABET_LEN - 1)];
        pText[textIdx + 2] = MIMEEncStr[hasSecond ? (combined >> 6 & (ALPHABET_LEN - 1)) : ALPHABET_LEN];
        pText[textIdx + 3] = MIMEEncStr[hasThird ? (combined & (ALPHABET_LEN - 1)) : ALPHABET_LEN];

        textIdx += 4;
    }

    *pTextWritten = textIdx;

    if (pDataRead != NULL) {
        textIdx = dataSize;

        if (dataSize < dataIdx) {
            dataIdx = dataSize;
        }

        *pDataRead = dataIdx;
    }

    return result;
}

NWC24Err NWC24Base64Encode(const u8* pData, s32 dataSize, char* pText, s32 textSize, u32* pTextWritten) {
    return Base64Encode((char*)pData, dataSize, NULL, pText, textSize, pTextWritten);
}

void NWC24InitBase64Table(char* pTable) {
    int i;
    for (i = 0; i < NWC24_BASE64_TABLE_SIZE; i++) {
        pTable[i] = 0xFF;
    }

    for (i = 'A'; i <= 'Z'; i++) {
        pTable[i] = 0 + i - 'A';
    }

    for (i = 'a'; i <= 'z'; i++) {
        pTable[i] = 26 + i - 'a';
    }

    for (i = '0'; i <= '9'; i++) {
        pTable[i] = 26 + 26 + i - '0';
    }

    pTable['+'] = 26 + 26 + 10;
    pTable['/'] = 26 + 26 + 10 + 1;
}

static NWC24Err QEncode(u8* pText, u32 textSize, u32* pTextWritten, u8* pData, u32 dataSize, u32* pDataRead, s32 maxLineLength) {
    u8* pWritePtr;
    u32 textWritten;
    u32 dataIdx;
    NWC24Err result;
    s32 lineLength;
    u8 octets[MAX_OCTETS];
    BOOL lineBreak;
    s32 emitCount;
    s32 bytesRead;
    s16 currentByte;
    s32 i;

    result = NWC24_OK;

    if (pText == NULL) {
        return NWC24_ERR_INVALID_VALUE;
    }

    if (pData == NULL) {
        return NWC24_ERR_INVALID_VALUE;
    }

    if (dataSize <= 0) {
        return NWC24_ERR_OVERFLOW;
    }

    if (pDataRead != NULL) {
        *pDataRead = 0;
    }

    if (pTextWritten != NULL) {
        *pTextWritten = 0;
    }

    pWritePtr = pText;
    lineLength = 0;
    textWritten = 0;
    dataIdx = 0;

    while (dataIdx < dataSize) {
        currentByte = *pData;
        lineBreak = FALSE;

        if ((char)currentByte >= '!' && (char)currentByte <= '~' && (char)currentByte != '=') {
            bytesRead = 1;
            emitCount = 1;
            octets[0] = *pData;

        } else if (dataIdx + 1 < dataSize && (char)currentByte == '\r' && (char)pData[1] == '\n') {
            bytesRead = 2;
            emitCount = 1;
            octets[0] = *pData;
            octets[1] = pData[1];

        } else {
            octets[0] = '=';

            if ((*pData >> 4) >= 10) {
                octets[1] = (*pData >> 4) % 10 + 'A';
            } else {
                octets[1] = (*pData >> 4) % 10 + '0';
            }

            if ((*pData & 0x0F) >= 10) {
                octets[2] = (*pData & 0x0F) % 10 + 'A';
            } else {
                octets[2] = (*pData & 0x0F) % 10 + '0';
            }

            bytesRead = 1;
            emitCount = 3;
        }

        if (maxLineLength == 0 && lineLength + emitCount >= DEFAULT_LINE_LEN) {
            lineBreak = TRUE;
            emitCount += 3;
        }

        if (textWritten + emitCount >= textSize) {
            result = NWC24_ERR_OVERFLOW;
            break;
        }

        textWritten += emitCount;

        if (lineBreak) {
            pWritePtr[0] = '=';
            pWritePtr[1] = '\r';
            pWritePtr[2] = '\n';

            pWritePtr += 3;
            emitCount -= 3;

            lineLength = 0;
        }

        lineLength += emitCount;

        for (i = 0; i < emitCount; i++) {
            *(pWritePtr++) = octets[i];
        }

        dataIdx += bytesRead;
        pData += bytesRead;
    }

    if (pDataRead != NULL) {
        *pDataRead = dataIdx;
    }

    if (pTextWritten != NULL) {
        *pTextWritten = textWritten;
    }

    return result;
}

NWC24Err NWC24EncodeQuotedPrintable(char* pText, u32 textSize, u32* pTextWritten, const u8* pData, u32 dataSize, u32* pDataRead) {
    return QEncode((u8*)pText, textSize, pTextWritten, (u8*)pData, dataSize, pDataRead, 0);
}
