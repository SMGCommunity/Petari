#include <RVLFaceLibInternal.h>
#include <cstdio>

void RFLi_MakeRandomFace(RFLiCharInfo* info, RFLSex sex, RFLAge age,
                         RFLRace race) {
    RFLSex sex_;
    RFLAge age_;
    RFLRace race_;
    u32 rand;

    if (sex == RFLSex_All) {
        if (RFLi_GetRandU32(RFLSex_All) == RFLSex_Male) {
            sex_ = RFLSex_Male;
        } else {
            sex_ = RFLSex_Female;
        }
    } else {
        sex_ = sex;
    }

    if (age == RFLAge_All) {
        rand = RFLi_GetRandU32(10);

        if (rand < 4) {
            age_ = RFLAge_Child;
        } else if (rand < 8) {
            age_ = RFLAge_Adult;
        } else {
            age_ = RFLAge_Elder;
        }
    } else {
        age_ = age;
    }

    switch (race) {
    case RFLRace_Black:
        race_ = RFLRace_Black;
        break;
    case RFLRace_White:
        race_ = RFLRace_White;
        break;
    case RFLRace_Asian:
        race_ = RFLRace_Asian;
        break;
    case RFLRace_All:
        rand = RFLi_GetRandU32(10);

        if (rand < 4) {
            race_ = RFLRace_Asian;
        } else if (rand < 8) {
            race_ = RFLRace_White;
        } else {
            race_ = RFLRace_Black;
        }
        break;
    }

    RFLi_MakeRandomFace_Core(info, sex_, age_, race_);
}

