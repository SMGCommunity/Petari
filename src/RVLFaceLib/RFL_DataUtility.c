#include <RVLFaceLibInternal.h>
#include <cstdio>

// For masking out padding when comparing fields
#define PADMASK(t, size) ((t)(~((1 << size) - 1)))
// Padding bitmask constants (see RFLiCharInfo structure)
#define FACELINE_PAD_MASK (PADMASK(u16, 6))
#define HAIR_PAD_MASK (PADMASK(u16, 5))
#define EYE_PAD_MASK (PADMASK(u32, 5))
#define EYEBROW_PAD_MASK (PADMASK(u32, 6))
#define NOSE_PAD_MASK (PADMASK(u16, 3))
#define MOUTH_PAD_MASK (PADMASK(u16, 0))
#define BEARD_PAD_MASK (PADMASK(u16, 0))
#define GLASS_PAD_MASK (PADMASK(u16, 1))
#define MOLE_PAD_MASK (PADMASK(u16, 1))

BOOL RFLiCheckValidInfo(const RFLiCharInfo* info) {
    if (info->faceline.type > RFLi_MAX_FACELINE_TYPE) {
        return FALSE;
    }
    if (info->faceline.color > RFLi_MAX_FACELINE_COLOR) {
        return FALSE;
    }
    if (info->faceline.texture > RFLi_MAX_FACELINE_TEXTURE) {
        return FALSE;
    }

    if (info->hair.type > RFLi_MAX_HAIR_TYPE) {
        return FALSE;
    }
    if (info->hair.color > RFLi_MAX_HAIR_COLOR) {
        return FALSE;
    }
    if (info->hair.flip > TRUE) {
        return FALSE;
    }

    if (info->eye.type > RFLi_MAX_EYE_TYPE) {
        return FALSE;
    }
    if (info->eye.color > RFLi_MAX_EYE_COLOR) {
        return FALSE;
    }
    if (info->eye.scale > RFLi_MAX_EYE_SCALE) {
        return FALSE;
    }
    if (info->eye.rotate > RFLi_MAX_EYE_ROTATE) {
        return FALSE;
    }
    if (info->eye.x > RFLi_MAX_EYE_X) {
        return FALSE;
    }
    if (info->eye.y > RFLi_MAX_EYE_Y) {
        return FALSE;
    }

    if (info->eyebrow.type > RFLi_MAX_EYEBROW_TYPE) {
        return FALSE;
    }
    if (info->eyebrow.color > RFLi_MAX_EYEBROW_COLOR) {
        return FALSE;
    }
    if (info->eyebrow.scale > RFLi_MAX_EYEBROW_SCALE) {
        return FALSE;
    }
    if (info->eyebrow.rotate > RFLi_MAX_EYEBROW_ROTATE) {
        return FALSE;
    }
    if (info->eyebrow.x > RFLi_MAX_EYEBROW_X) {
        return FALSE;
    }
    if (info->eyebrow.y < RFLi_MIN_EYEBROW_Y) {
        return FALSE;
    }
    if (info->eyebrow.y > RFLi_MAX_EYEBROW_Y) {
        return FALSE;
    }

    if (info->nose.type > RFLi_MAX_NOSE_TYPE) {
        return FALSE;
    }
    if (info->nose.scale > RFLi_MAX_NOSE_SCALE) {
        return FALSE;
    }
    if (info->nose.y > RFLi_MAX_NOSE_Y) {
        return FALSE;
    }

    if (info->mouth.type > RFLi_MAX_MOUTH_TYPE) {
        return FALSE;
    }
    if (info->mouth.color > RFLi_MAX_MOUTH_COLOR) {
        return FALSE;
    }
    if (info->mouth.scale > RFLi_MAX_MOUTH_SCALE) {
        return FALSE;
    }
    if (info->mouth.y > RFLi_MAX_MOUTH_Y) {
        return FALSE;
    }

    if (info->beard.mustache > RFLi_MAX_BEARD_MUSTACHE) {
        return FALSE;
    }
    if (info->beard.type > RFLi_MAX_BEARD_TYPE) {
        return FALSE;
    }
    if (info->beard.color > RFLi_MAX_BEARD_COLOR) {
        return FALSE;
    }
    if (info->beard.scale > RFLi_MAX_BEARD_SCALE) {
        return FALSE;
    }
    if (info->beard.y > RFLi_MAX_BEARD_Y) {
        return FALSE;
    }

    if (info->glass.type > RFLi_MAX_GLASS_TYPE) {
        return FALSE;
    }
    if (info->glass.color > RFLi_MAX_GLASS_COLOR) {
        return FALSE;
    }
    if (info->glass.scale > RFLi_MAX_GLASS_SCALE) {
        return FALSE;
    }
    if (info->glass.y > RFLi_MAX_GLASS_Y) {
        return FALSE;
    }

    if (info->mole.type > RFLi_MAX_MOLE_TYPE) {
        return FALSE;
    }
    if (info->mole.scale > RFLi_MAX_MOLE_SCALE) {
        return FALSE;
    }
    if (info->mole.x > RFLi_MAX_MOLE_X) {
        return FALSE;
    }
    if (info->mole.y > RFLi_MAX_MOLE_Y) {
        return FALSE;
    }

    if (!RFLiIsValidName2(info)) {
        return FALSE;
    }

    if (info->body.height > RFLi_MAX_BODY_HEIGHT) {
        return FALSE;
    }
    if (info->body.build > RFLi_MAX_BODY_BUILD) {
        return FALSE;
    }

    if (info->personal.sex > RFLSex_Female) {
        return FALSE;
    }
    if (!RFLiCheckBirthday(info->personal.bmonth, info->personal.bday)) {
        return FALSE;
    }
    if (info->personal.color >= RFLFavoriteColor_Max) {
        return FALSE;
    }

    if (RFLiIsSpecialID(&info->createID) && !info->personal.localOnly) {
        return FALSE;
    }

    return TRUE;
}

