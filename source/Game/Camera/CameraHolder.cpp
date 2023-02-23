#include "Game/Camera/Camera.h"
#include "Game/Camera/CameraAnim.h"
#include "Game/Camera/CameraBehind.h"
#include "Game/Camera/CameraBlackHole.h"
#include "Game/Camera/CameraCharmedFix.h"
#include "Game/Camera/CameraCharmedTripodBoss.h"
#include "Game/Camera/CameraCharmedVecReg.h"
#include "Game/Camera/CameraCharmedVecRegTower.h"
#include "Game/Camera/CameraCubePlanet.h"
#include "Game/Camera/CameraDPD.h"
#include "Game/Camera/CameraDead.h"
#include "Game/Camera/CameraFix.h"
#include "Game/Camera/CameraFixedPoint.h"
#include "Game/Camera/CameraFixedThere.h"
#include "Game/Camera/CameraFollow.h"
#include "Game/Camera/CameraFooFighter.h"
#include "Game/Camera/CameraFooFighterPlanet.h"
#include "Game/Camera/CameraFrontAndBack.h"
#include "Game/Camera/CameraGround.h"
#include "Game/Camera/CameraHolder.h"
#include "Game/Camera/CameraInnerCylinder.h"
#include "Game/Camera/CameraInwardSphere.h"
#include "Game/Camera/CameraInwardTower.h"
#include "Game/Camera/CameraMedianPlanet.h"
#include "Game/Camera/CameraMedianTower.h"
#include "Game/Camera/CameraMtxRegParallel.h"
#include "Game/Camera/CameraObjParallel.h"
#include "Game/Camera/CameraParallel.h"
#include "Game/Camera/CameraRaceFollow.h"
#include "Game/Camera/CameraRailDemo.h"
#include "Game/Camera/CameraRailFollow.h"
#include "Game/Camera/CameraRailWatch.h"
#include "Game/Camera/CameraSlide.h"
#include "Game/Camera/CameraSpiral.h"
#include "Game/Camera/CameraSubjective.h"
#include "Game/Camera/CameraTalk.h"
#include "Game/Camera/CameraTower.h"
#include "Game/Camera/CameraTowerPos.h"
#include "Game/Camera/CameraTripodBoss.h"
#include "Game/Camera/CameraTripodBossJoint.h"
#include "Game/Camera/CameraTripodPlanet.h"
#include "Game/Camera/CameraTrundle.h"
#include "Game/Camera/CameraTwistedPassage.h"
#include "Game/Camera/CameraWaterFollow.h"
#include "Game/Camera/CameraWaterPlanet.h"
#include "Game/Camera/CameraWaterPlanetBoss.h"
#include "Game/Camera/CameraWonderPlanet.h"
#include <cstring>

struct CameraTableEntry {
    const char *mName;                  // _0
    const char *mJapaneseDescription;   // _4
    Camera* (*mCreateFunction)(void);   // _8
    u8 _C;
    u8 _D[3];
};

namespace {
    const char *gDefaultCameraName = "CAM_TYPE_XZ_PARA";

    template<typename T>
    Camera *createCamera();

    template<>
    Camera *createCamera<CameraParallel>() {
        return new CameraParallel("平行カメラ");
    }

    template<>
    Camera *createCamera<CameraTower>() {
        return new CameraTower("塔カメラ");
    }

    template<>
    Camera *createCamera<CameraFollow>() {
        return new CameraFollow("フォローカメラ");
    }

    template<>
    Camera *createCamera<CameraWonderPlanet>() {
        return new CameraWonderPlanet("ワンダープラネットカメラ");
    }

    template<>
    Camera *createCamera<CameraFix>() {
        return new CameraFix("完全固定カメラ");
    }

    template<>
    Camera *createCamera<CameraFixedPoint>() {
        return new CameraFixedPoint("定点カメラ");
    }

    template<>
    Camera *createCamera<CameraBehind>() {
        return new CameraBehind("後方カメラ");
    }

    template<>
    Camera *createCamera<CameraInwardTower>() {
        return new CameraInwardTower("塔内部カメラ");
    }

