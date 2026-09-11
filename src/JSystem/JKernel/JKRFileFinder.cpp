#include "JSystem/JKernel/JKRFileFinder.hpp"
#include "JSystem/JKernel/JKRArchive.hpp"

JKRArcFinder::JKRArcFinder(JKRArchive* pArchive, long firstFileIndex, long nrFiles) {
    mArchive = pArchive;
    mHasMoreFiles = nrFiles > 0;
    mFirstIndex = firstFileIndex;
    mLastIndex = firstFileIndex + nrFiles - 1;
    mCurrentIndex = firstFileIndex;

    findNextFile();
}

bool JKRArcFinder::findNextFile() {
    if (mHasMoreFiles) {
        bool moreFiles = mCurrentIndex <= mLastIndex;
        mHasMoreFiles = moreFiles;

        if (moreFiles & 0xFF) {
            JKRArchive::SDirEntry dir;
            mHasMoreFiles = mArchive->getDirEntry(&dir, mCurrentIndex);

            mName = dir.mName;
            mDirIndex = mCurrentIndex;
            mFileID = dir.mFileID;
            mFileFlag = dir.mFileFlag;
            mFileIsFolder = ((mFileFlag >> JKRArchive::FILE_FLAG_FOLDER_SHIFT) & 1) != 0;

            mCurrentIndex++;
        }
    }

    return mHasMoreFiles;
}

JKRFileFinder::JKRFileFinder() {
    mHasMoreFiles = false;
    mFileIsFolder = false;
}

JKRArcFinder::~JKRArcFinder() {
}
