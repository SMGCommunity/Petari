#ifndef RVL_FACE_LIBRARY_INTERNAL_TYPES_H
#define RVL_FACE_LIBRARY_INTERNAL_TYPES_H
#include <RFL_Types.h>
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
 * Common enums
 */

typedef enum {
    RFLiArcID_Beard,
    RFLiArcID_Eye,
    RFLiArcID_Eyebrow,
    RFLiArcID_Faceline,
    RFLiArcID_FaceTex,
    RFLiArcID_ForeHead,
    RFLiArcID_Glass,
    RFLiArcID_GlassTex,
    RFLiArcID_Hair,
    RFLiArcID_Mask,
    RFLiArcID_Mole,
    RFLiArcID_Mouth,
    RFLiArcID_Mustache,
    RFLiArcID_Nose,
    RFLiArcID_Nline,
    RFLiArcID_NlineTex,
    RFLiArcID_Cap,
    RFLiArcID_CapTex,
    RFLiArcID_Max
} RFLiArcID;

typedef enum {
    RFLiAsyncTag_WriteFile,
    RFLiAsyncTag_WriteCloseFile,
    RFLiAsyncTag_ReadFile,
    RFLiAsyncTag_ReadCloseFile,
    RFLiAsyncTag_ReadCache,
    RFLiAsyncTag_ReadCacheClose,
    RFLiAsyncTag_WriteHDBFile,
    RFLiAsyncTag_WriteHDBFileClose,
    RFLiAsyncTag_ReadHDBFile,
    RFLiAsyncTag_ReadHDBFileClose,
    RFLiAsyncTag_CreateAsync,
    RFLiAsyncTag_OpenAsync,
    RFLiAsyncTag_SeekAsync,
    RFLiAsyncTag_ReadAsync,
    RFLiAsyncTag_WriteAsync,
    RFLiAsyncTag_CloseAsync,
    RFLiAsyncTag_GetLengthAsync,
    RFLiAsyncTag_DeleteAsync,
    RFLiAsyncTag_CreateDirAsync,

    RFLiAsyncTag_Max
} RFLiAsyncTag;

typedef enum {
    RFLiFileBrokenType_DBNotFound,
    RFLiFileBrokenType_DBBroken,
    RFLiFileBrokenType_ResBroken,
    RFLiFileBrokenType_Corrupt,

    RFLiFileBrokenType_Max
} RFLiFileBrokenType;

typedef enum {
    RFLiFileType_Database,
    RFLiFileType_Resource,

    RFLiFileType_Max
} RFLiFileType;

typedef enum {
    RFLiHiddenType_Any,
    RFLiHiddenType_No,
    RFLiHiddenType_Yes,
} RFLiHiddenType;

typedef enum {
    RFLiOpenType_Multi,
    RFLiOpenType_Single,
    RFLiOpenType_Opened
} RFLiOpenType;

typedef enum {
    RFLiPartsShp_Nose,
    RFLiPartsShp_Forehead,
    RFLiPartsShp_Faceline,
    RFLiPartsShp_Hair,
    RFLiPartsShp_Cap,
    RFLiPartsShp_Beard,
    RFLiPartsShp_Noseline,
    RFLiPartsShp_Mask,
    RFLiPartsShp_Glass,

    RFLiPartsShp_Max
} RFLiPartsShp;

typedef enum {
    RFLiPartsShpTex_Face,
    RFLiPartsShpTex_Cap,
    RFLiPartsShpTex_Noseline,
    RFLiPartsShpTex_Glass,

    RFLiPartsShpTex_Max
} RFLiPartsShpTex;

typedef enum {
    RFLiPartsTex_Eye,
    RFLiPartsTex_Eyebrow,
    RFLiPartsTex_Mouth,
    RFLiPartsTex_Mustache,
    RFLiPartsTex_Mole,

    RFLiPartsTex_Max
} RFLiPartsTex;

/**
 * Common typedefs
 */

typedef void (*RFLiCallback)(void);
typedef void (*RFLiExCallback)(u32 arg);

/**
 * Common structs
 */
typedef struct RFLiCharInfo {
    union {
        struct {
            u16 type : 3;
            u16 color : 3;
            u16 texture : 4;
            u16 padding : 6;
        };
        u16 rawdata;
    } faceline; // at 0x0

    union {
        struct {
            u16 type : 7;
            u16 color : 3;
            u16 flip : 1;
            u16 padding : 5;
        };
        u16 rawdata;
    } hair; // at 0x2

    union {
        struct {
            u32 type : 6;
            u32 color : 3;
            u32 scale : 4;
            u32 rotate : 5;
            u32 x : 4;
            u32 y : 5;
            u32 padding : 5;
        };
        u32 rawdata;
    } eye; // at 0x4

    union {
        struct {
            u32 type : 5;
            u32 color : 3;
            u32 scale : 4;
            u32 rotate : 5;
            u32 x : 4;
            u32 y : 5;
            u32 padding : 6;
        };
        u32 rawdata;
    } eyebrow; // at 0x8

    union {
        struct {
            u16 type : 4;
            u16 scale : 4;
            u16 y : 5;
            u16 padding : 3;
        };
        u16 rawdata;
    } nose; // at 0xC

    union {
        struct {
            u16 type : 5;
            u16 color : 2;
            u16 scale : 4;
            u16 y : 5;
        };
        u16 rawdata;
    } mouth; // at 0xE

    union {
        struct {
            u16 mustache : 2;
            u16 type : 2;
            u16 color : 3;
            u16 scale : 4;
            u16 y : 5;
        };
        u16 rawdata;
    } beard; // at 0x10

    union {
        struct {
            u16 type : 4;
            u16 color : 3;
            u16 scale : 4;
            u16 y : 5;
        };
        u16 rawdata;
    } glass; // at 0x12

    union {
        struct {
            u16 type : 1;
            u16 scale : 4;
            u16 x : 5;
            u16 y : 5;
            u16 padding : 1;
        };
        u16 rawdata;
    } mole; // at 0x14

    struct {
        u8 height;
        u8 build;
    } body; // at 0x16

    struct {
        wchar_t name[RFL_NAME_LEN + 1];       // at 0x18
        wchar_t creator[RFL_CREATOR_LEN + 1]; // at 0x2E

        // at 0x44
        u16 sex : 1;
        u16 bmonth : 4;
        u16 bday : 5;
        u16 color : 4;
        u16 favorite : 1;
        u16 localOnly : 1;
    } personal; // at 0x18

    RFLCreateID createID; // at 0x46
} RFLiCharInfo;

