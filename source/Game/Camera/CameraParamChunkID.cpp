#include "Game/Camera/CameraParamChunkID.h"
#include <string.h>
#include <cstdio>

CameraParamChunkID::CameraParamChunkID() {
    _4 = 0;
    mName = NULL;
}

CameraParamChunkID::CameraParamChunkID(const CameraParamChunkID &other) {
    _4 = other._4;
    u32 length = strlen(other.mName);
    char *buffer = getBuffer(length + 1);
    strcpy(buffer, other.mName);
    mName = buffer;
}

bool CameraParamChunkID::operator>(const CameraParamChunkID &other) const {
    if (mName == NULL) {
        return other.mName != NULL;
    }
    else if (other.mName == NULL) {
        return false;
    }

    bool result = true;

    if (_4 <= other._4) {
        bool stringEqual = false;

        if (_4 == other._4 && strcmp(mName, other.mName) == 0) {
            stringEqual = true;
        }
        
        if (!stringEqual) {
            result = false;
        }
    }

    return result;
}

bool CameraParamChunkID::operator==(const CameraParamChunkID &other) const {
    if (mName == NULL) {
        return other.mName == NULL;
    }
    else if (other.mName == NULL) {
        return false;
    }

    if (_4 == other._4) {
        strcmp(mName, other.mName);
    }

    return _4 == other._4 && strcmp(mName, other.mName) == 0;
}


char *CameraParamChunkID::getBuffer(unsigned long length) {
    return new char[length];
}

bool CameraParamChunkID::equals(long a1, const char *pName) const {
    if (mName != NULL) {
        return a1 == _4 && strcmp(mName, pName) == 0;
    }
    
    return false;
}

void CameraParamChunkID::createCubeID(long a1, unsigned short id) {
    _4 = static_cast<s8>(a1);

    const u32 formatSize = 7;
    char *buffer = getBuffer(formatSize);
    snprintf(buffer, formatSize, "c:%04x", id);

    mName = buffer;
}

void CameraParamChunkID::createGroupID(long a1, const char *a2, unsigned long a3, unsigned long a4) {
    _4 = static_cast<s8>(a1);

    char buffer[0x100];
    snprintf(buffer, sizeof(buffer), "g:%s:%d:%d", a2, a3, a4);

    u32 actualSize = strlen(&buffer[0]);
    char *buffer2 = getBuffer(actualSize + 1);

    strcpy(buffer2, &buffer[0]);
    mName = buffer2;
}

void CameraParamChunkID::createOtherID(long a1, const char *a2) {
    _4 = static_cast<s8>(a1);

    char buffer[0x100];
    snprintf(buffer, sizeof(buffer), "o:%s", a2);

    u32 actualSize = strlen(&buffer[0]);
    char *buffer2 = getBuffer(actualSize + 1);

    strcpy(buffer2, &buffer[0]);
    mName = buffer2;
}

void CameraParamChunkID::createEventID(long a1, const char *a2) {
    _4 = static_cast<s8>(a1);

    char buffer[0x100];
    snprintf(buffer, sizeof(buffer), "e:%s", a2);

    u32 actualSize = strlen(&buffer[0]);
    char *buffer2 = getBuffer(actualSize + 1);

    strcpy(buffer2, &buffer[0]);
    mName = buffer2;
}

void CameraParamChunkID::createStartID(long a1, unsigned short id) {
    _4 = static_cast<s8>(a1);

    char buffer[0x100];
    snprintf(buffer, sizeof(buffer), "s:%04x", id);

    u32 actualSize = strlen(&buffer[0]);
    char *buffer2 = getBuffer(actualSize + 1);

    strcpy(buffer2, &buffer[0]);
    mName = buffer2;
}