#include "Game/System/LayoutHolder.hpp"
#include <JSystem/JKernel/JKRArchive.hpp>

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

LayoutHolder::~LayoutHolder() {}

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