    template<>
    Camera *createCamera<CameraFixedThere>() {
        return new CameraFixedThere("その場定点カメラ");
    }

    template<>
    Camera *createCamera<CameraTripodBoss>() {
        return new CameraTripodBoss("三脚ボスカメラ");
    }

    template<>
    Camera *createCamera<CameraTowerPos>() {
        return new CameraTowerPos("塔カメラ（サブターゲット付き）");
    }

    template<>
    Camera *createCamera<CameraTripodPlanet>() {
        return new CameraTripodPlanet("三脚惑星カメラ");
    }

    template<>
    Camera *createCamera<CameraDead>() {
        return new CameraDead("通常死亡カメラ");
    }

    template<>
    Camera *createCamera<CameraInwardSphere>() {
        return new CameraInwardSphere("球内部カメラ");
    }

    template<>
    Camera *createCamera<CameraRailDemo>() {
        return new CameraRailDemo("レールデモカメラ");
    }

    template<>
    Camera *createCamera<CameraRailFollow>() {
        return new CameraRailFollow("レールフォローカメラ");
    }

    template<>
    Camera *createCamera<CameraTripodBossJoint>() {
        return new CameraTripodBossJoint("三脚ボス関節カメラ");
    }

    template<>
    Camera *createCamera<CameraCharmedTripodBoss>() {
        return new CameraCharmedTripodBoss("三脚ボスジョイント注視カメラ");
    }

    template<>
    Camera *createCamera<CameraObjParallel>() {
        return new CameraObjParallel("オブジェ並行カメラ");
    }

    template<>
    Camera *createCamera<CameraCharmedFix>() {
        return new CameraCharmedFix("サンボカメラ");
    }

    template<>
    Camera *createCamera<CameraGround>() {
        return new CameraGround("地面カメラ");
    }

    template<>
    Camera *createCamera<CameraTrundle>() {
        return new CameraTrundle("トランドルカメラ");
    }

    template<>
    Camera *createCamera<CameraCubePlanet>() {
        return new CameraCubePlanet("キューブ惑星カメラ");
    }

    template<>
    Camera *createCamera<CameraInnerCylinder>() {
        return new CameraInnerCylinder("CameraInnerCylinder");
    }

    template<>
    Camera *createCamera<CameraSpiral>() {
        return new CameraSpiral("螺旋カメラ（デモ用）");
    }

    template<>
    Camera *createCamera<CameraTalk>() {
        return new CameraTalk("会話カメラ");
    }

    template<>
    Camera *createCamera<CameraMtxRegParallel>() {
        return new CameraMtxRegParallel("マトリクスレジスタ並行カメラ");
    }

    template<>
    Camera *createCamera<CameraCharmedVecReg>() {
        return new CameraCharmedVecReg("ベクトルレジスタ注目カメラ");
    }

    template<>
    Camera *createCamera<CameraMedianPlanet>() {
        return new CameraMedianPlanet("中点注目プラネットカメラ");
    }

    template<>
    Camera *createCamera<CameraTwistedPassage>() {
        return new CameraTwistedPassage("ねじれ回廊カメラ");
    }

    template<>
    Camera *createCamera<CameraMedianTower>() {
        return new CameraMedianTower("中点塔カメラ");
    }

    template<>
    Camera *createCamera<CameraCharmedVecRegTower>() {
        return new CameraCharmedVecRegTower("CameraCharmedVecRegTower");
    }

    template<>
    Camera *createCamera<CameraFrontAndBack>() {
        return new CameraFrontAndBack("表裏カメラ");
    }

    template<>
    Camera *createCamera<CameraRaceFollow>() {
        return new CameraRaceFollow("CameraRaceFollow");
    }

    template<>
    Camera *createCamera<CameraSlide>() {
        return new CameraSlide("スライドカメラ");
    }

    template<>
    Camera *createCamera<CameraFooFighter>() {
        return new CameraFooFighter("フーファイター");
    }

    template<>
    Camera *createCamera<CameraFooFighterPlanet>() {
        return new CameraFooFighterPlanet("フーファイタープラネットカメラ");
    }

    template<>
    Camera *createCamera<CameraBlackHole>() {
        return new CameraBlackHole("ブラックホールカメラ");
    }

