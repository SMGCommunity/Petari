#include "Game/System/LayoutHolder.hpp"
#include "Game/Util/SystemUtil.hpp"
#include <JSystem/JKernel/JKRFileFinder.hpp>
#include <JSystem/JKernel/JKRArchive.hpp>
#include <cstdio>
#include <cstring>

extern "C" {
int strncasecmp(const char* s1, const char* s2, size_t n);
}

namespace {
    const char* sLayoutExt[] = {
        ".brlyt",
        nullptr,
    };
    const char* sAnimationExt[] = {
        ".brlan",
        nullptr,
    };
};  // namespace

LayoutHolder::LayoutHolder(JKRArchive& rArchive) : nw4r::lyt::ResourceAccessor(), mArchive(&rArchive) {
    initializeArc();
}

LayoutHolder::~LayoutHolder() {
}

void* LayoutHolder::GetResource(u32 resourceKind, const char* pName, u32* pResourceInfo) {
    void* pResource = nullptr;

    switch (resourceKind) {
    case 'blyt':
        pResource = mLayoutRes.getRes(pName);
        break;
    case 'anim':
        pResource = mAnimRes.getRes(pName);
        break;
    default:
        if (strstr(pName, ".brfnt") == 0) {
            pResource = mResOther.getRes(pName);
        }
        break;
    }

    if (pResourceInfo != nullptr) {
        if (pResource != nullptr) {
            *pResourceInfo = static_cast< u32* >(pResource)[1];
        } else {
            *pResourceInfo = 0;
        }
    }

    return pResource;
}

nw4r::ut::Font* LayoutHolder::GetFont(const char* pName) {
    if (strncasecmp(pName, "MessageFont26", strlen("MessageFont26")) == 0) {
        return MR::getFontOnCurrentLanguage();
    }

    if (strncasecmp(pName, "MenuFont64", strlen("MenuFont64")) == 0) {
        return MR::getMenuFontNW4R();
    }

    if (strncasecmp(pName, "NumberFont", strlen("NumberFont")) == 0) {
        return MR::getNumberFontNW4R();
    }

    if (strncasecmp(pName, "PictureFont", strlen("PictureFont")) == 0) {
        return MR::getPictureFontNW4R();
    }

    if (strncasecmp(pName, "CinemaFont26", strlen("CinemaFont26")) == 0) {
        return MR::getCinemaFontNW4R();
    }

    return MR::getFontOnCurrentLanguage();
}

bool LayoutHolder::isAnimationHashEqual(u32 hash, u32 fileID) const {
    return mAnimRes.getFileInfo(fileID)->isEqualHashCode(hash);
}

void LayoutHolder::initializeArc() {
    u32 resCount = mArchive->countResource();
    resCount -= initEachResTable(&mLayoutRes, ::sLayoutExt);
    resCount -= initEachResTable(&mAnimRes, ::sAnimationExt);

    if (resCount > 0) {
        mResOther.newFileInfoTable(resCount);
    }

    mount(nullptr);
}

JKRArcFinder* LayoutHolder::getFileFinder(const char* pRoot) {
    if (pRoot == nullptr) {
        return mArchive->getFirstFile("/");
    }

    return mArchive->getFirstFile(pRoot);
}

u32 LayoutHolder::initEachResTable(ResTable* pResTable, const char* const* pExtensionTable) {
    u32 resCount = 0;

    for (s32 i = 0; pExtensionTable[i] != nullptr; i++) {
        resCount += count(pExtensionTable[i], nullptr);
    }

    if (resCount > 0) {
        pResTable->newFileInfoTable(resCount);
    }

    return resCount;
}

u32 LayoutHolder::count(const char* pExtension, const char* pRoot) {
    u32 resCount = 0;
    JKRArcFinder* pFinder = getFileFinder(pRoot);

    while (pFinder->mHasMoreFiles) {
        if (pFinder->mFileIsFolder) {
            if (pFinder->mName[0] != '.') {
                char path[128];
                sprintf(path, "%s%s%s", pRoot, "/", pFinder->mName);
                resCount += count(pExtension, path);
            }
        } else {
            if (pExtension == nullptr || strstr(pFinder->mName, pExtension) != nullptr) {
                resCount++;
            }
        }
        pFinder->findNextFile();
    }

    delete pFinder;
    return resCount;
}

void LayoutHolder::mount(char* pRoot) {
    JKRFileFinder* pFinder = getFileFinder(pRoot);

    while (pFinder->mHasMoreFiles) {
        if (pFinder->mFileIsFolder) {
            if (pFinder->mName[0] != '.') {
                char path[128];
                snprintf(path, 128, "%s/%s", pRoot, pFinder->mName);
                mount(path);
            }
        } else {
            u32 fileID = mArchive->getFileAttribute(pFinder->mFileID);
            ResFileInfo* pInfo = createAndRegisterObject(pFinder->mName, mArchive->getResource(pFinder->mFileID));
            pInfo->_8 = mArchive->getResource(pFinder->mFileID);
            pInfo->_4 = mArchive->getResSize(pInfo->_8);
            pInfo->_C = pFinder->mFileID;
        }
        pFinder->findNextFile();
    }

    delete pFinder;
}

ResFileInfo* LayoutHolder::createAndRegisterObject(const char* pName, void* pResource) {
    if (strstr(pName, ".brlyt") != nullptr) {
        return mLayoutRes.add(pName, pResource, false);
    }

    if (strstr(pName, ".brlan") != nullptr) {
        return mAnimRes.add(pName, pResource, false);
    }

    return mResOther.add(pName, pResource, false);
}
