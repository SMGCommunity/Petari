#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JKernel/JKRHeap.h"
#include <ctype.h>
#include <string.h>

JKRArchive::JKRArchive() {
	_30 = false;
	_60 = 1;
}

JKRArchive::JKRArchive(long a1, EMountMode mountMode) {
	_30 = false;
	mMountMode = mountMode;
	_34 = 1;
	_58 = 1;
	mHeap = JKRHeap::findFromRoot(this);

	if (mHeap == NULL) {
		mHeap = JKRHeap::sCurrentHeap;
	}

	_40 = a1;

	if (gCurrentFileLoader == NULL) {
		sCurrentDirIndex = 0;
		gCurrentFileLoader = this;
	}
}

JKRArchive::~JKRArchive() {

}

bool JKRArchive::isSameName(CArcName &rName, unsigned long nameOffset, unsigned short hash) const {
	if (rName.mHash != hash) {
		return false;
	}

	return strcmp(mStringTable + nameOffset, rName.mName) == 0;
}

JKRArchive::SDirEntry *JKRArchive::findResType(unsigned long a1) const {
	SDirEntry *current = mDirs;

	for (u32 i = 0; i < mInfoBlock->mNrDirs; i++) {		
		if (current->mID == a1) {
			return current;
		}

		current++;
	}

	return NULL;
}

JKRArchive::SDirEntry *JKRArchive::findDirectory(const char *pName, unsigned long dirIndex) const {
	if (pName == NULL) {
		return &mDirs[dirIndex];
	}

	CArcName name;
	name.store(pName, '/');

	SDIFileEntry *current = &mFiles[mDirs[dirIndex].mFirstFileIndex];

	for (u32 i = 0; i < mDirs[dirIndex].mNrFiles; i++) {
		//if (isSameName(name, current->mFlag & 0xFFFFFF, current->mHash)) {
		//	if ((current->mTestFlag & 2) != 0) {
		//		return NULL;
		//	}
		//}

		current++;
	}

	return NULL;
}
















#ifdef NON_MATCHING
// tolower() is inlined
void JKRArchive::CArcName::store(const char *pName) {
	mHash = 0;
	u32 length = 0;

	for (; *pName != 0; pName++) {
		char lowerChar = tolower(*pName);

		mHash = lowerChar + mHash * 3;

		if (length < MAX_NAME_LENGTH) {
			mName[length++] = lowerChar;
		}
	}

	mLength = static_cast<u16>(length);
	mName[length] = 0;
}
#endif

#ifdef NON_MATCHING
// tolower() is inlined
void JKRArchive::CArcName::store(const char *pName, char stopChar) {
	mHash = 0;
	u32 length = 0;

	for (; *pName != stopChar; pName++) {
		char lowerChar = tolower(*pName);

		mHash = lowerChar + mHash * 3;

		if (length < MAX_NAME_LENGTH) {
			mName[length++] = lowerChar;
		}
	}

	mLength = static_cast<u16>(length);
	mName[length] = 0;
}
#endif

#ifdef NON_MATCHING
// blt ..., blr is optimized to bgelr
void JKRArchive::setExpandSize(SDIFileEntry *pFile, unsigned long size) {
	u32 fileIndex = static_cast<u32>(pFile - mFiles);

	if (mExpandSizes == NULL) {
		return;
	}

	if (fileIndex >= mInfoBlock->mNrFiles) {
		return;
	}

	mExpandSizes[fileIndex] = size;
}
#endif

u32 JKRArchive::getExpandSize(SDIFileEntry *pFile) const {
	u32 fileIndex = static_cast<u32>(pFile - mFiles);

	if (mExpandSizes == NULL || fileIndex >= mInfoBlock->mNrFiles) {
		return 0;
	}

	return mExpandSizes[fileIndex];
}
