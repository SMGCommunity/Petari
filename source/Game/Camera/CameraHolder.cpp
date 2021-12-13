#include "Game/Camera/Camera.h"
#include "Game/Camera/CameraHolder.h"
#include <string.h>

struct CameraTableEntry {
    const char *mName;                  // _0
    const char *mJapaneseDescription;   // _4
    Camera* (*mCreateFunction)(void);   // _8
    u8 _C;
    u8 _D[3];
};

#define NR_CAMERA_TYPES 45

const char *sDefaultCameraName = "CAM_TYPE_XZ_PARA";

// TODO: add function pointers (camera classes need to be created first)
CameraTableEntry sCameraTable[NR_CAMERA_TYPES] = {
    { "CAM_TYPE_XZ_PARA",               "並行",                       NULL, 1 },
    { "CAM_TYPE_TOWER",                 "塔",                         NULL, 1 },
    { "CAM_TYPE_FOLLOW",                "フォロー",                    NULL, 1 },
    { "CAM_TYPE_WONDER_PLANET",         "プラネット",                  NULL, 1 },
    { "CAM_TYPE_POINT_FIX",             "完全固定",                    NULL, 1 },
    { "CAM_TYPE_EYEPOS_FIX",            "定点",                       NULL, 1 },
    { "CAM_TYPE_SLIDER",                "スライダー",                  NULL, 1 },
    { "CAM_TYPE_INWARD_TOWER",          "塔内部",                      NULL, 1 },
    { "CAM_TYPE_EYEPOS_FIX_THER",       "その場定点",                  NULL, 1 },
    { "CAM_TYPE_TRIPOD_BOSS",           "三脚ボス",                    NULL, 1 },
    { "CAM_TYPE_TOWER_POS",             "塔（サブターゲット付き）",     NULL, 1 },
    { "CAM_TYPE_TRIPOD_PLANET",         "三脚惑星",                    NULL, 1 },
    { "CAM_TYPE_DEAD",                  "通常死亡",                    NULL, 1 },
    { "CAM_TYPE_INWARD_SPHERE",         "球内部",                      NULL, 1 },
    { "CAM_TYPE_RAIL_DEMO",             "レールデモ",                  NULL, 1 },
    { "CAM_TYPE_RAIL_FOLLOW",           "レールフォロー",              NULL, 1 },
    { "CAM_TYPE_TRIPOD_BOSS_JOINT",     "三脚ボスジョイント",           NULL, 1 },
    { "CAM_TYPE_CHARMED_TRIPOD_BOSS",   "三脚ボスジョイント注視",       NULL, 1 },
    { "CAM_TYPE_OBJ_PARALLEL",          "オブジェ並行",                NULL, 1 },
    { "CAM_TYPE_CHARMED_FIX",           "サンボ",                      NULL, 1 },
    { "CAM_TYPE_GROUND",                "地面",                        NULL, 1 },
    { "CAM_TYPE_TRUNDLE",               "トランドル",                  NULL, 1 },
    { "CAM_TYPE_CUBE_PLANET",           "キューブ惑星",                NULL, 1 },
    { "CAM_TYPE_INNER_CYLINDER",        "円筒内部",                    NULL, 1 },
    { "CAM_TYPE_SPIRAL_DEMO",           "螺旋デモ",                    NULL, 1 },
    { "CAM_TYPE_TALK",                  "会話",                       NULL, 1 },
    { "CAM_TYPE_MTXREG_PARALLEL",       "マトリクスレジスタ並行",       NULL, 1 },
    { "CAM_TYPE_CHARMED_VECREG",        "ベクトルレジスタ注目",         NULL, 1 },
    { "CAM_TYPE_MEDIAN_PLANET",         "中点注目プラネット",           NULL, 1 },
    { "CAM_TYPE_TWISTED_PASSAGE",       "ねじれ回廊",                  NULL, 1 },
    { "CAM_TYPE_MEDIAN_TOWER",          "中点塔カメラ",                NULL, 1 },
    { "CAM_TYPE_CHARMED_VECREG_TOWER",  "VecReg角度補正塔カメラ",       NULL, 1 },
    { "CAM_TYPE_FRONT_AND_BACK",        "表裏カメラ",                  NULL, 1 },
    { "CAM_TYPE_RACE_FOLLOW",           "レース用フォロー",             NULL, 1 },
    { "CAM_TYPE_2D_SLIDE",              "２Ｄスライド",                NULL, 1 },
    { "CAM_TYPE_FOO_FIGHTER",           "フーファイター",              NULL, 1 },
    { "CAM_TYPE_FOO_FIGHTER_PLANET",    "フーファイタープラネット",     NULL, 1 },
    { "CAM_TYPE_BLACK_HOLE",            "ブラックホール",              NULL, 0 },
    { "CAM_TYPE_ANIM",                  "アニメ",                     NULL, 0 },
    { "CAM_TYPE_DPD",                   "ＤＰＤ",                     NULL, 1 },
    { "CAM_TYPE_WATER_FOLLOW",          "水中フォロー",                NULL, 1 },
    { "CAM_TYPE_WATER_PLANET",          "水中プラネット",              NULL, 1 },
    { "CAM_TYPE_WATER_PLANET_BOSS",     "水中プラネットボス",          NULL, 1 },
    { "CAM_TYPE_RAIL_WATCH",            "レール注目",                 NULL, 1 },
    { "CAM_TYPE_SUBJECTIVE",            "主観",                       NULL, 1 }
};

CameraHolder::CameraHolder(const char *pName) : NameObj(pName) {
    createCameras();
    mDefaultCameraIndex = getIndexOf(sDefaultCameraName);
    mDefaultFrom14 = _14[mDefaultCameraIndex];
}

CameraHolder::~CameraHolder() {

}

s32 CameraHolder::getIndexOf(const char *pName) const {
    for (s32 i = 0; i < NR_CAMERA_TYPES; i++) {
        if (strcmp(pName, sCameraTable[i].mName) == 0) {
            return i;
        }
    }

    return -1;
}

Camera *CameraHolder::getDefaultCamera() {
    return mCameras[mDefaultCameraIndex];
}

s32 CameraHolder::getIndexOfDefault() const {
    return mDefaultCameraIndex;
}

s32 CameraHolder::getIndexOf(Camera *pCamera) const {
    for (s32 i = 0; i < NR_CAMERA_TYPES; i++) {
        if (mCameras[i] == pCamera) {
            return i;
        }
    }

    return -1;
}

/*void CameraHolder::createCameras() {
    mCameras = new Camera *[NR_CAMERA_TYPES];
    _14 = new void *[NR_CAMERA_TYPES];

    for (s32 i = 0; i < NR_CAMERA_TYPES; i++) {
        Camera *camera = sCameraTable[i].mCreateFunction();
        mCameras[i] = camera;
        //_14[i] = camera->Fun80093d04();
    }
}*/

Camera *CameraHolder::getCameraInner(long index) const {
    return mCameras[index];
}