BOOL RFLiIsValidOnNAND(const RFLiCharInfo* info) {
    return !RFLiIsTemporaryID(&info->createID);
}

RFLErrcode RFLiPickupCharInfo(RFLiCharInfo* info, RFLDataSource source,
                              RFLMiddleDB* db, u16 index) {
    RFLErrcode err;

    if (!RFLAvailable()) {
        return RFLErrcode_NotAvailable;
    }

    switch (source) {
    case RFLDataSource_Official:
        err = RFLiGetCharInfo(info, index);
        break;
    case RFLDataSource_Controller1:
    case RFLDataSource_Controller2:
    case RFLDataSource_Controller3:
    case RFLDataSource_Controller4:
        if (RFLiGetControllerData(info, source - RFLDataSource_Controller1,
                                  (u8)index, FALSE)) {
            err = RFLErrcode_Success;
        } else {
            err = RFLErrcode_Broken;
        }
        break;
    case RFLDataSource_Middle:
        if (RFLiGetCharInfoMiddleDB(info, db, index)) {
            err = RFLErrcode_Success;
        } else {
            err = RFLErrcode_Broken;
        }
        break;
    case RFLDataSource_Default:
        RFLiGetDefaultData(info, index);
        err = RFLErrcode_Success;
        break;
    }

    if (err == RFLErrcode_Success) {
        if (!RFLiIsValidID(&info->createID)) {
            err = RFLErrcode_Broken;
        } else if (!RFLiCheckValidInfo(info)) {
            err = RFLErrcode_Broken;
        }
    }

    return err;
}

static void copyChar2Additional_(RFLAdditionalInfo* dst,
                                 const RFLiCharInfo* src) {
    u8 height;
    u8 build;

    memcpy(dst->name, src->personal.name, sizeof(wchar_t) * RFL_NAME_LEN);
    dst->name[RFL_NAME_LEN] = '\0';

    memcpy(dst->creator, src->personal.creator,
           sizeof(wchar_t) * RFL_CREATOR_LEN);
    // @bug Copy paste error
    dst->name[RFL_CREATOR_LEN] = '\0';

    dst->sex = src->personal.sex;

    if (RFLiCheckBirthday(src->personal.bmonth, src->personal.bday)) {
        dst->bmonth = src->personal.bmonth;
        dst->bday = src->personal.bday;
    } else {
        dst->bmonth = 0;
        dst->bday = 0;
    }

    dst->color = src->personal.color;
    dst->favorite = src->personal.favorite;

    height = src->body.height;
    build = src->body.build;

    // @bug Max height is 128, not 127
    if (height > RFLi_MAX_BODY_HEIGHT - 1) {
        height = RFLi_MAX_BODY_HEIGHT - 1;
    }
    // @bug Max build is 128, not 127
    if (build > RFLi_MAX_BODY_BUILD - 1) {
        build = RFLi_MAX_BODY_BUILD - 1;
    }

    dst->height = height;
    dst->build = build;
    memcpy(&dst->createID, &src->createID, sizeof(RFLCreateID));
    dst->reserved = 0;
    dst->skinColor = RFLiGetFacelineColor(src);
}

RFLErrcode RFLGetAdditionalInfo(RFLAdditionalInfo* addi, RFLDataSource source,
                                RFLMiddleDB* db, u16 index) {
    RFLiCharInfo info;
    RFLErrcode err;

    err = RFLiPickupCharInfo(&info, source, db, index);
    if (err == RFLErrcode_Success) {
        copyChar2Additional_(addi, &info);
    }

    return err;
}

static BOOL RFLiCheckBirthday(u8 month, u8 day) {
    // (One-indexed)
    static const u8 scDayMax[12 + 1] = {0,  31, 29, 31, 30, 31, 30,
                                        31, 31, 30, 31, 30, 31};

    if (month == 0 && day != 0) {
        return FALSE;
    }

    if (month != 0 && day == 0) {
        return FALSE;
    }

    if (month > 12 || day > 31) {
        return FALSE;
    }

    return day <= scDayMax[month];
}
