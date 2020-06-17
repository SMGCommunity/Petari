#ifndef JMAPINFO_H
#define JMAPINFO_H

#include <revolution.h>

struct JMapData
{
    s32 _0;
    s32 mNumData; // _4
    s32 mDataOffset; // _8
    u32 _C;

    struct JMapItem
    {
        u32 mHash; // _0
        u32 mMask; // _4
        u16 mOffsetData; // _8
        u8 mShift; // _A
        u8 mType; // _B
    };

    JMapItem mItems[]; // _10
};

// credit to riidefi for this one
struct JMapDataPtr
{
	inline JMapDataPtr(const void* ptr)
		: _wrapped(static_cast<const JMapData*>(ptr))
	{}

	inline bool valid() const
	{
		return (volatile bool)(volatile long long int)_wrapped;
	}

	const JMapData* _wrapped;
};

class JMapInfo
{
public:
    JMapInfo();
    JMapInfo(u32 val);
    ~JMapInfo() { }

    bool attach(const void *);
    void setName(const char *);
    const char* getName() const;
    s32 searchItemInfo(const char *) const;
    s32 getValueType(const char *) const;
    s32 getValueFast(s32, s32, const char **) const;

    template<typename T>
    void getValue(s32, const char *, T *) const;

    const JMapData* mData; // _0
    const char* mName; // _4
};

#endif // JMAPINFO_H