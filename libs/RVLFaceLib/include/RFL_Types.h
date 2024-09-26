#ifndef RVL_FACE_LIBRARY_TYPES_H
#define RVL_FACE_LIBRARY_TYPES_H
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Common types used throughout many RFL files.
 *
 * To avoid circular depencies, common structures and all enums have been moved
 * here.
 *
 * Everything is organized in alphabetical order (when possible, otherwise
 * dependency order).
 */

/**
 * Common defines
 */

#define RFL_NAME_LEN 10
#define RFL_CREATOR_LEN 10
#define RFL_CREATEID_LEN 8

#define RFL_CTRL_CHAR_MAX 10
#define RFL_DB_CHAR_MAX 100

/**
 * Common enums
 */

typedef enum { RFLAge_Child, RFLAge_Adult, RFLAge_Elder, RFLAge_All } RFLAge;

typedef enum {
    RFLDataSource_Official,
    RFLDataSource_Hidden,
    RFLDataSource_Controller1,
    RFLDataSource_Controller2,
    RFLDataSource_Controller3,
    RFLDataSource_Controller4,
    RFLDataSource_Default,
    RFLDataSource_Middle,

    RFLDataSource_Max
} RFLDataSource;

typedef enum {
    RFLErrcode_Success,
    RFLErrcode_NotAvailable,
    RFLErrcode_NANDCommandfail,
    RFLErrcode_Loadfail,
    RFLErrcode_Savefail,
    RFLErrcode_Fatal,
    RFLErrcode_Busy,
    RFLErrcode_Broken,
    RFLErrcode_Exist,
    RFLErrcode_DBFull,
    RFLErrcode_DBNodata,
    RFLErrcode_Controllerfail,
    RFLErrcode_NWC24Fail,
    RFLErrcode_MaxFiles,
    RFLErrcode_MaxBlocks,
    RFLErrcode_WrongParam,
    RFLErrcode_NoFriends,
    RFLErrcode_Isolation,

    RFLErrcode_Unknown = 0xFF
} RFLErrcode;

typedef enum {
    RFLExp_Normal,
    RFLExp_Smile,
    RFLExp_Anger,
    RFLExp_Sorrow,
    RFLExp_Surprise,
    RFLExp_Blink,
    RFLExp_OpenMouth,

    RFLExp_Max
} RFLExpression;

typedef enum {
    RFLFavoriteColor_Red,
    RFLFavoriteColor_Orange,
    RFLFavoriteColor_Yellow,
    RFLFavoriteColor_YellowGreen,
    RFLFavoriteColor_Green,
    RFLFavoriteColor_Blue,
    RFLFavoriteColor_SkyBlue,
    RFLFavoriteColor_Pink,
    RFLFavoriteColor_Purple,
    RFLFavoriteColor_Brown,
    RFLFavoriteColor_White,
    RFLFavoriteColor_Black,

    RFLFavoriteColor_Max
} RFLFavoriteColor;

typedef enum {
    RFLRace_Black,
    RFLRace_White,
    RFLRace_Asian,
    RFLRace_All
} RFLRace;

typedef enum {
    // No mipmap
    RFLResolution_64 = 64,
    RFLResolution_128 = 128,
    RFLResolution_256 = 256,

    // Mipmap
    RFLResolution_64M = 64 | 32,
    RFLResolution_128M = 128 | 64 | 32,
    RFLResolution_256M = 256 | 128 | 64 | 32
} RFLResolution;

typedef enum {
    RFLExpFlag_Normal = (1 << RFLExp_Normal),
    RFLExpFlag_Smile = (1 << RFLExp_Smile),
    RFLExpFlag_Anger = (1 << RFLExp_Anger),
    RFLExpFlag_Sorrow = (1 << RFLExp_Sorrow),
    RFLExpFlag_Surprise = (1 << RFLExp_Surprise),
    RFLExpFlag_Blink = (1 << RFLExp_Blink),
    RFLExpFlag_OpenMouth = (1 << RFLExp_OpenMouth),
} RFLExpFlag;

typedef enum { RFLSex_Male, RFLSex_Female, RFLSex_All } RFLSex;

/**
 * Common typedefs
 */

typedef void (*RFLCallback)(void);

/**
 * Common structs
 */
typedef struct RFLCreateID {
    u8 data[RFL_CREATEID_LEN];
} RFLCreateID;

typedef struct RFLCharData {
    u8 dummy[0x4A];
} RFLCharData;

typedef struct RFLStoreData {
    u8 dummy[0x4C];
} RFLStoreData;

#ifdef __cplusplus
}
#endif
#endif
