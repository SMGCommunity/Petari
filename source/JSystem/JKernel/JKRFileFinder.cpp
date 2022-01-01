#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JKernel/JKRFileFinder.h"

JKRFileFinder::JKRFileFinder() {
    mHasMoreFiles = false;
    mFileIsFolder = false;
}

JKRArcFinder::JKRArcFinder(JKRArchive *pArchive, long firstFileIndex, long nrFiles) {
    mArchive = pArchive;
    mHasMoreFiles = nrFiles > 0;
    mFirstIndex = firstFileIndex;
    mLastIndex = firstFileIndex + nrFiles - 1;
    mCurrentIndex = firstFileIndex;

    findNextFile();
}

#ifdef NON_MATCHING
// Looks identical to base destructor, does not call ~JKRFileFinder()
JKRArcFinder::~JKRArcFinder() {

}
#endif

bool JKRArcFinder::findNextFile() {
    if (mHasMoreFiles) {
        bool moreFiles = mCurrentIndex <= mLastIndex;
        mHasMoreFiles = moreFiles;

        // Weird code
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
