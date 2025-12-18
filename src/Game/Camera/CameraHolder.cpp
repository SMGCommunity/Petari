#include "Game/Camera/CameraHolder.hpp"
#include "Game/Camera/Camera.hpp"
#include "Game/Camera/CameraAnim.hpp"
#include "Game/Camera/CameraBehind.hpp"
#include "Game/Camera/CameraBlackHole.hpp"
#include "Game/Camera/CameraCharmedFix.hpp"
#include "Game/Camera/CameraCharmedTripodBoss.hpp"
#include "Game/Camera/CameraCharmedVecReg.hpp"
#include "Game/Camera/CameraCharmedVecRegTower.hpp"
#include "Game/Camera/CameraCubePlanet.hpp"
#include "Game/Camera/CameraDPD.hpp"
#include "Game/Camera/CameraDead.hpp"
#include "Game/Camera/CameraFix.hpp"
#include "Game/Camera/CameraFixedPoint.hpp"
#include "Game/Camera/CameraFixedThere.hpp"
#include "Game/Camera/CameraFollow.hpp"
#include "Game/Camera/CameraFooFighter.hpp"
#include "Game/Camera/CameraFooFighterPlanet.hpp"
#include "Game/Camera/CameraFrontAndBack.hpp"
#include "Game/Camera/CameraGround.hpp"
#include "Game/Camera/CameraInnerCylinder.hpp"
#include "Game/Camera/CameraInwardSphere.hpp"
#include "Game/Camera/CameraInwardTower.hpp"
#include "Game/Camera/CameraMedianPlanet.hpp"
#include "Game/Camera/CameraMedianTower.hpp"
#include "Game/Camera/CameraMtxRegParallel.hpp"
#include "Game/Camera/CameraObjParallel.hpp"
#include "Game/Camera/CameraParallel.hpp"
#include "Game/Camera/CameraRaceFollow.hpp"
#include "Game/Camera/CameraRailDemo.hpp"
#include "Game/Camera/CameraRailFollow.hpp"
#include "Game/Camera/CameraRailWatch.hpp"
#include "Game/Camera/CameraSlide.hpp"
#include "Game/Camera/CameraSpiral.hpp"
#include "Game/Camera/CameraSubjective.hpp"
#include "Game/Camera/CameraTalk.hpp"
#include "Game/Camera/CameraTower.hpp"
#include "Game/Camera/CameraTowerPos.hpp"
#include "Game/Camera/CameraTripodBoss.hpp"
#include "Game/Camera/CameraTripodBossJoint.hpp"
#include "Game/Camera/CameraTripodPlanet.hpp"
#include "Game/Camera/CameraTrundle.hpp"
#include "Game/Camera/CameraTwistedPassage.hpp"
#include "Game/Camera/CameraWaterFollow.hpp"
#include "Game/Camera/CameraWaterPlanet.hpp"
#include "Game/Camera/CameraWaterPlanetBoss.hpp"
#include "Game/Camera/CameraWonderPlanet.hpp"
#include <cstring>

struct CameraTableEntry {
    /* 0x00 */ const char* mName;
    /* 0x04 */ const char* mExplain;
    /* 0x08 */ Camera* (*mCreateFunc)(void);
    /* 0x0C */ bool mIsPublic;
};

namespace {
    template < typename T >
    Camera* createCamera() {
        return new T();
    }

