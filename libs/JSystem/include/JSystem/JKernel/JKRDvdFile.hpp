#pragma once

#include "JSystem/JKernel/JKRDisposer.hpp"

class JKRADCommand;
class JKRAramBlock;
class JSUFileInputStream;

class JKRFile : public JKRDisposer {
public:
    JKRFile();

    virtual ~JKRFile() {
    }

    /* vt[03] */ virtual bool open(const char*) = 0;
    /* vt[04] */ virtual void close() = 0;
    /* vt[05] */ virtual s32 readData(void*, s32, s32) = 0;
    /* vt[06] */ virtual s32 writeData(const void*, s32, s32) = 0;
    /* vt[07] */ virtual s32 getFileSize() const = 0;

    bool isAvailable() const {
        return mIsAvailable;
    }

    bool mIsAvailable;  // 0x18
};

class JKRDvdFile : public JKRFile {
public:
    JKRDvdFile();
    JKRDvdFile(s32);

    virtual ~JKRDvdFile();
    virtual bool open(const char*);
    virtual void close();
    virtual s32 readData(void*, s32, s32);
    virtual s32 writeData(const void*, s32, s32);
    virtual s32 getFileSize() const;
    virtual bool open(s32);

    s32 sync();
    void initiate();

    u32 getFileID() const {
        return mFileInfo.startAddr;
    }
    DVDFileInfo* getFileInfo() {
        return &mFileInfo;
    }
    int getStatus() const {
        return DVDGetCommandBlockStatus(&mFileInfo.cb);
    }

    /* 0x1C */ OSMutex mMutex1;
    /* 0x34 */ OSMutex mMutex2;
    /* 0x4C */ JKRAramBlock* mBlock;
    /* 0x50 */ OSThread* field_0x50;
    /* 0x54 */ JSUFileInputStream* mFileStream;
    /* 0x58 */ u32 field_0x58;
    /* 0x5C */ DVDFileInfo mFileInfo;
    /* 0x98 */ JKRDvdFile* mDvdFile;
    /* 0x9C */ OSMessageQueue mMessageQueue1;
    /* 0xBC */ OSMessage mMessage1;
    /* 0xC0 */ OSMessageQueue mMessageQueue2;
    /* 0xE0 */ OSMessage mMessage2;
    /* 0xE4 */ JSULink< JKRDvdFile > mDvdLink;
    /* 0xF4 */ OSThread* mOSThread;

    static JSUList< JKRDvdFile > sDvdList;
    static void doneProcess(s32, DVDFileInfo*);
};
