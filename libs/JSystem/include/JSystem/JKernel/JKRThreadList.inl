template <>
inline JSUList< JKRThread >::JSUList(bool initialize) : JSUPtrList(initialize) {
}

template <>
inline JSUList< JKRThread >::~JSUList() {
}

template <>
inline JSUList< JKRTask >::JSUList() : JSUPtrList() {
}

template <>
inline JSUList< JKRTask >::~JSUList() {
}
