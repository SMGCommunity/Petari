#pragma once

#include <nw4r/lyt/drawInfo.h>

namespace nw4r {
    namespace lyt {
        class ArcResourceAccessor;
        class Layout;
    };  // namespace lyt
};  // namespace nw4r

struct MEMAllocator;

class IsbnManager {
public:
    /* 0x00 */ bool _0;
    /* 0x04 */ MEMAllocator* mpAllocator;
    /* 0x08 */ nw4r::lyt::Layout* mpLayout;
    /* 0x0C */ nw4r::lyt::ArcResourceAccessor* mpResAccessor;
    /* 0x10 */ nw4r::lyt::DrawInfo mDrawInfo;
    /* 0x64 */ u16 _64;
    /* 0x66 */ u16 _66;
    /* 0x68 */ u16 _68;
    /* 0x6C */ void* mpIsbnBuffer;
    /* 0x70 */ void* mpLayoutBuffer;
    /* 0x74 */ void* mpResAccBuffer;

    static const char* cIsbnNumberPaneName[];
    static const char* cRegistNumberPaneName[];
    static const char* cOtherNumberPaneName[];

public:
    /// @brief Creates a new `IsbnManager`.
    /// @param pAllocator A pointer to the memory allocator.
    IsbnManager(MEMAllocator* pAllocator);

    /// @brief Destroys the `IsbnManager`.
    virtual ~IsbnManager();

    void setAdjustRate(f32, f32);
    void setNumber(const wchar_t* pIsbnNumber, const wchar_t* pRegistNumber, const wchar_t* pOtherNumber);
    void calculateView();
    static IsbnManager* create(void* pArchiveBuf, MEMAllocator* pAllocator);
    bool calc(bool);
    void draw();
    void reset();
};
