#pragma once

#include "Game/System/BinaryDataChunkHolder.hpp"

class ConfigDataCreateChunk;
class ConfigDataMii;
class ConfigDataMisc;

class ConfigDataHolder {
    friend class UserFile;

public:
    ConfigDataHolder();

    void   setIsCreated(bool);
    bool   isCreated() const;
    void   setLastLoadedMario(bool);
    bool   isLastLoadedMario() const;
    void   onCompleteEndingMario();
    void   onCompleteEndingLuigi();
    bool   isOnCompleteEndingMario();
    bool   isOnCompleteEndingLuigi();
    void   updateLastModified();
    OSTime getLastModified() const;
    void   setMiiOrIconId(const void*, const u32*);
    bool   getMiiId(void*) const;
    bool   getIconId(u32*) const;
    void   resetAllData();
    s32    makeFileBinary(u8*, u32);
    bool   loadFromFileBinary(const char*, const u8*, u32);

private:
    /* 0x00 */ BinaryDataChunkHolder* mChunkHolder;
    /* 0x04 */ ConfigDataCreateChunk* mCreateChunk;
    /* 0x08 */ ConfigDataMii*         mMii;
    /* 0x0C */ ConfigDataMisc*        mMisc;
    /* 0x10 */ char                   mName[16];
};

class ConfigDataCreateChunk : public BinaryDataChunkBase {
    friend class ConfigDataHolder;

public:
    ConfigDataCreateChunk();

    virtual u32  makeHeaderHashCode() const;
    virtual u32  getSignature() const;
    virtual s32  serialize(u8*, u32) const;
    virtual s32  deserialize(const u8*, u32);
    virtual void initializeData();

private:
    /* 0x04 */ bool mIsCreated;
};
