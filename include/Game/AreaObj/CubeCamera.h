#pragma once

#include "Game/AreaObj/AreaObj.h"

class CubeCameraArea : public AreaObj {
public:
    enum ECategory {
        CATEGORY_UNKNOWN_0 = 0,
        CATEGORY_UNKNOWN_1 = 1,
        CATEGORY_UNKNOWN_2 = 2,
        CATEGORY_UNKNOWN_3 = 3,
        CATEGORY_UNKNOWN_4 = 4
    };
    
    CubeCameraArea(int, const char *);

    virtual ~CubeCameraArea() {

    }

    virtual void init(const JMapInfoIter &);
    virtual void movement();
    virtual bool isInVolume(const TVec3f &) const;
    virtual s32 getCategoryArg() const;

    bool isValidCameraID() const;
    u16 getCameraID() const;

    static void setCurrentCategory(s32);

    static s32 sCubeCategory;

    u32 _3C;
    u32 mZoneID; // _40
};

class CubeCameraMgr : public AreaObjMgr {
public:
    CubeCameraMgr(s32, const char *);

    virtual ~CubeCameraMgr();

    void initAfterLoad();
    void sort();
};