void RFLi_MakeRandomFace_Core(RFLiCharInfo* info, RFLSex sex, RFLAge age,
                              RFLRace race) {
    int sex_ = sex;
    int age_ = age;
    int race_ = race;
    u8 y = 0;
    int i;

    if (sex_ == RFLSex_Female) {
        y += RFLi_GetRandU32(3);
    }

    if (age_ == RFLAge_Child) {
        y += RFLi_GetRandU32(3);
    }

    info->faceline.type = RFLi_GetFacelineType(sex_, age_, race_);
    info->faceline.color = RFLi_GetFaceColor(sex_, race_);
    info->faceline.texture = RFLi_GetFaceTexType(sex_, age_, race_);

    info->hair.type = RFLi_GetHairType(sex_, age_, race_);
    info->hair.color = RFLi_GetHairColor(age_, race_);
    info->hair.flip = RFLi_GetRandU32(TRUE + 1);

    info->eye.type = RFLi_GetEyeType(sex_, age_, race_);
    info->eye.color = RFLi_GetEyeColor(race_);
    info->eye.scale = 4;
    if (sex_ == RFLSex_Male) {
        info->eye.rotate = 4;
        info->eye.rotate +=
            (RFLi_EYE_ROT_OFFSET[2] - RFLi_EYE_ROT_OFFSET[info->eye.type]);
    } else {
        info->eye.rotate = 3;
        info->eye.rotate +=
            (RFLi_EYE_ROT_OFFSET[4] - RFLi_EYE_ROT_OFFSET[info->eye.type]);
    }
    info->eye.x = 2;
    info->eye.y = y + 12;

    info->eyebrow.type = RFLi_GetEyebrowType(sex_, age_, race_);
    info->eyebrow.color = info->hair.color;
    info->eyebrow.scale = 4;
    info->eyebrow.rotate = 6;
    info->eyebrow.x = 2;
    if (race_ == RFLRace_Asian) {
        info->eyebrow.y = y + 9;
        info->eyebrow.rotate += (RFLi_EYEBROW_ROT_OFFSET[6] -
                                 RFLi_EYEBROW_ROT_OFFSET[info->eyebrow.type]);
    } else {
        info->eyebrow.y = y + 10;
        info->eyebrow.rotate += (RFLi_EYEBROW_ROT_OFFSET[0] -
                                 RFLi_EYEBROW_ROT_OFFSET[info->eyebrow.type]);
    }

    info->nose.type = RFLi_GetNoseType(sex_, age_, race_);
    if (sex_ == RFLSex_Male) {
        info->nose.scale = 4;
    } else {
        info->nose.scale = 3;
    }
    info->nose.y = y + 9;

    info->mouth.type = RFLi_GetMouthType(sex_, age_, race_);
    if (sex_ == RFLSex_Male) {
        info->mouth.color = 0;
    } else {
        info->mouth.color = RFLi_GetRandU32(RFLi_MAX_MOUTH_COLOR + 1);
    }
    info->mouth.scale = 4;
    info->mouth.y = y + 13;

    if (sex_ == RFLSex_Male && (age_ == RFLAge_Adult || age_ == RFLAge_Elder) &&
        RFLi_GetRandU32(10) < 2) {
        u32 r = RFLi_GetRandU32(3);

        if (r == 0) {
            info->beard.mustache = 0;
            info->beard.type = RFLi_GetRandU32(RFLi_MAX_BEARD_TYPE) + 1;
        } else if (r == 1) {
            info->beard.mustache = RFLi_GetRandU32(RFLi_MAX_BEARD_MUSTACHE) + 1;
            info->beard.type = 0;
        } else if (r == 2) {
            info->beard.mustache = RFLi_GetRandU32(RFLi_MAX_BEARD_MUSTACHE) + 1;
            info->beard.type = RFLi_GetRandU32(RFLi_MAX_BEARD_TYPE) + 1;
        }

        info->beard.color = info->hair.color;
        info->beard.scale = 4;
        info->beard.y = 10;
    } else {
        info->beard.mustache = 0;
        info->beard.type = 0;
        info->beard.color = 0;
        info->beard.scale = 0;
        info->beard.y = 0;
    }

    info->glass.type = RFLi_GetGlassType(age_);
    info->glass.color = 0;
    info->glass.scale = 4;
    info->glass.y = y + 10;

    info->mole.type = 0;
    info->mole.scale = 4;
    info->mole.x = RFLi_GetRandU32(RFLi_MAX_MOLE_X + 1);
    info->mole.y = RFLi_GetRandU32(RFLi_MAX_MOLE_Y + 1);

    info->body.height = RFLi_MAX_BODY_HEIGHT / 2;
    info->body.build = RFLi_MAX_BODY_BUILD / 2;

    for (i = 0; i < RFL_NAME_LEN + 1; i++) {
        info->personal.name[i] = L'\0';
    }
    wcsncpy(info->personal.name, L"no name",
            sizeof(L"no name") / sizeof(wchar_t) - 1);

    for (i = 0; i < RFL_CREATOR_LEN + 1; i++) {
        info->personal.creator[i] = L'\0';
    }

    info->personal.sex = sex_;
    info->personal.bmonth = 0;
    info->personal.bday = 0;
    info->personal.color = RFLi_GetRandU32(RFLFavoriteColor_Max);
    info->personal.favorite = FALSE;
    info->personal.localOnly = FALSE;
}