    template<>
    Camera *createCamera<CameraAnim>() {
        return new CameraAnim("アニメーションカメラ");
    }

    template<>
    Camera *createCamera<CameraDPD>() {
        return new CameraDPD("DPD操作カメラ");
    }

    template<>
    Camera *createCamera<CameraWaterFollow>() {
        return new CameraWaterFollow("水中フォロー");
    }

    template<>
    Camera *createCamera<CameraWaterPlanet>() {
        return new CameraWaterPlanet("水中プラネット");
    }

    template<>
    Camera *createCamera<CameraWaterPlanetBoss>() {
        return new CameraWaterPlanetBoss("水中プラネットボス");
    }

    template<>
    Camera *createCamera<CameraRailWatch>() {
        return new CameraRailWatch("レール注目カメラ");
    }

    template<>
    Camera *createCamera<CameraSubjective>() {
        return new CameraSubjective("主観カメラ");
    }

    #define NR_CAMERA_TYPES 45

    CameraTableEntry gCameraTable[NR_CAMERA_TYPES] = {
        { "CAM_TYPE_XZ_PARA",               "並行",                       createCamera<CameraParallel>,           1 },
        { "CAM_TYPE_TOWER",                 "塔",                         createCamera<CameraTower>,              1 },
        { "CAM_TYPE_FOLLOW",                "フォロー",                    createCamera<CameraFollow>,             1 },
        { "CAM_TYPE_WONDER_PLANET",         "プラネット",                  createCamera<CameraWonderPlanet>,       1 },
        { "CAM_TYPE_POINT_FIX",             "完全固定",                    createCamera<CameraFix>,                1 },
        { "CAM_TYPE_EYEPOS_FIX",            "定点",                       createCamera<CameraFixedPoint>,         1 },
        { "CAM_TYPE_SLIDER",                "スライダー",                  createCamera<CameraBehind>,             1 },
        { "CAM_TYPE_INWARD_TOWER",          "塔内部",                      createCamera<CameraInwardTower>,        1 },
        { "CAM_TYPE_EYEPOS_FIX_THER",       "その場定点",                  createCamera<CameraFixedThere>,         1 },
        { "CAM_TYPE_TRIPOD_BOSS",           "三脚ボス",                    createCamera<CameraTripodBoss>,         1 },
        { "CAM_TYPE_TOWER_POS",             "塔（サブターゲット付き）",     createCamera<CameraTowerPos>,           1 },
        { "CAM_TYPE_TRIPOD_PLANET",         "三脚惑星",                    createCamera<CameraTripodPlanet>,       1 },
        { "CAM_TYPE_DEAD",                  "通常死亡",                    createCamera<CameraDead>,               1 },
        { "CAM_TYPE_INWARD_SPHERE",         "球内部",                      createCamera<CameraInwardSphere>,       1 },
        { "CAM_TYPE_RAIL_DEMO",             "レールデモ",                  createCamera<CameraRailDemo>,           1 },
        { "CAM_TYPE_RAIL_FOLLOW",           "レールフォロー",              createCamera<CameraRailFollow>,         1 },
        { "CAM_TYPE_TRIPOD_BOSS_JOINT",     "三脚ボスジョイント",           createCamera<CameraTripodBossJoint>,    1 },
        { "CAM_TYPE_CHARMED_TRIPOD_BOSS",   "三脚ボスジョイント注視",       createCamera<CameraCharmedTripodBoss>,  1 },
        { "CAM_TYPE_OBJ_PARALLEL",          "オブジェ並行",                createCamera<CameraObjParallel>,        1 },
        { "CAM_TYPE_CHARMED_FIX",           "サンボ",                      createCamera<CameraCharmedFix>,         1 },
        { "CAM_TYPE_GROUND",                "地面",                        createCamera<CameraGround>,             1 },
        { "CAM_TYPE_TRUNDLE",               "トランドル",                  createCamera<CameraTrundle>,            1 },
        { "CAM_TYPE_CUBE_PLANET",           "キューブ惑星",                createCamera<CameraCubePlanet>,         1 },
        { "CAM_TYPE_INNER_CYLINDER",        "円筒内部",                    createCamera<CameraInnerCylinder>,      1 },
        { "CAM_TYPE_SPIRAL_DEMO",           "螺旋デモ",                    createCamera<CameraSpiral>,             1 },
        { "CAM_TYPE_TALK",                  "会話",                       createCamera<CameraTalk>,               1 },
        { "CAM_TYPE_MTXREG_PARALLEL",       "マトリクスレジスタ並行",       createCamera<CameraMtxRegParallel>,     1 },
        { "CAM_TYPE_CHARMED_VECREG",        "ベクトルレジスタ注目",         createCamera<CameraCharmedVecReg>,      1 },
        { "CAM_TYPE_MEDIAN_PLANET",         "中点注目プラネット",           createCamera<CameraMedianPlanet>,       1 },
        { "CAM_TYPE_TWISTED_PASSAGE",       "ねじれ回廊",                  createCamera<CameraTwistedPassage>,     1 },
        { "CAM_TYPE_MEDIAN_TOWER",          "中点塔カメラ",                createCamera<CameraMedianTower>,        1 },
        { "CAM_TYPE_CHARMED_VECREG_TOWER",  "VecReg角度補正塔カメラ",       createCamera<CameraCharmedVecRegTower>, 1 },
        { "CAM_TYPE_FRONT_AND_BACK",        "表裏カメラ",                  createCamera<CameraFrontAndBack>,       1 },
        { "CAM_TYPE_RACE_FOLLOW",           "レース用フォロー",             createCamera<CameraRaceFollow>,         1 },
        { "CAM_TYPE_2D_SLIDE",              "２Ｄスライド",                createCamera<CameraSlide>,              1 },
        { "CAM_TYPE_FOO_FIGHTER",           "フーファイター",              createCamera<CameraFooFighter>,         1 },
        { "CAM_TYPE_FOO_FIGHTER_PLANET",    "フーファイタープラネット",     createCamera<CameraFooFighterPlanet>,   1 },
        { "CAM_TYPE_BLACK_HOLE",            "ブラックホール",              createCamera<CameraBlackHole>,          0 },
        { "CAM_TYPE_ANIM",                  "アニメ",                     createCamera<CameraAnim>,               0 },
        { "CAM_TYPE_DPD",                   "ＤＰＤ",                     createCamera<CameraDPD>,                1 },
        { "CAM_TYPE_WATER_FOLLOW",          "水中フォロー",                createCamera<CameraWaterFollow>,        1 },
        { "CAM_TYPE_WATER_PLANET",          "水中プラネット",              createCamera<CameraWaterPlanet>,        1 },
        { "CAM_TYPE_WATER_PLANET_BOSS",     "水中プラネットボス",          createCamera<CameraWaterPlanetBoss>,    1 },
        { "CAM_TYPE_RAIL_WATCH",            "レール注目",                 createCamera<CameraRailWatch>,          1 },
        { "CAM_TYPE_SUBJECTIVE",            "主観",                       createCamera<CameraSubjective>,         1 }
    };
}

CameraHolder::CameraHolder(const char *pName) : NameObj(pName) {
    createCameras();
    mDefaultCameraIndex = getIndexOf(gDefaultCameraName);
    mDefaultTranslator = mTranslators[mDefaultCameraIndex];
}

CameraHolder::~CameraHolder() {

}

CamTranslatorBase *CameraHolder::getTranslator(long index) {
    return mTranslators[index];
}

s32 CameraHolder::getIndexOf(const char *pName) const {
    for (s32 i = 0; i < NR_CAMERA_TYPES; i++) {
        if (strcmp(pName, gCameraTable[i].mName) == 0) {
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

void CameraHolder::createCameras() {
    mCameras = new Camera *[NR_CAMERA_TYPES];
    mTranslators = new CamTranslatorBase *[NR_CAMERA_TYPES];

    for (s32 i = 0; i < NR_CAMERA_TYPES; i++) {
        mCameras[i] = gCameraTable[i].mCreateFunction();
        mTranslators[i] = mCameras[i]->createTranslator();
    }
}

Camera *CameraHolder::getCameraInner(long index) const {
    return mCameras[index];
}