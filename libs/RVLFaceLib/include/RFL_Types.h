#ifndef RFL_TYPES_H
#define RFL_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>
#include <revolution/gx.h>

typedef enum {
	RFLErrcode_Success 			= 0,
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
	RFLErrcode_Unknown			= 255
} RFLErrcode;

typedef struct RFLCreateID {
	u8	data[8];
} _RFLCreateID;

typedef struct RFLCharData {
	u8 data[0x4A];	
} _RFLCharData;

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
	RFLExp_Normal = 0,
	RFLExp_Smile,
	RFLExp_Anger,
	RFLExp_Sorrow,
	RFLExp_Surprise,
	RFLExp_Blink,
	RFLExp_OpenMouth,
	RFLExp_Max
} RFLExpression;

typedef enum {
	RFLExpFlag_Normal		= 0x1 << RFLExp_Normal,
	RFLExpFlag_Smile		= 0x1 << RFLExp_Smile,
	RFLExpFlag_Anger		= 0x1 << RFLExp_Anger,
	RFLExpFlag_Sorrow		= 0x1 << RFLExp_Sorrow,
	RFLExpFlag_Surprise		= 0x1 << RFLExp_Surprise,
	RFLExpFlag_Blink		= 0x1 << RFLExp_Blink,
	RFLExpFlag_OpenMouth	= 0x1 << RFLExp_OpenMouth
} RFLExpressionFlag;

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
	RFLGender_Male,
	RFLGender_Female,
	RFLGender_Max
} RFLGender;

typedef struct {
    u16 name[11];
    u16 creator[11];
    RFLCreateID createID;
    u32 gender : 1;
    u32 birth_month : 4;
    u32 birth_day : 5;
    u32 favoriteColor : 4;
    u32 favorite : 1;
    u32 height : 7;
    u32 build : 7;
    u32 reserved : 3;
    GXColor skinColor;
} RFLAdditionalInfo;

typedef enum {
	RFLSex_Male,
	RFLSex_Female,
	RFLSex_All
} RFLSex;

typedef enum {
	RFLAge_Child,
	RFLAge_Adult,
	RFLAge_Elder,
	RFLAge_All
} RFLAge;

typedef enum {
	RFLRace_Black,
	RFLRace_White,
	RFLRace_Asian,
	RFLRace_All
} RFLRace;

typedef struct {
	u8 data[24];
} RFLMiddleDB;

typedef struct {
	u8 data[76];
} RFLStoreData;

typedef struct {
	u8 data[16];
} RFLWiFiInformation;

#ifdef __cplusplus
}
#endif

#endif // RFL_TYPES_H