u8 RFLi_GetFacelineType(RFLSex sex, RFLAge age, RFLRace race) {
    static const u8
        facelineTypeTable[RFLSex_All * RFLAge_All * RFLRace_All][10] = {
            // clang-format off
        /* Male,   Child, Black */ {0, 0, 0, 1, 1, 1, 2, 3, 4, 5},
        /* Male,   Child, White */ {0, 0, 0, 1, 1, 1, 2, 3, 4, 5},
        /* Male,   Child, Asian */ {0, 0, 0, 1, 1, 1, 2, 3, 4, 5},
        /* Male,   Adult, Black */ {0, 0, 1, 2, 2, 3, 4, 5, 6, 7},
        /* Male,   Adult, White */ {0, 1, 2, 2, 3, 4, 5, 6, 6, 7},
        /* Male,   Adult, Asian */ {0, 0, 1, 2, 2, 3, 4, 5, 6, 7},
        /* Male,   Elder, Black */ {0, 0, 1, 2, 2, 3, 4, 5, 6, 7},
        /* Male,   Elder, White */ {0, 1, 2, 2, 3, 4, 5, 6, 7, 7},
        /* Male,   Elder, Asian */ {0, 0, 1, 2, 2, 3, 4, 5, 6, 7},
        /* Female, Child, Black */ {0, 0, 0, 1, 1, 1, 2, 3, 4, 5},
        /* Female, Child, White */ {0, 0, 0, 1, 1, 1, 2, 3, 4, 5},
        /* Female, Child, Asian */ {0, 0, 0, 1, 1, 1, 2, 3, 4, 5},
        /* Female, Adult, Black */ {0, 0, 0, 1, 1, 2, 2, 3, 4, 5},
        /* Female, Adult, White */ {0, 0, 0, 1, 1, 2, 2, 3, 4, 5},
        /* Female, Adult, Asian */ {0, 0, 0, 1, 1, 2, 2, 3, 4, 5},
        /* Female, Elder, Black */ {0, 0, 0, 1, 1, 2, 2, 3, 4, 5},
        /* Female, Elder, White */ {0, 0, 0, 1, 1, 2, 2, 3, 4, 5},
        /* Female, Elder, Asian */ {0, 0, 0, 1, 1, 2, 2, 3, 4, 5}
            // clang-format on
        };

    return facelineTypeTable[(sex * 9) + (age * 3) + race]
                            [RFLi_GetRandU32(LENGTHOF(facelineTypeTable[0]))];
}

