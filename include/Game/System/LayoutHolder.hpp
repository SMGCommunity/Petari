#pragma once

#include "Game/System/ResourceInfo.hpp"
#include <nw4r/lyt/resourceAccessor.h>

class JKRArchive;

class LayoutHolder : public nw4r::lyt::ResourceAccessor {
public:
    LayoutHolder(JKRArchive&);

    virtual ~LayoutHolder();
    virtual void* GetResource(u32, const char*, u32*);
    virtual nw4r::ut::Font* GetFont(const char*);
    virtual void* getResOther(const char*) const;
    virtual u32 getResOtherNum() const;
    virtual const char* getResOtherName(u32) const;
    virtual void* getResOther(u32) const;
    virtual bool isExistResOther(const char*) const;

    void initializeArc();

    u32 initEachResTable(ResTable*, const char* const*);

    void mount(char*);

    JKRArchive* mArchive;  // 0x4
    ResTable mLayoutRes;   // 0x8
    ResTable mAnimRes;     // 0x10
    ResTable mResOther;    // 0x18
};
