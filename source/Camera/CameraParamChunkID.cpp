#include "Camera/CameraParamChunkID.h"
#include "smg.h"

#include<stdio.h>
#include<cstring>

using namespace std;

CameraParamChunkID::CameraParamChunkID()
{
    mChunkLength = 0;
    mBuffer = nullptr;
}

CameraParamChunkID::CameraParamChunkID(const CameraParamChunkID &otherChunk)
{
    mChunkLength = otherChunk.mChunkLength;
    char* buf = getBuffer(strlen(otherChunk.mBuffer) + 1);
    strcpy(buf, otherChunk.mBuffer);
    mBuffer = buf;
}

bool CameraParamChunkID::operator>(const CameraParamChunkID &otherChunk) const
{
    bool ret;

    if (mBuffer == nullptr)
    {
        return otherChunk.mBuffer;
    }
    else
    {
        if (otherChunk.mBuffer == nullptr)
        {
            return false;
        }
        else
        {
            ret = true;

            if ((s32)mChunkLength <= (s32)otherChunk.mChunkLength)
            {
                bool temp = false;

                if (mChunkLength == otherChunk.mChunkLength && strcmp(mBuffer, otherChunk.mBuffer) > 0)
                {
                    temp = true;
                }

                if (!temp)
                {
                    ret = false;
                }
            }
        }
    }

    return ret;
}

bool CameraParamChunkID::operator==(const CameraParamChunkID &otherChunk) const
{
    bool ret;

    if (mBuffer == nullptr)
    {
        return !otherChunk.mBuffer;
    }
    else
    {
        if (otherChunk.mBuffer == nullptr)
        {
            return false;
        }
        else
        {
            if ((s32)otherChunk.mChunkLength == (s32)mChunkLength)
            {
                strcmp(mBuffer, otherChunk.mBuffer);
            }

            ret = false;

            if ((s32)otherChunk.mChunkLength == (s32)mChunkLength && strcmp(mBuffer, otherChunk.mBuffer))
            {
                ret = true;
            }
        }
    }

    return ret;
}

bool CameraParamChunkID::equals(s32 length, const char *pBuffer) const
{
    bool ret;

    if (mBuffer == nullptr)
    {
        ret = false;
    }
    else
    {
        ret = false;

        if (length == (s32)mChunkLength && strcmp(mBuffer, pBuffer) == 0)
        {
            ret = true;
        }
    }    

    return ret;
}

char* CameraParamChunkID::getBuffer(u32 size)
{
    return new char[size];
}

void CameraParamChunkID::createCubeID(s32 length, u16 id)
{
    mChunkLength = length;
    char* buf = getBuffer(length);
    snprintf(buf, 7, "c:%04x", id);
    mBuffer = buf;
}

void CameraParamChunkID::createGroupID(s32 length, const char *pGroupName, u32 unk1, u32 unk2)
{
    mChunkLength = length;
    char buf[0x100];
    snprintf(buf, 0x100, "g:%s:%d:%d", pGroupName, unk1, unk2);
    char* newBuf = getBuffer(strlen(buf) + 1);
    strcpy(newBuf, buf);
    mBuffer = newBuf;
}

void CameraParamChunkID::createOtherID(s32 length, const char *pName)
{
    mChunkLength = length;
    char buf[0x100];
    snprintf(buf, 0x100, "o:%s", pName);
    char* newBuf = getBuffer(strlen(buf) + 1);
    strcpy(newBuf, buf);
    mBuffer = newBuf;
}

void CameraParamChunkID::createEventID(s32 length, const char *pName)
{
    mChunkLength = length;
    char buf[0x100];
    snprintf(buf, 0x100, "e:%s", pName);
    char* newBuf = getBuffer(strlen(buf) + 1);
    strcpy(newBuf, buf);
    mBuffer = newBuf;
}

void CameraParamChunkID::createStartID(s32 length, u16 id)
{
    mChunkLength = length;
    char buf[0x100];
    snprintf(buf, 0x100, "s:%04x", id);
    char* newBuf = getBuffer(strlen(buf) + 1);
    strcpy(newBuf, buf);
    mBuffer = newBuf;
}