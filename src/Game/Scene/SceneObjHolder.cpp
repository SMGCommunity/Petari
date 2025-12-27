#include "Game/AreaObj/AreaObjContainer.hpp"
#include "Game/Boss/BossAccessor.hpp"
#include "Game/Boss/SkeletalFishBabyRailHolder.hpp"
#include "Game/Boss/SkeletalFishBossRailHolder.hpp"
#include "Game/Boss/TripodBossAccesser.hpp"
#include "Game/Camera/CameraContext.hpp"
#include "Game/Camera/CameraDirector.hpp"
#include "Game/Demo/DemoDirector.hpp"
#include "Game/Demo/PrologueDirector.hpp"
#include "Game/Effect/EffectSystem.hpp"
// #include "Game/Enemy/BegomanBase.hpp"
// #include "Game/Enemy/KabokuriFireHolder.hpp"
#include "Game/Enemy/KameckBeamHolder.hpp"
#include "Game/Enemy/KarikariDirector.hpp"
// #include "Game/Enemy/TakoHeiInkHolder.hpp"
// #include "Game/GameAudio/AudBgmConductor.hpp"
// #include "Game/GameAudio/AudCameraWatcher.hpp"
#include "Game/GameAudio/AudEffectDirector.hpp"
#include "Game/Gravity/PlanetGravityManager.hpp"
#include "Game/LiveActor/AllLiveActorGroup.hpp"
#include "Game/LiveActor/ClippingDirector.hpp"
#include "Game/LiveActor/LiveActorGroupArray.hpp"
#include "Game/LiveActor/MessageSensorHolder.hpp"
#include "Game/LiveActor/MirrorCamera.hpp"
#include "Game/LiveActor/SensorHitChecker.hpp"
#include "Game/LiveActor/ShadowController.hpp"
#include "Game/LiveActor/ShadowSurfaceDrawer.hpp"
#include "Game/LiveActor/ShadowVolumeDrawer.hpp"
#include "Game/LiveActor/VolumeModelDrawer.hpp"
#include "Game/Map/Air.hpp"
#include "Game/Map/CollisionDirector.hpp"
#include "Game/Map/LightDirector.hpp"
#include "Game/Map/NamePosHolder.hpp"
// #include "Game/Map/OceanHomeMapCtrl.hpp"
#include "Game/Map/PlanetMapCreator.hpp"
#include "Game/Map/QuakeEffectGenerator.hpp"
#include "Game/Map/RaceManager.hpp"
#include "Game/Map/SleepControllerHolder.hpp"
#include "Game/Map/SphereSelector.hpp"
#include "Game/Map/StageSwitch.hpp"
#include "Game/Map/SunshadeMapHolder.hpp"
#include "Game/Map/SwitchWatcherHolder.hpp"
#include "Game/Map/WaterAreaHolder.hpp"
// #include "Game/Map/WaterPlant.hpp"
#include "Game/MapObj/AirBubbleHolder.hpp"
#include "Game/MapObj/ArrowSwitchMultiHolder.hpp"
#include "Game/MapObj/BigBubbleHolder.hpp"
#include "Game/MapObj/BigFanHolder.hpp"
#include "Game/MapObj/ChipHolder.hpp"
// #include "Game/MapObj/ClipAreaDropHolder.hpp"
#include "Game/MapObj/ClipAreaHolder.hpp"
// #include "Game/MapObj/ClipFieldFillDraw.hpp"
#include "Game/MapObj/CoinHolder.hpp"
#include "Game/MapObj/CoinRotater.hpp"
#include "Game/MapObj/EarthenPipe.hpp"
// #include "Game/MapObj/ElectricRailHolder.hpp"
#include "Game/MapObj/FallOutFieldDraw.hpp"
#include "Game/MapObj/FirePressureBulletHolder.hpp"
#include "Game/MapObj/GCapture.hpp"
#include "Game/MapObj/MapPartsRailGuideHolder.hpp"
#include "Game/MapObj/MiniatureGalaxyHolder.hpp"
#include "Game/MapObj/Note.hpp"
#include "Game/MapObj/PurpleCoinHolder.hpp"
#include "Game/MapObj/SpiderThread.hpp"
#include "Game/MapObj/SpinDriverPathDrawer.hpp"
#include "Game/MapObj/StarPieceDirector.hpp"
// #include "Game/MapObj/WarpPod.hpp"
#include "Game/MapObj/WaterPressureBulletHolder.hpp"
#include "Game/NPC/EventDirector.hpp"
#include "Game/NPC/MiiFaceIconHolder.hpp"
#include "Game/NPC/MiiFacePartsHolder.hpp"
// #include "Game/NPC/NPCDirector.hpp"
#include "Game/NPC/TalkDirector.hpp"
#include "Game/NameObj/MovementOnOffGroupHolder.hpp"
#include "Game/NameObj/NameObj.hpp"
#include "Game/NameObj/NameObjExecuteHolder.hpp"
#include "Game/NameObj/NameObjGroup.hpp"
#include "Game/Player/GroupChecker.hpp"
#include "Game/Player/MarioHolder.hpp"
// #include "Game/Player/PlayerEvent.hpp"
// #include "Game/Ride/FluffWind.hpp"
// #include "Game/Ride/PlantLeaf.hpp"
// #include "Game/Ride/PlantStalk.hpp"
// #include "Game/Ride/SwingRope.hpp"
// #include "Game/Ride/Trapeze.hpp"
#include "Game/Scene/PlacementStateChecker.hpp"
#include "Game/Scene/SceneDataInitializer.hpp"
#include "Game/Scene/SceneNameObjMovementController.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Scene/ScenePlayingResult.hpp"
#include "Game/Scene/StageDataHolder.hpp"
#include "Game/Scene/StopSceneController.hpp"
#include "Game/Screen/BloomEffect.hpp"
#include "Game/Screen/BloomEffectSimple.hpp"
#include "Game/Screen/CaptureScreenDirector.hpp"
#include "Game/Screen/CenterScreenBlur.hpp"
#include "Game/Screen/CinemaFrame.hpp"
#include "Game/Screen/CometRetryButton.hpp"
#include "Game/Screen/DepthOfFieldBlur.hpp"
#include "Game/Screen/GalaxyMapController.hpp"
#include "Game/Screen/GalaxyNamePlateDrawer.hpp"
#include "Game/Screen/GameSceneLayoutHolder.hpp"
// #include "Game/Screen/HeatHazeEffect.hpp"
#include "Game/Screen/ImageEffectSystemHolder.hpp"
#include "Game/Screen/InformationObserver.hpp"
#include "Game/Screen/LensFlare.hpp"
#include "Game/Screen/MoviePlayerSimple.hpp"
#include "Game/Screen/MoviePlayingSequence.hpp"
#include "Game/Screen/OdhConverter.hpp"
#include "Game/Screen/PlayerActionGuidance.hpp"
#include "Game/Screen/SceneWipeHolder.hpp"
#include "Game/Screen/ScreenAlphaCapture.hpp"
#include "Game/Screen/ScreenBlurEffect.hpp"
#include "Game/Screen/StaffRoll.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemSceneController.hpp"
#include "Game/Util/BaseMatrixFollowTargetHolder.hpp"
// #include "Game/Util/FurCtrl.hpp"
#include "Game/Util/SceneUtil.hpp"
// #include "Game/Util/ShareUtil.hpp"
#include "Game/SingletonHolder.hpp"