u8 RFLi_GetHairType(RFLSex sex, RFLAge age, RFLRace race) {
    const u8 hair_parts[RFLSex_All * RFLAge_All * RFLRace_All][72] = {
        // clang-format off
        /* Male,   Child, Black */ {13, 23, 30, 31, 32, 33, 34, 35, 36, 37, 38, 40, 43, 44, 45, 47, 48, 49, 50, 51, 52, 54, 56, 57, 64, 66,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Male,   Child, White */ {13, 23, 30, 31, 32, 33, 34, 35, 36, 37, 38, 40, 43, 44, 45, 47, 48, 49, 50, 51, 52, 54, 56, 57, 64, 66,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Male,   Child, Asian */ {13, 23, 30, 31, 32, 33, 34, 35, 36, 37, 38, 40, 43, 44, 45, 47, 48, 49, 50, 51, 52, 54, 56, 57, 64, 66,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Male,   Adult, Black */ {13, 23, 30, 31, 32, 33, 34, 36, 37, 38, 40, 42, 43, 44, 45, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 58, 59, 60, 64, 65, 66, 67, 68, 70,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Male,   Adult, White */ {13, 23, 30, 31, 32, 33, 34, 36, 37, 38, 39, 40, 43, 44, 45, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 58, 59, 60, 64, 65, 66, 67, 68, 70,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Male,   Adult, Asian */ {13, 23, 30, 31, 32, 33, 34, 36, 37, 38, 39, 40, 43, 44, 45, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 58, 59, 60, 64, 65, 66, 67, 68, 70,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Male,   Elder, Black */ {13, 23, 30, 36, 37, 41, 45, 47, 51, 53, 54, 55, 58, 59, 65, 67,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Male,   Elder, White */ {13, 23, 30, 36, 37, 39, 41, 45, 47, 51, 53, 54, 55, 58, 59, 65, 67,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Male,   Elder, Asian */ {13, 23, 30, 36, 37, 39, 41, 45, 47, 51, 53, 54, 55, 58, 59, 65, 67,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Female, Child, Black */ { 0,  1,  2,  3,  4,  5,  6,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 24, 25, 26, 28, 46, 50, 61, 62, 63, 64, 69,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Female, Child, White */ { 0,  1,  2,  3,  4,  5,  6,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 24, 25, 26, 28, 46, 50, 61, 62, 63, 64, 69,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Female, Child, Asian */ { 0,  1,  2,  3,  4,  5,  6,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 24, 25, 26, 28, 46, 50, 61, 62, 63, 64, 69,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Female, Adult, Black */ { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 24, 25, 26, 27, 29, 42, 50, 58, 60, 62, 63, 64, 69, 71,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Female, Adult, White */ { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 24, 25, 26, 27, 29, 50, 58, 60, 62, 63, 64, 69, 71,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Female, Adult, Asian */ { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 24, 25, 26, 27, 29, 50, 58, 60, 62, 63, 64, 69, 71,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Female, Elder, Black */ { 0,  1,  2,  3,  4,  5,  6, 10, 11, 12, 13, 14, 16, 17, 18, 20, 21, 24, 25, 58, 62, 69,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Female, Elder, White */ { 0,  1,  2,  3,  4,  5,  6, 10, 11, 12, 13, 14, 16, 17, 18, 20, 21, 24, 25, 58, 62, 69,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Female, Elder, Asian */ { 0,  1,  2,  3,  4,  5,  6, 10, 11, 12, 13, 14, 16, 17, 18, 20, 21, 24, 25, 58, 62, 69,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}
        // clang-format on
    };

    const u8 hair_parts_num[RFLSex_All * RFLAge_All * RFLRace_All] = {
        26, 26, 26, 34, 34, 34, 16, 17, 17, 33, 33, 33, 37, 36, 36, 22, 22, 22};

    u8 index = (sex * 9) + (age * 3) + race;
    return hair_parts[index][RFLi_GetRandU32(hair_parts_num[index])];
}

u8 RFLi_GetEyeType(RFLSex sex, RFLAge age, RFLRace race) {
    const u8 eye_parts[RFLSex_All * RFLAge_All * RFLRace_All][48] = {
        // clang-format off
        /* Male,   Child, Black */ { 2,  3,  5,  7,  8,  9, 11, 12, 13, 15, 16, 18, 27, 29, 32, 34, 36, 38, 39, 41, 43, 47,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Male,   Child, White */ { 2,  3,  5,  7,  8,  9, 11, 12, 13, 15, 16, 18, 27, 29, 32, 34, 36, 38, 39, 41, 43, 47,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Male,   Child, Asian */ { 2,  3,  5,  7,  8,  9, 11, 12, 13, 15, 16, 18, 26, 27, 29, 32, 34, 36, 38, 39, 41, 43, 47,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Male,   Adult, Black */ { 2,  3,  5,  6,  7,  8,  9, 11, 12, 13, 15, 16, 17, 18, 21, 22, 27, 29, 31, 32, 34, 36, 37, 38, 39, 41, 43, 44, 47,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Male,   Adult, White */ { 2,  3,  5,  6,  7,  8,  9, 11, 12, 13, 15, 16, 17, 18, 21, 22, 27, 29, 31, 32, 34, 36, 37, 38, 39, 41, 43, 44, 47,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Male,   Adult, Asian */ { 2,  3,  5,  6,  7,  8,  9, 11, 12, 13, 15, 16, 18, 21, 22, 26, 27, 29, 31, 32, 34, 36, 37, 38, 39, 41, 43, 44, 47,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Male,   Elder, Black */ { 2,  3,  5,  7,  8,  9, 11, 12, 13, 14, 15, 16, 17, 18, 21, 22, 31, 32, 34, 36, 37, 39, 41, 44,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Male,   Elder, White */ { 2,  3,  5,  7,  8,  9, 11, 12, 13, 14, 15, 16, 17, 18, 21, 22, 31, 32, 34, 36, 37, 39, 41, 44,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Male,   Elder, Asian */ { 2,  3,  5,  7,  8,  9, 11, 12, 13, 14, 15, 16, 18, 21, 22, 26, 31, 32, 34, 36, 37, 39, 41, 44,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Female, Child, Black */ { 0,  1,  2,  4,  5,  7,  8,  9, 10, 11, 12, 13, 15, 16, 18, 19, 23, 24, 25, 27, 28, 29, 32, 33, 34, 35, 38, 39, 40, 41, 42, 45, 46, 47,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Female, Child, White */ { 0,  1,  2,  4,  5,  7,  8,  9, 10, 11, 12, 13, 15, 16, 18, 19, 23, 24, 25, 27, 28, 29, 32, 33, 34, 35, 38, 39, 40, 41, 42, 45, 46, 47,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Female, Child, Asian */ { 0,  1,  2,  4,  5,  7,  8,  9, 10, 11, 12, 13, 15, 16, 18, 19, 23, 24, 25, 26, 27, 28, 29, 32, 33, 34, 35, 38, 39, 40, 41, 42, 45, 46, 47,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Female, Adult, Black */ { 0,  1,  2,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 15, 16, 17, 18, 19, 20, 21, 23, 24, 25, 27, 28, 29, 30, 32, 33, 34, 35, 37, 38, 39, 40, 41, 42, 45, 46, 47,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Female, Adult, White */ { 0,  1,  2,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 15, 16, 17, 18, 19, 20, 21, 23, 24, 25, 27, 28, 29, 30, 32, 33, 34, 35, 37, 38, 39, 40, 41, 42, 45, 46, 47,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Female, Adult, Asian */ { 0,  1,  2,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 15, 16, 18, 19, 20, 21, 23, 24, 25, 26, 27, 28, 29, 30, 32, 33, 34, 35, 37, 38, 39, 40, 41, 42, 45, 46, 47,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Female, Elder, Black */ { 1,  2,  5,  7,  8,  9, 11, 12, 13, 14, 15, 16, 17, 18, 21, 32, 34, 37, 39, 41,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Female, Elder, White */ { 1,  2,  5,  7,  8,  9, 11, 12, 13, 14, 15, 16, 17, 18, 21, 32, 34, 37, 39, 41,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Female, Elder, Asian */ { 1,  2,  5,  7,  8,  9, 11, 12, 13, 14, 15, 16, 18, 21, 26, 32, 34, 37, 39, 41,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}
        // clang-format on
    };

    const u8 eye_parts_num[RFLSex_All * RFLAge_All * RFLRace_All] = {
        22, 22, 23, 29, 29, 29, 24, 24, 24, 34, 34, 35, 40, 40, 40, 20, 20, 20};

    u8 index = (sex * 9) + (age * 3) + race;
    return eye_parts[index][RFLi_GetRandU32(eye_parts_num[index])];
}

u8 RFLi_GetEyebrowType(RFLSex sex, RFLAge age, RFLRace race) {
    static const u8 eyebrow_parts[RFLSex_All * RFLAge_All * RFLRace_All][24] = {
        // clang-format off
        /* Male,   Child, Black */ { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 17, 18, 20,  0,  0,  0,  0,  0,  0},
        /* Male,   Child, White */ { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 17, 18, 20,  0,  0,  0,  0,  0,  0},
        /* Male,   Child, Asian */ { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 17, 18, 20,  0,  0,  0,  0,  0,  0},
        /* Male,   Adult, Black */ { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,  0},
        /* Male,   Adult, White */ { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,  0},
        /* Male,   Adult, Asian */ { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,  0},
        /* Male,   Elder, Black */ { 0,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 20, 21, 22,  0,  0,  0},
        /* Male,   Elder, White */ { 0,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 20, 21, 22,  0,  0,  0},
        /* Male,   Elder, Asian */ { 0,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 20, 21, 22,  0,  0,  0},
        /* Female, Child, Black */ { 0,  1,  3,  7,  8,  9, 10, 11, 13,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Female, Child, White */ { 0,  1,  3,  7,  8,  9, 10, 11, 13,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Female, Child, Asian */ { 0,  1,  3,  7,  8,  9, 10, 11, 13,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Female, Adult, Black */ { 0,  1,  3,  7,  8,  9, 10, 11, 13, 15, 19,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Female, Adult, White */ { 0,  1,  3,  7,  8,  9, 10, 11, 13, 15, 19,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Female, Adult, Asian */ { 0,  1,  3,  7,  8,  9, 10, 11, 13, 15, 19,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Female, Elder, Black */ { 0,  3,  7,  8,  9, 10, 11, 13, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Female, Elder, White */ { 0,  3,  7,  8,  9, 10, 11, 13, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /* Female, Elder, Asian */ { 0,  3,  7,  8,  9, 10, 11, 13, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        // clang-format on
    };

    const u8 eyebrow_parts_num[RFLSex_All * RFLAge_All * RFLRace_All] = {
        18, 18, 18, 23, 23, 23, 21, 21, 21, 9, 9, 9, 11, 11, 11, 9, 9, 9};

    u8 index = (sex * 9) + (age * 3) + race;
    return eyebrow_parts[index][RFLi_GetRandU32(eyebrow_parts_num[index])];
}

u8 RFLi_GetNoseType(RFLSex sex, RFLAge age, RFLRace race) {
    const u8 nose_parts[RFLSex_All * RFLAge_All * RFLRace_All][12] = {
        // clang-format off
        /* Male,   Child, Black */ { 0,  1,  2,  3,  4,  5,  7,  8, 10,  0,  0,  0},
        /* Male,   Child, White */ { 0,  1,  2,  3,  4,  5,  7,  8, 10,  0,  0,  0},
        /* Male,   Child, Asian */ { 0,  1,  2,  3,  4,  5,  7,  8, 10,  0,  0,  0},
        /* Male,   Adult, Black */ { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11},
        /* Male,   Adult, White */ { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11},
        /* Male,   Adult, Asian */ { 0,  1,  2,  3,  4,  5,  6,  7,  8, 10, 11,  0},
        /* Male,   Elder, Black */ { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11},
        /* Male,   Elder, White */ { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11},
        /* Male,   Elder, Asian */ { 0,  1,  2,  3,  4,  5,  6,  7,  8, 10, 11,  0},
        /* Female, Child, Black */ { 0,  1,  3,  4,  8, 10,  0,  0,  0,  0,  0,  0},
        /* Female, Child, White */ { 0,  1,  3,  4,  8, 10,  0,  0,  0,  0,  0,  0},
        /* Female, Child, Asian */ { 0,  1,  3,  4,  8, 10,  0,  0,  0,  0,  0,  0},
        /* Female, Adult, Black */ { 0,  1,  3,  4,  6,  8,  9, 10, 11,  0,  0,  0},
        /* Female, Adult, White */ { 0,  1,  3,  4,  6,  8,  9, 10, 11,  0,  0,  0},
        /* Female, Adult, Asian */ { 0,  1,  3,  4,  6,  8, 10, 11,  0,  0,  0,  0},
        /* Female, Elder, Black */ { 0,  1,  3,  4,  6,  8,  9, 10, 11,  0,  0,  0},
        /* Female, Elder, White */ { 0,  1,  3,  4,  6,  8,  9, 10, 11,  0,  0,  0},
        /* Female, Elder, Asian */ { 0,  1,  3,  4,  6,  8, 10, 11,  0,  0,  0,  0}
        // clang-format on
    };

    const u8 nose_parts_num[RFLSex_All * RFLAge_All * RFLRace_All] = {
        9, 9, 9, 12, 12, 11, 12, 12, 11, 6, 6, 6, 9, 9, 8, 9, 9, 8};

    u8 index = (sex * 9) + (age * 3) + race;
    return nose_parts[index][RFLi_GetRandU32(nose_parts_num[index])];
}

u8 RFLi_GetMouthType(RFLSex sex, RFLAge age, RFLRace race) {
    const u8 mouth_parts[RFLSex_All * RFLAge_All * RFLRace_All][24] = {
        // clang-format off
        /* Male,   Child, Black */ { 0,  2,  3,  6,  7,  8,  9, 10, 12, 14, 15, 17, 18, 19, 21, 22, 23,  0,  0,  0,  0,  0,  0,  0},
        /* Male,   Child, White */ { 0,  2,  3,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 17, 18, 19, 21, 22, 23,  0,  0,  0,  0,  0},
        /* Male,   Child, Asian */ { 0,  2,  3,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 17, 18, 19, 21, 22, 23,  0,  0,  0,  0,  0},
        /* Male,   Adult, Black */ { 0,  2,  3,  6,  7,  8,  9, 10, 12, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,  0,  0,  0,  0,  0},
        /* Male,   Adult, White */ { 0,  2,  3,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,  0,  0,  0},
        /* Male,   Adult, Asian */ { 0,  2,  3,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,  0,  0,  0},
        /* Male,   Elder, Black */ { 0,  2,  3,  6,  7,  8,  9, 10, 12, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,  0,  0,  0,  0,  0},
        /* Male,   Elder, White */ { 0,  2,  3,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,  0,  0,  0},
        /* Male,   Elder, Asian */ { 0,  2,  3,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,  0,  0,  0},
        /* Female, Child, Black */ { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 12, 14, 15, 17, 18, 19, 21, 22, 23,  0,  0,  0,  0,  0},
        /* Female, Child, White */ { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 12, 13, 14, 15, 17, 18, 19, 21, 22, 23,  0,  0,  0,  0},
        /* Female, Child, Asian */ { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 12, 13, 14, 15, 17, 18, 19, 21, 22, 23,  0,  0,  0,  0},
        /* Female, Adult, Black */ { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 12, 14, 15, 17, 18, 19, 21, 22, 23,  0,  0,  0,  0,  0},
        /* Female, Adult, White */ { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 12, 13, 14, 15, 17, 18, 19, 21, 22, 23,  0,  0,  0,  0},
        /* Female, Adult, Asian */ { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 12, 13, 14, 15, 17, 18, 19, 21, 22, 23,  0,  0,  0,  0},
        /* Female, Elder, Black */ { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 12, 14, 15, 16, 17, 18, 19, 21, 22, 23,  0,  0,  0,  0},
        /* Female, Elder, White */ { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23,  0,  0,  0},
        /* Female, Elder, Asian */ { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23,  0,  0,  0}
        // clang-format on
    };

    const u8 mouth_parts_num[RFLSex_All * RFLAge_All * RFLRace_All] = {
        17, 19, 19, 19, 21, 21, 19, 21, 21, 19, 20, 20, 19, 20, 20, 20, 21, 21};

    u8 index = (sex * 9) + (age * 3) + race;
    return mouth_parts[index][RFLi_GetRandU32(mouth_parts_num[index])];
}

u8 RFLi_GetFaceTexType(RFLSex sex, RFLAge age, RFLRace race) {
    static const u8
        faceTexTypeTable[RFLSex_All * RFLAge_All * RFLRace_All][20] = {
            // clang-format off
        /* Male,   Child, Black */ { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  3,  3},
        /* Male,   Child, White */ { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  3,  3,  3,  3,  8,  8},
        /* Male,   Child, Asian */ { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  3,  3},
        /* Male,   Adult, Black */ { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  3,  4,  5,  6,  7,  8,  9, 10},
        /* Male,   Adult, White */ { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  3,  4,  5,  6,  7,  8,  9, 10},
        /* Male,   Adult, Asian */ { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  3,  4,  5,  6,  7,  9, 10},
        /* Male,   Elder, Black */ {10, 10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11},
        /* Male,   Elder, White */ {10, 10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11},
        /* Male,   Elder, Asian */ {10, 10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11},
        /* Female, Child, Black */ { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  3,  3,  4,  4},
        /* Female, Child, White */ { 0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  3,  3,  4,  4},
        /* Female, Child, Asian */ { 0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  3,  3,  4,  4},
        /* Female, Adult, Black */ { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  3,  3,  4,  4,  8,  8},
        /* Female, Adult, White */ { 0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  4,  4,  8,  8},
        /* Female, Adult, Asian */ { 0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  4,  4},
        /* Female, Elder, Black */ {10, 10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11},
        /* Female, Elder, White */ {10, 10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11},
        /* Female, Elder, Asian */ {10, 10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11}
            // clang-format on
        };

    return faceTexTypeTable[(sex * 9) + (age * 3) + race]
                           [RFLi_GetRandU32(LENGTHOF(faceTexTypeTable[0]))];
}

u8 RFLi_GetGlassType(RFLAge age) {
    u32 rand = RFLi_GetRandU32(100);

    switch (age) {
    case RFLAge_Child:
        if (rand < 90) {
            return 0;
        }
        if (rand < 94) {
            return 1;
        }
        if (rand < 96) {
            return 2;
        }
        return 3;
    case RFLAge_Adult:
        if (rand < 83) {
            return 0;
        }
        if (rand < 86) {
            return 1;
        }
        if (rand < 90) {
            return 2;
        }
        if (rand < 93) {
            return 3;
        }
        if (rand < 94) {
            return 4;
        }
        if (rand < 96) {
            return 5;
        }
        if (rand < 98) {
            return 6;
        }
        return 7;
    case RFLAge_Elder:
        if (rand < 78) {
            return 0;
        }
        if (rand < 83) {
            return 1;
        }
        if (rand < 93) {
            return 3;
        }
        if (rand < 98) {
            return 6;
        }
        return 7;
    default:
        return 0;
    }
}

u8 RFLi_GetFaceColor(RFLSex sex, RFLRace race) {
    static const u8 faceColorTable[RFLSex_All * RFLRace_All][10] = {
        // clang-format off
        /* Male,   Black */ {2, 2, 4, 4, 4, 4, 5, 5, 5, 5},
        /* Male,   White */ {0, 0, 0, 0, 1, 1, 2, 3, 3, 3},
        /* Male,   Asian */ {0, 0, 1, 1, 1, 1, 1, 1, 1, 2},
        /* Female, Black */ {2, 2, 4, 4, 4, 4, 5, 5, 5, 5},
        /* Female, White */ {0, 0, 0, 0, 0, 0, 0, 0, 1, 3},
        /* Female, Asian */ {0, 0, 0, 0, 0, 0, 0, 0, 1, 1}
        // clang-format on
    };

    return faceColorTable[(sex * 3) + race]
                         [RFLi_GetRandU32(LENGTHOF(faceColorTable[0]))];
}

u8 RFLi_GetHairColor(RFLAge age, RFLRace race) {
    static const u8 hairColorTable[RFLAge_All * RFLRace_All][20] = {
        // clang-format off
        /* Black, Child */ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        /* Black, Adult */ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        /* Black, Elder */ {0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
        /* White, Child */ {2, 3, 3, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7},
        /* White, Adult */ {2, 3, 3, 3, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7},
        /* White, Elder */ {2, 3, 3, 4, 4, 4, 4, 4, 4, 5, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7},
        /* Asian, Child */ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
        /* Asian, Adult */ {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 3, 3, 3, 3},
        /* Asian, Elder */ {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4}
        // clang-format on
    };

    return hairColorTable[(race * 4 - race) + age]
                         [RFLi_GetRandU32(LENGTHOF(hairColorTable[0]))];
}

u8 RFLi_GetEyeColor(RFLRace race) {
    static const u8 eyeColorTable[RFLRace_All][10] = {
        // clang-format off
        /* Black */ {0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        /* White */ {0, 1, 1, 2, 3, 3, 4, 4, 4, 5},
        /* Asian */ {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
        // clang-format on
    };

    return eyeColorTable[race][RFLi_GetRandU32(LENGTHOF(eyeColorTable[0]))];
}

u32 RFLi_GetRandU32(u32 max) {
    return OSGetTick() % max;
}
