#pragma once

class NameObj;
class NameObjArchiveListCollector;
class JMapInfoIter;

typedef NameObj* (*CreatorFuncPtr)(const char*);
typedef void (*ArchiveFuncPtr)(NameObjArchiveListCollector*, const JMapInfoIter&);

namespace {
    template < typename T >
    NameObj* createNameObj(const char* pName) {
        return new T(pName);
    }
};  // namespace

/// @brief A namespace for creating `NameObj` instances.
namespace NameObjFactory {
    struct Name2CreateFunc {
        /* 0x0 */ const char* mName;
        /* 0x4 */ CreatorFuncPtr mCreateFunc;
        /* 0x8 */ const char* mArchiveName;
    };

    struct Name2Archive {
        /* 0x0* */ const char* mObjectName;
        /* 0x4* */ const char* mArchiveName;
    };

    struct Name2MakeArchiveListFunc {
        /* 0x0 */ const char* mName;
        /* 0x4 */ ArchiveFuncPtr mArchiveFunc;
    };

    CreatorFuncPtr getCreator(const char*);
    void requestMountObjectArchives(const char*, const JMapInfoIter&);
    bool isReadResourceFromDVD(const char*, const JMapInfoIter&);
    bool isPlayerArchiveLoaderObj(const char*);
    const Name2CreateFunc* getName2CreateFunc(const char*, const Name2CreateFunc*);
    void getMountObjectArchiveList(NameObjArchiveListCollector*, const char*, const JMapInfoIter&);
};  // namespace NameObjFactory