    static const CameraTableEntry sCameraTable[] = {
        {"CAM_TYPE_XZ_PARA", "並行", createCamera< CameraParallel >, true},
        {"CAM_TYPE_TOWER", "塔", createCamera< CameraTower >, true},
        {"CAM_TYPE_FOLLOW", "フォロー", createCamera< CameraFollow >, true},
        {"CAM_TYPE_WONDER_PLANET", "プラネット", createCamera< CameraWonderPlanet >, true},
        {"CAM_TYPE_POINT_FIX", "完全固定", createCamera< CameraFix >, true},
        {"CAM_TYPE_EYEPOS_FIX", "定点", createCamera< CameraFixedPoint >, true},
        {"CAM_TYPE_SLIDER", "スライダー", createCamera< CameraBehind >, true},
        {"CAM_TYPE_INWARD_TOWER", "塔内部", createCamera< CameraInwardTower >, true},
        {"CAM_TYPE_EYEPOS_FIX_THERE", "その場定点", createCamera< CameraFixedThere >, true},
        {"CAM_TYPE_TRIPOD_BOSS", "三脚ボス", createCamera< CameraTripodBoss >, true},
        {"CAM_TYPE_TOWER_POS", "塔（サブターゲット付き）", createCamera< CameraTowerPos >, true},
        {"CAM_TYPE_TRIPOD_PLANET", "三脚惑星", createCamera< CameraTripodPlanet >, true},
        {"CAM_TYPE_DEAD", "通常死亡", createCamera< CameraDead >, true},
        {"CAM_TYPE_INWARD_SPHERE", "球内部", createCamera< CameraInwardSphere >, true},
        {"CAM_TYPE_RAIL_DEMO", "レールデモ", createCamera< CameraRailDemo >, true},
        {"CAM_TYPE_RAIL_FOLLOW", "レールフォロー", createCamera< CameraRailFollow >, true},
        {"CAM_TYPE_TRIPOD_BOSS_JOINT", "三脚ボスジョイント", createCamera< CameraTripodBossJoint >, true},
        {"CAM_TYPE_CHARMED_TRIPOD_BOSS", "三脚ボスジョイント注視", createCamera< CameraCharmedTripodBoss >, true},
        {"CAM_TYPE_OBJ_PARALLEL", "オブジェ並行", createCamera< CameraObjParallel >, true},
        {"CAM_TYPE_CHARMED_FIX", "サンボ", createCamera< CameraCharmedFix >, true},
        {"CAM_TYPE_GROUND", "地面", createCamera< CameraGround >, true},
        {"CAM_TYPE_TRUNDLE", "トランドル", createCamera< CameraTrundle >, true},
        {"CAM_TYPE_CUBE_PLANET", "キューブ惑星", createCamera< CameraCubePlanet >, true},
        {"CAM_TYPE_INNER_CYLINDER", "円筒内部", createCamera< CameraInnerCylinder >, true},
        {"CAM_TYPE_SPIRAL_DEMO", "螺旋デモ", createCamera< CameraSpiral >, true},
        {"CAM_TYPE_TALK", "会話", createCamera< CameraTalk >, true},
        {"CAM_TYPE_MTXREG_PARALLEL", "マトリクスレジスタ並行", createCamera< CameraMtxRegParallel >, true},
        {"CAM_TYPE_CHARMED_VECREG", "ベクトルレジスタ注目", createCamera< CameraCharmedVecReg >, true},
        {"CAM_TYPE_MEDIAN_PLANET", "中点注目プラネット", createCamera< CameraMedianPlanet >, true},
        {"CAM_TYPE_TWISTED_PASSAGE", "ねじれ回廊", createCamera< CameraTwistedPassage >, true},
        {"CAM_TYPE_MEDIAN_TOWER", "中点塔カメラ", createCamera< CameraMedianTower >, true},
        {"CAM_TYPE_CHARMED_VECREG_TOWER", "VecReg角度補正塔カメラ", createCamera< CameraCharmedVecRegTower >, true},
        {"CAM_TYPE_FRONT_AND_BACK", "表裏カメラ", createCamera< CameraFrontAndBack >, true},
        {"CAM_TYPE_RACE_FOLLOW", "レース用フォロー", createCamera< CameraRaceFollow >, true},
        {"CAM_TYPE_2D_SLIDE", "２Ｄスライド", createCamera< CameraSlide >, true},
        {"CAM_TYPE_FOO_FIGHTER", "フーファイター", createCamera< CameraFooFighter >, true},
        {"CAM_TYPE_FOO_FIGHTER_PLANET", "フーファイタープラネット", createCamera< CameraFooFighterPlanet >, true},
        {"CAM_TYPE_BLACK_HOLE", "ブラックホール", createCamera< CameraBlackHole >, false},
        {"CAM_TYPE_ANIM", "アニメ", createCamera< CameraAnim >, false},
        {"CAM_TYPE_DPD", "ＤＰＤ", createCamera< CameraDPD >, true},
        {"CAM_TYPE_WATER_FOLLOW", "水中フォロー", createCamera< CameraWaterFollow >, true},
        {"CAM_TYPE_WATER_PLANET", "水中プラネット", createCamera< CameraWaterPlanet >, true},
        {"CAM_TYPE_WATER_PLANET_BOSS", "水中プラネットボス", createCamera< CameraWaterPlanetBoss >, true},
        {"CAM_TYPE_RAIL_WATCH", "レール注目", createCamera< CameraRailWatch >, true},
        {"CAM_TYPE_SUBJECTIVE", "主観", createCamera< CameraSubjective >, true},
    };
    static const char* sDefaultCamera = "CAM_TYPE_XZ_PARA";
};  // namespace

CameraHolder::CameraHolder(const char* pName) : NameObj(pName) {
    createCameras();

    mDefaultCameraIndex = getIndexOf(sDefaultCamera);
    mDefaultCamera = getDefaultCamera();
}

s32 CameraHolder::getNum() const {
    return sizeof(sCameraTable) / sizeof(*sCameraTable);
}

CamTranslatorBase* CameraHolder::getTranslator(s32 index) {
    return mTranslators[index];
}

s32 CameraHolder::getIndexOf(const char* pName) const {
    for (s32 i = 0; i < getNum(); i++) {
        if (strcmp(pName, ::sCameraTable[i].mName) == 0) {
            return i;
        }
    }

    return -1;
}

Camera* CameraHolder::getDefaultCamera() {
    return mCameras[mDefaultCameraIndex];
}

s32 CameraHolder::getIndexOfDefault() const {
    return mDefaultCameraIndex;
}

s32 CameraHolder::getIndexOf(Camera* pCamera) const {
    for (s32 i = 0; i < getNum(); i++) {
        if (mCameras[i] == pCamera) {
            return i;
        }
    }

    return -1;
}

void CameraHolder::createCameras() {
    mCameras = new Camera*[getNum()];
    mTranslators = new CamTranslatorBase*[getNum()];

    for (s32 i = 0; i < getNum(); i++) {
        mCameras[i] = sCameraTable[i].mCreateFunc();
        mTranslators[i] = mCameras[i]->createTranslator();
    }
}

Camera* CameraHolder::getCameraInner(s32 index) const {
    return mCameras[index];
}