SceneObjHolder::SceneObjHolder() {
    for (int i = 0; i < SceneObj_COUNT; i++) {
        mObj[i] = nullptr;
    }
}

NameObj* SceneObjHolder::create(int id) {
    NameObj* pObj = mObj[id];

    if (pObj != nullptr) {
        return pObj;
    }

    pObj = newEachObj(id);
    pObj->initWithoutIter();

    mObj[id] = pObj;

    return pObj;
}

NameObj* SceneObjHolder::getObj(int id) const {
    return mObj[id];
}

bool SceneObjHolder::isExist(int id) const {
    return mObj[id] != nullptr;
}

NameObj* SceneObjHolder::newEachObj(int id) {
    switch (id) {
    case SceneObj_SensorHitChecker:
        return new SensorHitChecker("センサー当たり");
    case SceneObj_CollisionDirector:
        return new CollisionDirector();
    case SceneObj_ClippingDirector:
        return new ClippingDirector();
    case SceneObj_DemoDirector:
        return new DemoDirector("デモ指揮");
    case SceneObj_EventDirector:
        return new EventDirector();
    case SceneObj_EffectSystem:
        return new EffectSystem("エフェクトシステム", true);
    case SceneObj_LightDirector:
        return new LightDirector();
    case SceneObj_SceneDataInitializer:
        return new SceneDataInitializer();
    case SceneObj_StageDataHolder:
        return new StageDataHolder(MR::getCurrentStageName(), 0, true);
    case SceneObj_MessageSensorHolder:
        return new MessageSensorHolder("システム汎用センサー");
    case SceneObj_StageSwitchContainer:
        return new StageSwitchContainer();
    case SceneObj_SwitchWatcherHolder:
        return new SwitchWatcherHolder();
    case SceneObj_SleepControllerHolder:
        // return new SleepControllerHolder();
        return nullptr;
    case SceneObj_AreaObjContainer:
        return new AreaObjContainer("エリアオブジェクトコンテナ管理");
    case SceneObj_LiveActorGroupArray:
        return new LiveActorGroupArray("オブジェクトグループ");
    case SceneObj_MovementOnOffGroupHolder:
        return new MovementOnOffGroupHolder("Movementグループ管理");
    case SceneObj_CaptureScreenActor:
        return new CaptureScreenActor(45, "Indirect");
    case SceneObj_AudCameraWatcher:
        // return new AudCameraWatcher();
        return nullptr;
    case SceneObj_AudEffectDirector:
        return new AudEffectDirector();
    case SceneObj_AudBgmConductor:
        // return new AudBgmConductor();
        return nullptr;
    case SceneObj_MarioHolder:
        return new MarioHolder();
    case SceneObj_MirrorCamera:
        return new MirrorCamera("鏡用カメラ");
    case SceneObj_CameraContext:
        return new CameraContext();
    case SceneObj_NameObjGroup:
        return new NameObjGroup("IgnorePauseNameObj", 16);
    case SceneObj_TalkDirector:
        return new TalkDirector("会話ディレクター");
    case SceneObj_EventSequencer:
        // return new EventSequencer();
        return nullptr;
    case SceneObj_StopSceneController:
        return new StopSceneController();
    case SceneObj_SceneNameObjMovementController:
        return new SceneNameObjMovementController();
    case SceneObj_ImageEffectSystemHolder:
        return new ImageEffectSystemHolder();
    case SceneObj_BloomEffect:
        return new BloomEffect("ブルーム");
    case SceneObj_BloomEffectSimple:
        return new BloomEffectSimple();
    case SceneObj_ScreenBlurEffect:
        return new ScreenBlurEffect("画面ブラー");
    case SceneObj_DepthOfFieldBlur:
        return new DepthOfFieldBlur("被写界深度ブラー");
    case SceneObj_SceneWipeHolder:
        return new SceneWipeHolder();
    case SceneObj_PlayerActionGuidance:
        return new PlayerActionGuidance();
    case SceneObj_ScenePlayingResult:
        return new ScenePlayingResult();
    case SceneObj_LensFlareDirector:
        return new LensFlareDirector();
    case SceneObj_FurDrawManager:
        // return new FurDrawManager(64);
        return nullptr;
    case SceneObj_PlacementStateChecker:
        return new PlacementStateChecker("オブジェクト配置状態の監視");
    case SceneObj_NamePosHolder:
        return new NamePosHolder();
    case SceneObj_NPCDirector:
        // return new NPCDirector();
        return nullptr;
    case SceneObj_ResourceShare:
        // return new ResourceShare();
        return nullptr;
    case SceneObj_MoviePlayerSimple:
        return new MoviePlayerSimple();
    case SceneObj_WarpPodMgr:
        // return new WarpPodMgr("ワープポッド管理局");
        return nullptr;
    case SceneObj_CenterScreenBlur:
        return new CenterScreenBlur();
    case SceneObj_OdhConverter:
        return new OdhConverter();
    case SceneObj_CometRetryButton:
        return new CometRetryButton("コメットリトライボタン");
    case SceneObj_AllLiveActorGroup:
        return new AllLiveActorGroup();
    case SceneObj_CameraDirector:
        return new CameraDirector("カメラ管理");
    case SceneObj_PlanetGravityManager:
        return new PlanetGravityManager("重力");
    case SceneObj_BaseMatrixFollowTargetHolder:
        return new BaseMatrixFollowTargetHolder("行列追随先リスト", 256, 256);
    case SceneObj_GameSceneLayoutHolder:
        return new GameSceneLayoutHolder();
    case SceneObj_CoinHolder:
        return new CoinHolder("コイン管理");
    case SceneObj_PurpleCoinHolder:
        return new PurpleCoinHolder();
    case SceneObj_CoinRotater:
        return new CoinRotater("コイン回転管理");
    case SceneObj_AirBubbleHolder:
        return new AirBubbleHolder("空気アワ管理");
    case SceneObj_BigFanHolder:
        return new BigFanHolder();
    case SceneObj_KarikariDirector:
        // return new KarikariDirector("カリカリディレクター");
        return nullptr;
    case SceneObj_StarPieceDirector:
        // return new StarPieceDirector("スターピース指揮");
        return nullptr;
    case SceneObj_BegomanAttackPermitter:
        // return new BegomanAttackPermitter("ベーゴマン攻撃許可者");
        return nullptr;
    case SceneObj_TripodBossAccesser:
        return new TripodBossAccesser("三脚ボスアクセサ");
    case SceneObj_KameckBeamHolder:
        // return new KameckBeamHolder();
        return nullptr;
    case SceneObj_KameckFireBallHolder:
        // return new KameckFireBallHolder();
        return nullptr;
    case SceneObj_KameckBeamTurtleHolder:
        // return new KameckBeamTurtleHolder();
        return nullptr;
    case SceneObj_KabokuriFireHolder:
        // return new KabokuriFireHolder();
        return nullptr;
    case SceneObj_TakoHeiInkHolder:
        // return new TakoHeiInkHolder();
        return nullptr;
    case SceneObj_ShadowControllerHolder:
        return new ShadowControllerHolder();
    case SceneObj_ShadowVolumeDrawInit:
        return new ShadowVolumeDrawInit();
    case SceneObj_ShadowSurfaceDrawInit:
        return new ShadowSurfaceDrawInit("水面影描画初期化");
    case SceneObj_SwingRopeGroup:
        // return new SwingRopeGroup("スイングロープ描画");
        return nullptr;
    case SceneObj_PlantStalkDrawInit:
        // return new PlantStalkDrawInit("植物の茎描画初期化");
        return nullptr;
    case SceneObj_PlantLeafDrawInit:
        // return new PlantLeafDrawInit("描画初期化[植物の葉]");
        return nullptr;
    case SceneObj_TrapezeRopeDrawInit:
        // return new TrapezeRopeDrawInit("空中ブランコロープ描画");
    case SceneObj_VolumeModelDrawInit:
        return new VolumeModelDrawInit();
    case SceneObj_SpinDriverPathDrawInit:
        return new SpinDriverPathDrawInit();
    case SceneObj_NoteGroup:
        return new NoteGroup();
    case SceneObj_ClipAreaDropHolder:
        // return new ClipAreaDropHolder();
        return nullptr;
    case SceneObj_FallOutFieldDraw:
        return new FallOutFieldDraw("クリップエリア描画[抜き]");
    case SceneObj_ClipFieldFillDraw:
        // return new ClipFieldFillDraw("クリップエリア描画[塗りつぶし]");
        return nullptr;
    case SceneObj_ClipAreaHolder:
        return new ClipAreaHolder("クリップエリアホルダー");
    case SceneObj_ArrowSwitchMultiHolder:
        return new ArrowSwitchMultiHolder();
    case SceneObj_ScreenAlphaCapture:
        // return new ScreenAlphaCapture("アルファテクスチャ取り込み");
        return nullptr;
    case SceneObj_MapPartsRailGuideHolder:
        return new MapPartsRailGuideHolder();
    case SceneObj_GCapture:
        return new GCapture("Gキャプチャー");
    case SceneObj_NameObjExecuteHolder:
        return new NameObjExecuteHolder(4096);
    case SceneObj_ElectricRailHolder:
        // return new ElectricRailHolder("電撃レール保持");
        return nullptr;
    case SceneObj_SpiderThread:
        // return new SpiderThread("クモの巣");
        return nullptr;
    case SceneObj_QuakeEffectGenerator:
        return new QuakeEffectGenerator();
    case SceneObj_HeatHazeDirector:
        // return new HeatHazeDirector("陽炎制御");
        return nullptr;
    case SceneObj_BlueChipHolder:
        return new ChipHolder("ブルーチップホルダー", 0);
    case SceneObj_YellowChipHolder:
        return new ChipHolder("イエローーチップホルダー", 1);
    case SceneObj_BigBubbleHolder:
        // return new BigBubbleHolder("オオアワホルダー");
        return nullptr;
    case SceneObj_EarthenPipeMediator:
        return new EarthenPipeMediator();
    case SceneObj_WaterAreaHolder:
        // return new WaterAreaHolder();
        return nullptr;
    case SceneObj_WaterPlantDrawInit:
        // return new WaterPlantDrawInit();
        return nullptr;
    case SceneObj_OceanHomeMapCtrl:
        // return new OceanHomeMapCtrl();
        return nullptr;
    case SceneObj_RaceManager:
        return new RaceManager();
    case SceneObj_GroupCheckManager:
        return new GroupCheckManager("属性グループマネージャー");
    case SceneObj_SkeletalFishBabyRailHolder:
        return new SkeletalFishBabyRailHolder("スカルシャークベビーレール管理");
    case SceneObj_SkeletalFishBossRailHolder:
        return new SkeletalFishBossRailHolder("スカルシャークボスレール管理");
    case SceneObj_WaterPressureBulletHolder:
        return new WaterPressureBulletHolder("ウォータープレッシャー玉ホルダ−");
    case SceneObj_FirePressureBulletHolder:
        return new FirePressureBulletHolder("ファイアプレッシャー玉ホルダ−");
    case SceneObj_SunshadeMapHolder:
        return new SunshadeMapHolder();
    case SceneObj_MiiFacePartsHolder:
        return new MiiFacePartsHolder(128);
    case SceneObj_MiiFaceIconHolder:
        return new MiiFaceIconHolder(16, "Miiアイコン保持管理");
    case SceneObj_FluffWindHolder:
        // return new FluffWindHolder();
        return nullptr;
    case SceneObj_SphereSelector:
        return new SphereSelector();
    case SceneObj_GalaxyNamePlateDrawer:
        return new GalaxyNamePlateDrawer();
    case SceneObj_CinemaFrame:
        return new CinemaFrame(true);
    case SceneObj_BossAccessor:
        return new BossAccessor();
    case SceneObj_MiniatureGalaxyHolder:
        // return new MiniatureGalaxyHolder();
        return nullptr;
    case SceneObj_PlanetMapCreator:
        return new PlanetMapCreator("惑星クリエイタ");
    case SceneObj_PriorDrawAirHolder:
        return new PriorDrawAirHolder();
    case SceneObj_InformationObserver:
        return new InformationObserver();
    case SceneObj_GalaxyMapController:
        // return new GalaxyMapController();
        return nullptr;
    case SceneObj_MoviePlayingSequenceHolder:
        return new MoviePlayingSequenceHolder("ムービー管理保持");
    case SceneObj_PrologueHolder:
        return new PrologueHolder("プロローグ保持");
    case SceneObj_StaffRoll:
        return new StaffRoll("スタッフロール");
    default:
        return nullptr;
    }
}

namespace MR {
    NameObj* createSceneObj(int id) {
        return getSceneObjHolder()->create(id);
    }

    SceneObjHolder* getSceneObjHolder() {
        return SingletonHolder< GameSystem >::get()->mSceneController->getSceneObjHolder();
    }

    bool isExistSceneObj(int id) {
        GameSystemSceneController* pSceneController = SingletonHolder< GameSystem >::get()->mSceneController;

        if (pSceneController == nullptr) {
            return false;
        }

        if (!pSceneController->isExistSceneObjHolder()) {
            return false;
        }

        return MR::getSceneObjHolder()->isExist(id);
    }
};  // namespace MR
