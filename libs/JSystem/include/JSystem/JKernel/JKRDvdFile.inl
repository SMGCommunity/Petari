template <>
inline JSULink< JKRDvdFile >::~JSULink() {
}

inline s32 JKRDvdFile::getFileSize() const {
    return mFileInfo.length;
}