typedef struct RFLiCharData {
    // at 0x0
    u16 padding0 : 1;
    u16 sex : 1;
    u16 birthMonth : 4;
    u16 birthDay : 5;
    u16 favoriteColor : 4;
    u16 favorite : 1;

    wchar_t name[RFL_NAME_LEN]; // at 0x2
    u8 height;                  // at 0x16
    u8 build;                   // at 0x17
    RFLCreateID createID;       // at 0x18

    // at 0x20
    u16 faceType : 3;
    u16 faceColor : 3;
    u16 faceTex : 4;
    u16 padding2 : 3;
    u16 localonly : 1;
    u16 type : 2;

    // at 0x22
    u16 hairType : 7;
    u16 hairColor : 3;
    u16 hairFlip : 1;
    u16 padding3 : 5;

    // at 0x24
    u16 eyebrowType : 5;
    u16 eyebrowRotate : 5;
    u16 padding4 : 6;

    // at 0x26
    u16 eyebrowColor : 3;
    u16 eyebrowScale : 4;
    u16 eyebrowY : 5;
    u16 eyebrowX : 4;

    // at 0x28
    u16 eyeType : 6;
    u16 eyeRotate : 5;
    u16 eyeY : 5;

    // at 0x2A
    u16 eyeColor : 3;
    u16 eyeScale : 4;
    u16 eyeX : 4;
    u16 padding5 : 5;

    // at 0x2C
    u16 noseType : 4;
    u16 noseScale : 4;
    u16 noseY : 5;
    u16 padding6 : 3;

    // at 0x2E
    u16 mouthType : 5;
    u16 mouthColor : 2;
    u16 mouthScale : 4;
    u16 mouthY : 5;

    // at 0x30
    u16 glassType : 4;
    u16 glassColor : 3;
    u16 glassScale : 4;
    u16 glassY : 5;

    // at 0x32
    u16 mustacheType : 2;
    u16 beardType : 2;
    u16 beardColor : 3;
    u16 beardScale : 4;
    u16 beardY : 5;

    // at 0x34
    u16 moleType : 1;
    u16 moleScale : 4;
    u16 moleY : 5;
    u16 moleX : 5;
    u16 padding8 : 1;

    wchar_t creatorName[RFL_CREATOR_LEN]; // at 0x36
} RFLiCharData;

typedef struct RFLiStoreData {
    RFLiCharData data; // at 0x0
    u16 checksum;      // at 0x4A
} RFLiStoreData;

typedef struct RFLiHiddenCharData {
    // at 0x0
    u16 padding0 : 1;
    u16 sex : 1;
    u16 birthPadding : 9;
    u16 favoriteColor : 4;
    u16 favorite : 1;

    wchar_t name[RFL_NAME_LEN]; // at 0x2
    u8 height;                  // at 0x16
    u8 build;                   // at 0x17
    RFLCreateID createID;       // at 0x18

    // at 0x20
    u16 faceType : 3;
    u16 faceColor : 3;
    u16 faceTex : 4;
    u16 padding2 : 3;
    u16 localonly : 1;
    u16 type : 2;

    // at 0x22
    u16 hairType : 7;
    u16 hairColor : 3;
    u16 hairFlip : 1;
    u16 padding3 : 5;

    // at 0x24
    u16 eyebrowType : 5;
    u16 eyebrowRotate : 5;
    u16 padding4 : 6;

    // at 0x26
    u16 eyebrowColor : 3;
    u16 eyebrowScale : 4;
    u16 eyebrowY : 5;
    u16 eyebrowX : 4;

    // at 0x28
    u16 eyeType : 6;
    u16 eyeRotate : 5;
    u16 eyeY : 5;

    // at 0x2A
    u16 eyeColor : 3;
    u16 eyeScale : 4;
    u16 eyeX : 4;
    u16 padding5 : 5;

    // at 0x2C
    u16 noseType : 4;
    u16 noseScale : 4;
    u16 noseY : 5;
    u16 padding6 : 3;

    // at 0x2E
    u16 mouthType : 5;
    u16 mouthColor : 2;
    u16 mouthScale : 4;
    u16 mouthY : 5;

    // at 0x30
    u16 glassType : 4;
    u16 glassColor : 3;
    u16 glassScale : 4;
    u16 glassY : 5;

    // at 0x32
    u16 mustacheType : 2;
    u16 beardType : 2;
    u16 beardColor : 3;
    u16 beardScale : 4;
    u16 beardY : 5;

    // at 0x34
    u16 moleType : 1;
    u16 moleScale : 4;
    u16 moleY : 5;
    u16 moleX : 5;
    u16 padding8 : 1;

    char padding9[10]; // at 0x36
} RFLiHiddenCharData;

#ifdef __cplusplus
}
#endif
#endif
