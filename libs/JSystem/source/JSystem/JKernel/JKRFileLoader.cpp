#include "JSystem/JKernel/JKRFileLoader.h"
#include "revolution.h"

namespace {
    OSMutex gLoaderMutex; // _8060CFA8
}

JSUList<JKRFileLoader> JKRFileLoader::sFileLoaderList = JSUList<JKRFileLoader>();

JKRFileLoader::JKRFileLoader() : JKRDisposer(), mLoaderLink(this) {
    mLoaderName = nullptr;
    mLoaderType = 0;
    _34 = 0;
}

JKRFileLoader::~JKRFileLoader() {
    if (gCurrentFileLoader == this) {
        gCurrentFileLoader = nullptr;
    }
}

void JKRFileLoader::unmount() {
    if (_34 == 0) {
        return;
    }

    if (--_34 != 0) {
        return;
    }

    delete this;
}

void *JKRFileLoader::getGlbResource(const char *pName, JKRFileLoader *pLoader) {
    void *resource = nullptr;

    if (pLoader != nullptr) {
        resource = pLoader->getResource(0, pName);
    }
    else {
        JSUPtrLink *current = sFileLoaderList.mHead;

        while (current != nullptr) {
            resource = reinterpret_cast<JKRFileLoader *>(current->mData)->getResource(0, pName);

            if (resource != nullptr) {
                break;
            }

            current = current->mNext;
        }
    }

    return resource;
}

void JKRFileLoader::initializeVolumeList() {
    OSInitMutex(&gLoaderMutex);
}

void JKRFileLoader::prependVolumeList(JSULink<JKRFileLoader> *pLoader) {
    OSLockMutex(&gLoaderMutex);

    sFileLoaderList.prepend(pLoader);

    OSUnlockMutex(&gLoaderMutex);
}

void JKRFileLoader::removeVolumeList(JSULink<JKRFileLoader> *pLoader) {
    OSLockMutex(&gLoaderMutex);

    sFileLoaderList.remove(pLoader);

    OSUnlockMutex(&gLoaderMutex);
}
