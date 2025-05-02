#include "Game/Camera/CameraParamChunkID.hpp"
#include <cstdio>
#include <cstring>

CameraParamChunkID::CameraParamChunkID() {
    mZoneID = 0;
    mName = nullptr;
}

CameraParamChunkID::CameraParamChunkID(const CameraParamChunkID &other) {
    mZoneID = other.mZoneID;
    u32 length = strlen(other.mName);
    char *buffer = getBuffer(length + 1);
    strcpy(buffer, other.mName);
    mName = buffer;
}

bool CameraParamChunkID::operator>(const CameraParamChunkID &other) const {
    if (mName == nullptr) {
        return other.mName != nullptr;
    }
    else if (other.mName == nullptr) {
        return false;
    }

    bool result = true;

    if (mZoneID <= other.mZoneID) {
        bool stringEqual = false;

        if (mZoneID == other.mZoneID && strcmp(mName, other.mName) == 0) {
            stringEqual = true;
        }
        
        if (!stringEqual) {
            result = false;
        }
    }

    return result;
}

bool CameraParamChunkID::operator==(const CameraParamChunkID &other) const {
    if (mName == nullptr) {
        return other.mName == nullptr;
    }
    else if (other.mName == nullptr) {
        return false;
    }

    if (mZoneID == other.mZoneID) {
        strcmp(mName, other.mName);
    }

    return mZoneID == other.mZoneID && strcmp(mName, other.mName) == 0;
}


char *CameraParamChunkID::getBuffer(u32 length) {
    return new char[length];
}

bool CameraParamChunkID::equals(s32 zoneID, const char *pName) const {
    if (mName != nullptr) {
        return zoneID == mZoneID && strcmp(mName, pName) == 0;
    }
    
    return false;
}

void CameraParamChunkID::createCubeID(s32 zoneID, u16 id) {
    mZoneID = static_cast<s8>(zoneID);

    const u32 formatSize = 7;
    char *buffer = getBuffer(formatSize);
    snprintf(buffer, formatSize, "c:%04x", id);

    mName = buffer;
}

void CameraParamChunkID::createGroupID(s32 zoneID, const char *a2, u32 a3, u32 a4) {
    mZoneID = static_cast<s8>(zoneID);

    char buffer[0x100];
    snprintf(buffer, sizeof(buffer), "g:%s:%d:%d", a2, a3, a4);

    u32 actualSize = strlen(&buffer[0]);
    char *buffer2 = getBuffer(actualSize + 1);

    strcpy(buffer2, &buffer[0]);
    mName = buffer2;
}

void CameraParamChunkID::createOtherID(s32 zoneID, const char *a2) {
    mZoneID = static_cast<s8>(zoneID);

    char buffer[0x100];
    snprintf(buffer, sizeof(buffer), "o:%s", a2);

    u32 actualSize = strlen(&buffer[0]);
    char *buffer2 = getBuffer(actualSize + 1);

    strcpy(buffer2, &buffer[0]);
    mName = buffer2;
}

void CameraParamChunkID::createEventID(s32 zoneID, const char *a2) {
    mZoneID = static_cast<s8>(zoneID);

    char buffer[0x100];
    snprintf(buffer, sizeof(buffer), "e:%s", a2);

    u32 actualSize = strlen(&buffer[0]);
    char *buffer2 = getBuffer(actualSize + 1);

    strcpy(buffer2, &buffer[0]);
    mName = buffer2;
}

void CameraParamChunkID::createStartID(s32 zoneID, u16 id) {
    mZoneID = static_cast<s8>(zoneID);

    char buffer[0x100];
    snprintf(buffer, sizeof(buffer), "s:%04x", id);

    u32 actualSize = strlen(&buffer[0]);
    char *buffer2 = getBuffer(actualSize + 1);

    strcpy(buffer2, &buffer[0]);
    mName = buffer2;
}