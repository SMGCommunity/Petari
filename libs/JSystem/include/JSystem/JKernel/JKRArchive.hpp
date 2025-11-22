#pragma once

#include "JSystem/JKernel/JKRFileLoader.hpp"

class JKRArcFinder;

#define RARC_MAGIC 0x52415243
#define NULL_MAGIC 0
#define QUESTIONMARK_MAGIC 0x3F3F3F3F

#define MAX_NAME_LENGTH 0x100

class JKRArchive : public JKRFileLoader {
public:
    enum EMountDirection { MOUNT_DIRECTION_1 = 1 };

    enum EMountMode { MOUNT_MODE_0 = 0, MOUNT_MODE_MEM = 1, MOUNT_MODE_ARAM = 2, MOUNT_MODE_DVD = 3, MOUNT_MODE_COMP = 4 };

    enum EFileFlag {
        FILE_FLAG_FILE_SHIFT = 0,
        FILE_FLAG_FOLDER_SHIFT = 1,
        FILE_FLAG_COMPRESSED_SHIFT = 2,
        FILE_FLAG_MRAM_SHIFT = 4,
        FILE_FLAG_ARAM_SHIFT = 5,
        FILE_FLAG_DVD_SHIFT = 6,
        FILE_FLAG_IS_YAZ0_SHIFT = 7,

        FILE_FLAG_FILE = 1 << FILE_FLAG_FILE_SHIFT,
        FILE_FLAG_FOLDER = 1 << FILE_FLAG_FOLDER_SHIFT,
        FILE_FLAG_COMPRESSED = 1 << FILE_FLAG_COMPRESSED_SHIFT,
        FILE_FLAG_MRAM = 1 << FILE_FLAG_MRAM_SHIFT,
        FILE_FLAG_ARAM = 1 << FILE_FLAG_ARAM_SHIFT,
        FILE_FLAG_DVD = 1 << FILE_FLAG_DVD_SHIFT,
        FILE_FLAG_IS_YAZ0 = 1 << FILE_FLAG_IS_YAZ0_SHIFT
    };

    struct RarcHeader {
        u32 mMagic;           // 0x0
        u32 mFileSize;        // 0x4
        u32 mHeaderSize;      // 0x8
        u32 mFileDataOffset;  // 0xC
        u32 mTotalDataSize;   // 0x10
        u32 mMRamDataSize;    // 0x14
        u32 mARamDataSize;    // 0x18
        u32 _1C;
    };

    struct RarcInfoBlock {
        u32 mNrDirs;               // 0x0
        u32 mDirOffset;            // 0x4
        u32 mNrFiles;              // 0x8
        u32 mFileOffset;           // 0xC
        u32 mStringTableSize;      // 0x10
        u32 mStringTableOffset;    // 0x14
        u16 mNextAvailableFileID;  // 0x18
        u16 mFileIDIsIndex;        // 0x1A
        u32 _1C;
    };

    struct SDIFileEntry {
        u16 mFileID;           // 0x0
        u16 mHash;             // 0x2
        u32 mFlag : 8;         // 0x4
        u32 mNameOffset : 24;  // 0x5
        union {
            u32 mDataOffset;  // 0x8
            u32 mDirIndex;    // 0x8
        };
        union {
            u32 mDataSize;  // 0xC
        };
        void* mFileData;  // 0x10
    };

    struct SDIDirEntry {
        u32 mID;              // 0x0
        u32 mNameOffset;      // 0x4
        u16 mHash;            // 0x8
        u16 mNrFiles;         // 0xA
        u32 mFirstFileIndex;  // 0xC
    };

    struct SDirEntry {
        u8 mFileFlag;  // 0x0
        u8 _1;
        u16 mFileID;  // 0x2
        char* mName;  // 0x4
    };

    class CArcName {
    public:
        inline CArcName() {}

        const char* store(const char*);
        const char* store(const char*, char);

        u16 mHash;                        // 0x0
        u16 mLength;                      // 0x2
        char mName[MAX_NAME_LENGTH + 1];  // 0x4
    };

    JKRArchive();
    JKRArchive(long, EMountMode);
    virtual ~JKRArchive();

    virtual bool becomeCurrent(const char*);
    virtual void* getResource(const char*);
    virtual void* getResource(unsigned long, const char*);
    virtual u32 readResource(void*, unsigned long, const char*);
    virtual u32 readResource(void*, unsigned long, unsigned long, const char*);
    virtual void removeResourceAll();
    virtual bool removeResource(void*);
    virtual bool detachResource(void*);
    virtual s32 getResSize(const void*) const;
    virtual s32 countFile(const char*) const;
    JKRArcFinder* getFirstFile(const char*) const;
    virtual s32 getExpandedResSize(const void*) const;
    virtual void* fetchResource(SDIFileEntry*, unsigned long*) = 0;
    virtual void* fetchResource(void*, unsigned long, SDIFileEntry*, unsigned long*) = 0;
    virtual void setExpandSize(SDIFileEntry*, unsigned long);
    virtual u32 getExpandSize(SDIFileEntry*) const;

    static JKRArchive* check_mount_already(long);
    static JKRArchive* check_mount_already(long, JKRHeap*);
    static void mount(const char*, EMountMode, JKRHeap*, EMountDirection);
    static JKRArchive* mount(long, EMountMode, JKRHeap*, EMountDirection);
    bool getDirEntry(SDirEntry*, unsigned long) const;
    void* getIdxResource(unsigned long);
    void* getResource(unsigned short);
    u32 readResource(void*, unsigned long, unsigned short);
    u32 countResource() const;
    u32 getFileAttribute(unsigned long) const;
    bool isSameName(CArcName&, unsigned long, unsigned short) const;
    SDIDirEntry* findResType(unsigned long) const;
    SDIDirEntry* findDirectory(const char*, unsigned long) const;
    SDIFileEntry* findTypeResource(unsigned long, const char*) const;
    SDIFileEntry* findFsResource(const char*, unsigned long) const;
    SDIFileEntry* findIdxResource(unsigned long) const;
    SDIFileEntry* findNameResource(const char*) const;
    SDIFileEntry* findPtrResource(const void*) const;
    SDIFileEntry* findIdResource(unsigned short) const;

    static u32 sCurrentDirIndex;  // 0x806B7148

    JKRHeap* mHeap;  // 0x38
    u8 mMountMode;   // 0x3C
    u8 _3D[3];
    s32 mEntryNum;              // 0x40
    RarcInfoBlock* mInfoBlock;  // 0x44
    SDIDirEntry* mDirs;         // 0x48
    SDIFileEntry* mFiles;       // 0x4C
    u32* mExpandSizes;          // 0x50
    char* mStringTable;         // 0x54
    u32 _58;
    s32 _5C;
    u32 mMountDir;  // 0x60
};