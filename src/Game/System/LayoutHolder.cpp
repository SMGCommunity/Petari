#include "Game/System/LayoutHolder.hpp"

LayoutHolder::LayoutHolder(JKRArchive &rArchive) : nw4r::lyt::ResourceAccessor(), mArchive(&rArchive) {
    initializeArc();
}

LayoutHolder::~LayoutHolder() {

}

/*
void LayoutHolder::initializeArc() {
    u32 res_count = mArchive->countResource();
    res_count -= initEachResTable(&mLayoutRes, arr[0]);
    res_count -= initEachResTable(&mAnimRes, arr[1]);

    if (res_count > 0) {
        mResOther.newFileInfoTable(res_count);
    }

    mount(nullptr);
}
*/