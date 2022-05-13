#pragma once

#include "JSystem/JKernel/JKRFileLoader.h"

class JKRArcFinder;

#define RARC_MAGIC 0x52415243
#define NULL_MAGIC 0
#define QUESTIONMARK_MAGIC 0x3F3F3F3F

#define MAX_NAME_LENGTH 0x100

class JKRArchive : public JKRFileLoader {
public:
    enum EMountDirection {
        MOUNT_DIRECTION_1 = 1
    };

    enum EMountMode {
        MOUNT_MODE_0 = 0,
        MOUNT_MODE_MEM = 1,
        MOUNT_MODE_ARAM = 2,
        MOUNT_MODE_DVD = 3,
        MOUNT_MODE_COMP = 4
    };

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
        u32 mMagic;             // _0
        u32 mFileSize;          // _4
        u32 mHeaderSize;        // _8
        u32 mFileDataOffset;    // _C
        u32 mTotalDataSize;     // _10
        u32 mMRamDataSize;      // _14
        u32 mARamDataSize;      // _18
        u32 _1C;
    };

    struct RarcInfoBlock {
        u32 mNrDirs;                // _0
        u32 mDirOffset;             // _4
        u32 mNrFiles;               // _8
        u32 mFileOffset;            // _C
        u32 mStringTableSize;       // _10
        u32 mStringTableOffset;     // _14
        u16 mNextAvailableFileID;   // _18
        u16 mFileIDIsIndex;         // _1A
        u32 _1C;
    };

    struct SDIFileEntry {
        u16 mFileID;            // _0
        u16 mHash;              // _2
        u32 mFlag : 8;          // _4
        u32 mNameOffset : 24;   // _5
        union {
            u32 mDataOffset;    // _8
            u32 mDirIndex;      // _8
        };
        union {
            u32 mDataSize;      // _C
        };
        void *mFileData;        // _10
    };

    struct SDIDirEntry {
        u32 mID;                // _0
        u32 mNameOffset;        // _4
        u16 mHash;              // _8
        u16 mNrFiles;           // _A
        u32 mFirstFileIndex;    // _C
    };

    struct SDirEntry {
        u8 mFileFlag;   // _0
        u8 _1;
        u16 mFileID;    // _2
        char *mName;    // _4
    };

    class CArcName {
    public:
        inline CArcName() {

        }

        const char *store(const char *);
        const char *store(const char *, char);

        u16 mHash;                          // _0
        u16 mLength;                        // _2
        char mName[MAX_NAME_LENGTH + 1];    // _4
    };

    JKRArchive();
    JKRArchive(long, EMountMode);
    virtual ~JKRArchive();

    virtual bool becomeCurrent(const char *);
    virtual void *getResource(const char *);
    virtual void *getResource(unsigned long, const char *);
    virtual u32 readResource(void *, unsigned long, const char *);
    virtual u32 readResource(void *, unsigned long, unsigned long, const char *);
    virtual void removeResourceAll();
    virtual bool removeResource(void *);
    virtual bool detachResource(void *);
    virtual s32 getResSize(const void *) const;
    virtual s32 countFile(const char *) const;
    JKRArcFinder *getFirstFile(const char *) const;
    virtual s32 getExpandedResSize(const void *) const;
    virtual void *fetchResource(SDIFileEntry *, unsigned long *) = 0;
    virtual void *fetchResource(void *, unsigned long, SDIFileEntry *, unsigned long *) = 0;
    virtual void setExpandSize(SDIFileEntry *, unsigned long);
    virtual u32 getExpandSize(SDIFileEntry *) const;

    static JKRArchive *check_mount_already(long);
    static JKRArchive *check_mount_already(long, JKRHeap *);
    static void mount(const char *, EMountMode, JKRHeap *, EMountDirection);
    static JKRArchive *mount(long, EMountMode, JKRHeap *, EMountDirection);
    bool getDirEntry(SDirEntry *, unsigned long) const;
    void *getIdxResource(unsigned long);
    void *getResource(unsigned short);
    u32 readResource(void *, unsigned long, unsigned short);
    u32 countResource() const;
    u32 getFileAttribute(unsigned long) const;
    bool isSameName(CArcName &, unsigned long, unsigned short) const;
    SDIDirEntry *findResType(unsigned long) const;
    SDIDirEntry *findDirectory(const char *, unsigned long) const;
    SDIFileEntry *findTypeResource(unsigned long, const char *) const;
    SDIFileEntry *findFsResource(const char *, unsigned long) const;
    SDIFileEntry *findIdxResource(unsigned long) const;
    SDIFileEntry *findNameResource(const char *) const;
    SDIFileEntry *findPtrResource(const void *) const;
    SDIFileEntry *findIdResource(unsigned short) const;

    static u32 sCurrentDirIndex; // _806B7148

    JKRHeap *mHeap;             // _38
    u8 mMountMode;              // _3C
    u8 _3D[3];
    s32 mEntryNum;              // _40
    RarcInfoBlock *mInfoBlock;  // _44
    SDIDirEntry *mDirs;         // _48
    SDIFileEntry *mFiles;       // _4C
    u32 *mExpandSizes;          // _50
    char *mStringTable;         // _54
    u32 _58;
    s32 _5C;
    u32 mMountDir;              // _60
};