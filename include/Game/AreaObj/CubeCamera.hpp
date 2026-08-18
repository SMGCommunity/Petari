#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class CubeCameraArea : public AreaObj {
public:
    enum ECategory {
        /* 0x00 */ CATEGORY_UNKNOWN_0,
        /* 0x01 */ CATEGORY_UNKNOWN_1,
        /* 0x02 */ CATEGORY_UNKNOWN_2,
        /* 0x03 */ CATEGORY_UNKNOWN_3,
        /* 0x04 */ CATEGORY_UNKNOWN_4,
    };

    CubeCameraArea(int, const char*);

    virtual void init(const JMapInfoIter&);
    virtual void movement();
    virtual bool isInVolume(const TVec3f&) const;
    virtual s32 getCategoryArg() const;

    bool isValidCameraID() const;
    u16 getCameraID() const;

    static void setCurrentCategory(s32);

    static s32 sCubeCategory;

    u32 getZoneID() const {
        return mZoneID;
    }

    /* 0x3C */ s32 _3C;
    /* 0x40 */ u32 mZoneID;
};

class CubeCameraMgr : public AreaObjMgr {
public:
    CubeCameraMgr(s32, const char*);

    void initAfterLoad();
    void sort();
};
