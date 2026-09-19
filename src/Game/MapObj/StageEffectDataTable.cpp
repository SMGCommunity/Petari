#include "Game/MapObj/StageEffectDataTable.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <revolution/types.h>

void StageEffectDataTable_FORCE_MATCH_SDATA2() {
    (void)0.0f;
}

namespace {
    enum CameraShakeType {
        CameraShakeType_None = 0,
        CameraShakeType_Strong = 1,
        CameraShakeType_Normal = 2,
        CameraShakeType_NormalWeak = 3,
        CameraShakeType_Weak = 4,
        CameraShakeType_VeryWeak = 5
    };

    struct StageEffectCameraData {
        /* 0x00 */ const char* mObjectName;
        /* 0x04 */ CameraShakeType mStartShakeType;
        /* 0x08 */ f32 mMovingShakeIntensity;
        /* 0x0C */ f32 mMovingShakeSpeed;
        /* 0x10 */ CameraShakeType mStopShakeType;
        /* 0x14 */ f32 mShakeDistance;
    };

    struct StageEffectPadData {
        /* 0x00 */ const char* mObjectName;
        /* 0x04 */ const char* mStartRumblePattern;
        /* 0x04 */ const char* mMovingRumblePattern;
        /* 0x0C */ const char* mStopRumblePattern;
    };

    struct StageEffectSoundData {
        /* 0x00 */ const char* mObjectName;
        /* 0x04 */ const char* mStartSoundEffect;
        /* 0x08 */ const char* mMovingSoundEffect;
        /* 0x0C */ const char* mStopSoundEffect;
        /* 0x10 */ s32 mStopSoundEffectSteps;
        /* 0x14 */ s32 mSoundEffectType;
    };

    static const StageEffectCameraData sStageEffectCameraDataTable[] = {
        {"LavaHomeVolcanoInnerFlow", CameraShakeType_None, 0.3f, 2.2f, CameraShakeType_None, 0.0f},
        {"LavaRotatePlanetStartStep", CameraShakeType_None, 0.0f, 0.0f, CameraShakeType_None, 0.0f},
        {"OceanRingRuinsMove", CameraShakeType_NormalWeak, 0.0f, 0.0f, CameraShakeType_NormalWeak, 3000.0f},
        {"OceanRingRuinsGearBig", CameraShakeType_None, 0.0f, 0.0f, CameraShakeType_None, 0.0f},
        {"OceanRingRuinsGearSmall", CameraShakeType_None, 0.0f, 0.0f, CameraShakeType_None, 0.0f},
        {"OceanRotateBridge", CameraShakeType_None, 0.0f, 0.0f, CameraShakeType_None, 3000.0f},
        {"ShutterDoorA", CameraShakeType_None, 0.0f, 0.0f, CameraShakeType_Normal, 3000.0f},
        {"ShutterDoorD", CameraShakeType_None, 0.0f, 0.0f, CameraShakeType_Normal, 3000.0f},
        {"OceanSwingBoard", CameraShakeType_None, 0.0f, 0.0f, CameraShakeType_None, 0.0f},
        {"RotateNutStep", CameraShakeType_None, 0.0f, 0.0f, CameraShakeType_None, 0.0f},
        {"OnimasuPlanetRailMovePartsA", CameraShakeType_None, 0.0f, 0.0f, CameraShakeType_None, 0.0f},
        {"GhostShipCaveMoveGroundA", CameraShakeType_None, 0.0f, 0.0f, CameraShakeType_None, 0.0f},
        {"GhostShipCaveMoveGroundB", CameraShakeType_None, 0.0f, 0.0f, CameraShakeType_None, 0.0f},
        {"WaterRoadCaveStepA", CameraShakeType_None, 0.0f, 0.0f, CameraShakeType_None, 0.0f},
        {"TeresaRoomDoor", CameraShakeType_None, 0.1f, 2.2f, CameraShakeType_None, 0.0f},
        {"LavaObstacleRockHomeA", CameraShakeType_None, 0.1f, 2.2f, CameraShakeType_None, 0.0f},
        {"LavaObstacleRockShell", CameraShakeType_None, 0.1f, 2.2f, CameraShakeType_None, 0.0f},
        {"ForestObstacleWoodA", CameraShakeType_None, 0.1f, 2.2f, CameraShakeType_None, 0.0f},
        {"MiniMechaKoopaPartsMoveStepA", CameraShakeType_None, 0.0f, 0.0f, CameraShakeType_Normal, 10000.0f},
        {"MiniMechaKoopaPartsCage", CameraShakeType_None, 0.0f, 0.0f, CameraShakeType_Normal, 10000.0f},
        {"RayGunPlanetPartsScrew", CameraShakeType_NormalWeak, 0.0f, 0.0f, CameraShakeType_Normal, 10000.0f},
        {"KoopaVS1PartsMoveStepA", CameraShakeType_Weak, 0.0f, 0.0f, CameraShakeType_Weak, -1.0f},
        {"KoopaVS1PartsMoveStepB", CameraShakeType_NormalWeak, 0.0f, 0.0f, CameraShakeType_NormalWeak, -1.0f},
        {"KoopaVS1PartsRotateMoveStepA", CameraShakeType_Weak, 0.0f, 0.0f, CameraShakeType_Weak, -1.0f},
        {"KoopaVS1PartsRotateMoveStepB", CameraShakeType_Weak, 0.0f, 0.0f, CameraShakeType_Weak, -1.0f},
        {"BeeWallClimbPartsA", CameraShakeType_Weak, 0.0f, 0.0f, CameraShakeType_Weak, -1.0f},
        {"KoopaJrSmallShipA", CameraShakeType_Normal, 0.0f, 0.0f, CameraShakeType_Normal, 3000.0f},
        {"IceVolcanoAppearStepA", CameraShakeType_None, 0.0f, 0.0f, CameraShakeType_Strong, 3000.0f},
        {"IceVolcanoClimbingWall", CameraShakeType_None, 0.0f, 0.0f, CameraShakeType_Strong, 10000.0f},
        {"BroadBeanMoveStepA", CameraShakeType_None, 0.0f, 0.0f, CameraShakeType_VeryWeak, -1.0f},
        {"BroadBeanMoveStepB", CameraShakeType_None, 0.0f, 0.0f, CameraShakeType_VeryWeak, -1.0f},
        {"HeavensDoorInsideCage", CameraShakeType_None, 0.15f, 1.2f, CameraShakeType_None, 10000.0f},
    };

    static const StageEffectPadData sStageEffectPadDataTable[] = {
        {"LavaHomeVolcanoInnerFlow", "中", "中", nullptr},
        {"LavaRotatePlanetStartStep", nullptr, nullptr, nullptr},
        {"OceanRingRuinsMove", nullptr, nullptr, "中"},
        {"OceanRingRuinsGearBig", nullptr, nullptr, nullptr},
        {"OceanRingRuinsGearSmall", nullptr, nullptr, nullptr},
        {"OceanRotateBridge", nullptr, nullptr, "中"},
        {"ShutterDoorA", nullptr, nullptr, "中"},
        {"ShutterDoorD", nullptr, "微弱", "中"},
        {"OceanSwingBoard", nullptr, nullptr, nullptr},
        {"RotateNutStep", nullptr, nullptr, nullptr},
        {"OnimasuPlanetRailMovePartsA", nullptr, nullptr, nullptr},
        {"GhostShipCaveMoveGroundA", nullptr, nullptr, nullptr},
        {"GhostShipCaveMoveGroundB", nullptr, nullptr, nullptr},
        {"WaterRoadCaveStepA", nullptr, nullptr, nullptr},
        {"TeresaRoomDoor", "中", nullptr, nullptr},
        {"MiniMechaKoopaPartsMoveStepA", nullptr, nullptr, "中"},
        {"MiniMechaKoopaPartsCage", nullptr, nullptr, "中"},
        {"RayGunPlanetPartsScrew", "中", "微弱", "中"},
        {"KoopaVS1PartsMoveStepA", "弱", nullptr, "弱"},
        {"KoopaVS1PartsMoveStepB", "弱", nullptr, "弱"},
        {"KoopaVS1PartsRotateMoveStepA", "弱", nullptr, "弱"},
        {"KoopaVS1PartsRotateMoveStepB", "弱", nullptr, "弱"},
        {"BeeWallClimbPartsA", "弱", nullptr, "弱"},
        {"KoopaJrSmallShipA", "中", nullptr, "強"},
        {"IceVolcanoAppearStepA", nullptr, "弱", "強"},
        {"IceVolcanoClimbingWall", nullptr, "弱", "強"},
        {"BroadBeanMoveStepA", nullptr, nullptr, "弱"},
        {"BroadBeanMoveStepB", nullptr, nullptr, "弱"},
        {"HeavensDoorInsideCage", nullptr, "弱", nullptr},
    };

    static const StageEffectSoundData sStageEffectSeDataTable[] = {
        {"LavaHomeVolcanoInnerFlow", 0, "SE_OJ_LV_LAVA_INCREASE_HOME", 0, -1, 0},
        {"LavaRotatePlanetStartStep", "SE_OJ_LAVA_BR_ROCK_L_ON", "SE_OJ_LV_LAVA_BR_ROCK_L_MV", 0, -1, 0},
        {"OceanRingRuinsMove", "SE_OJ_OCN_RUIN_MOVE_START", "SE_OJ_LV_OCN_RUIN_MOVE", "SE_OJ_OCN_RUIN_STOP", -1, 0},
        {"OceanRingRuinsGearBig", "SE_OJ_OCN_RUIN_GEAR_L_ST", "SE_OJ_LV_OCN_RUIN_MOVE", "SE_OJ_OCN_RUIN_GEAR_L_END", -1, 0},
        {"OceanRingRuinsGearSmall", 0, "SE_OJ_LV_OCN_RUIN_GEAR_S_MV", 0, -1, 0},
        {"OceanRotateBridge", "SE_OJ_O_ROTATE_BRIDGE_ST", "SE_OJ_LV_O_ROTATE_BRIDGE_MV", "SE_OJ_O_ROTATE_BRIDGE_END", -1, 2},
        {"ShutterDoorA", 0, "SE_OJ_LV_SHUTTER_DOOR_MOVE", "SE_OJ_SHUTTER_DOOR_STOP", -1, 0},
        {"OceanSwingBoard", "SE_OJ_O_SWG_BOARD_MOVE_ST", "SE_OJ_LV_O_SWG_BOARD_MOVE", 0, -1, 0},
        {"RotateNutStep", 0, "SE_OJ_LV_ROTATE_NUT_MOVE", 0, -1, 0},
        {"OceanFloaterTowerRotateStepA", "SE_OJ_O_TOWER_ROTATE_START", "SE_OJ_LV_O_TOWER_ROTATE_MV", 0, -1, 1},
        {"OceanFloaterTowerRotateStepB", "SE_OJ_O_TOWER_ROTATE_START", "SE_OJ_LV_O_TOWER_ROTATE_MV", 0, -1, 1},
        {"OceanFloaterTowerRotateStepC", "SE_OJ_O_TOWER_ROTATE_START", "SE_OJ_LV_O_TOWER_ROTATE_MV", 0, -1, 0},
        {"OceanFloaterTowerRotateStepD", "SE_OJ_O_TOWER_ROTATE_START", "SE_OJ_LV_O_TOWER_ROTATE_MV", 0, -1, 0},
        {"OceanAppearBridge", "SE_OJ_O_APPEAR_BRDG_APPEAR", 0, 0, -1, 0},
        {"OnimasuPlanetRotatePartsA", 0, "SE_OJ_LV_STONE_ROTATOR_MOVE", 0, -1, 0},
        {"OnimasuPlanetRotatePartsB", 0, "SE_OJ_LV_STONE_ROTATOR_MOVE", 0, -1, 0},
        {"OnimasuPlanetRailMovePartsA", "SE_OJ_STONE_FLOOR_START", "SE_OJ_LV_STONE_FLOOR_MOVE", "SE_OJ_STONE_FLOOR_END", -1, 0},
        {"GhostShipCaveMoveGroundA", "SE_OJ_PTM_GROUND_START", "SE_OJ_LV_PTM_GROUND_MOVE", "SE_OJ_PTM_GROUND_STOP", -1, 0},
        {"GhostShipCaveMoveGroundB", "SE_OJ_PTM_GROUND_START", "SE_OJ_LV_PTM_GROUND_MOVE", "SE_OJ_PTM_GROUND_STOP", -1, 0},
        {"WaterRoadCaveStepA", "SE_OJ_W_RD_CAVE_STEP_ST", "SE_OJ_LV_W_RD_CAVE_STEP_MV", "SE_OJ_W_RD_CAVE_STEP_ED", -1, 0},
        {"LavaBreakColumn", "SE_OJ_LAVA_COLUMN_FALL_ST", "SE_OJ_LV_LAVA_COLUMN_FALL", 0, 75, 0},
        {"LavaBreakRockA", "SE_OJ_LAVA_BR_ROCK_H_ON", "SE_OJ_LV_LAVA_BR_ROCK_H_MV", 0, -1, 0},
        {"LavaBreakRockB", "SE_OJ_LAVA_BR_ROCK_S_ON", "SE_OJ_LV_LAVA_BR_ROCK_S_MV", 0, -1, 0},
        {"LavaBreakRockC", "SE_OJ_LAVA_BR_ROCK_L_ON", "SE_OJ_LV_LAVA_BR_ROCK_L_MV", 0, -1, 0},
        {"LavaBreakRockD", "SE_OJ_LAVA_BR_ROCK_S_ON", "SE_OJ_LV_LAVA_BR_ROCK_S_MV", 0, -1, 0},
        {"LavaFallDownColumnA", "SE_OJ_LAVA_COLUMN_FALL_1", 0, 0, -1, 0},
        {"LavaFallDownColumnB", "SE_OJ_LAVA_COLUMN_FALL_2", 0, 0, -1, 0},
        {"OceanHomeTopRock", 0, 0, 0, -1, 0},
        {"WaterWheelGear", 0, "SE_OJ_LV_W_WHEEL_GEAR_MOVE", 0, -1, 0},
        {"TeresaRoomDoor", "SE_OJ_TERESA_ROOM_DOOR_OPEN", 0, 0, -1, 0},
        {"WaterPowerStream", "SE_OJ_W_POWER_STREAM_ST", "SE_OJ_LV_W_POWER_STREAM_MV", 0, -1, 0},
        {"LavaObstacleRockShell", "SE_OJ_LAVA_OB_ROCK_S_ON", "SE_OJ_LV_LAVA_OB_ROCK_S_MV", "SE_OJ_LAVA_OB_ROCK_S_STOP", 290, 0},
        {"LavaObstacleRockHomeA", "SE_OJ_LAVA_OB_ROCK_A_ON", "SE_OJ_LV_LAVA_OB_ROCK_A_MV", "SE_OJ_LAVA_OB_ROCK_A_STOP", 210, 0},
        {"LavaObstacleRockHomeB", 0, "SE_OJ_LV_LAVA_OB_ROCK_B_MV", "SE_OJ_LAVA_OB_ROCK_B_STOP", 280, 0},
        {"UFOKinoko", 0, "SE_OJ_LV_UFO_KINOKO_ROTATE", 0, -1, 0},
        {"PhantomShipPropellerSmall", 0, "SE_OJ_LV_P_SHIP_PROPELLER_S", 0, -1, 0},
        {"PhantomShipPropellerBig", 0, "SE_OJ_LV_P_SHIP_PROPELLER_L", 0, -1, 0},
        {"ForestAppearStepA", "SE_OJ_FOREST_STEP_APPEAR", 0, 0, -1, 0},
        {"ForestObstacleWoodA", "SE_OJ_FOREST_OBS_WOOD_ON", "SE_OJ_LV_FOREST_OBS_WOOD_MV", "SE_OJ_FOREST_OBS_WOOD_STOP", 120, 2},
        {"RGTowerPressGroundA", "SE_OJ_STONE_FLOOR_S_START", "SE_OJ_LV_STONE_FLOOR_S_MOVE", "SE_OJ_STONE_FLOOR_S_END", -1, 0},
        {"TeresaMansionEntranceDoor", 0, 0, "SE_OJ_P_ROOM_DOOR_L_CL_ST", -1, 0},
        {"BattleShipMovePartsA", 0, "SE_OJ_LV_METAL_ROTATOR_MOVE", 0, -1, 0},
        {"ReverseGravityRoomPressGroundA", "SE_OJ_METAL_FLOOR_S_START", "SE_OJ_LV_METAL_FLOOR_S_MOVE", "SE_OJ_METAL_FLOOR_S_STOP", -1, 0},
        {"ReverseGravityRoomPressGroundB", "SE_OJ_METAL_FLOOR_L_START", "SE_OJ_LV_METAL_FLOOR_L_MOVE", "SE_OJ_METAL_FLOOR_L_STOP", -1, 0},
        {"ShutterDoorB", "SE_OJ_METAL_FLOOR_L_START", "SE_OJ_LV_METAL_FLOOR_L_MOVE", "SE_OJ_METAL_FLOOR_L_STOP", -1, 2},
        {"TeresaMansionHoleCover", 0, 0, "SE_OJ_STRANGEROCK_BREAK_S", -1, 0},
        {"KoopaStatue", 0, 0, "SE_OJ_STRANGEROCK_BREAK_S", -1, 2},
        {"TeresaRacePartsA", 0, "SE_OJ_LV_TR_RACE_ROTATER", 0, -1, 0},
        {"TeresaRacePartsB", "SE_OJ_TR_RACE_PARTS_BOUND", 0, 0, -1, 0},
        {"BeeWallClimbPartsA", 0, "SE_OJ_LV_BEE_WALL_MOVE", "SE_OJ_BEE_WALL_STOP", -1, 0},
        {"ShutterDoorC", "SE_OJ_FENCE_SHUTTER_START", "SE_OJ_LV_FENCE_SHUTTER_MOVE", "SE_OJ_FENCE_SHUTTER_STOP", 174, 2},
        {"ShutterDoorD", "SE_OJ_FENCE_SHUTTER_S_ST", "SE_OJ_LV_FENCE_SHUTTER_S_MV", "SE_OJ_FENCE_SHUTTER_S_ED", 180, 0},
        {"IcicleRock", 0, 0, "SE_OJ_STRANGEROCK_BREAK_S", -1, 0},
        {"QuestionBoxMoveStepA", 0, "SE_OJ_LV_Q_BOX_ROTATE", 0, -1, 0},
        {"QuestionBoxMoveStepB", 0, "SE_OJ_LV_Q_BOX_RAIL_MOVE", 0, -1, 0},
        {"QuestionBoxMoveStepC", 0, "SE_OJ_LV_Q_BOX_RAIL_UD", "SE_OJ_Q_BOX_RAIL_UD_STOP", -1, 0},
        {"KoopaVS1PartsMoveStepA", "SE_OJ_KOOPA_F_LIFT_ST", "SE_OJ_LV_KOOPA_F_LIFT_MV", "SE_OJ_KOOPA_F_LIFT_ED", -1, 0},
        {"KoopaVS1PartsMoveStepB", "SE_OJ_KOOPA_F_LIFT_ST", "SE_OJ_LV_KOOPA_F_LIFT_MV", "SE_OJ_KOOPA_F_LIFT_ED", -1, 0},
        {"KoopaVS1PartsRotateMoveStepA", "SE_OJ_KOOPA_F_R_LIFT_ST", "SE_OJ_LV_KOOPA_F_R_LIFT_MV", "SE_OJ_KOOPA_F_R_LIFT_ED", -1, 0},
        {"KoopaVS1PartsRotateMoveStepB", "SE_OJ_KOOPA_F_R_LIFT_ST", "SE_OJ_LV_KOOPA_F_R_LIFT_MV", "SE_OJ_KOOPA_F_R_LIFT_ED", -1, 0},
        {"SideSpikeMoveStepA", "SE_OJ_SIDE_SPIKE_STEP_ST", "SE_OJ_LV_SIDE_SPIKE_STEP_MV", "SE_OJ_SIDE_SPIKE_STEP_ED", -1, 0},
        {"PhantomTowerMoveStepA", "SE_OJ_STONE_FLOOR_S_START", "SE_OJ_LV_STONE_FLOOR_S_MOVE", "SE_OJ_STONE_FLOOR_S_END", -1, 0},
        {"IceVolcanoAppearStepA", "SE_OJ_VOL_APPEAR_STEP_ST", "SE_OJ_LV_VOL_APPEAR_STEP_MV", "SE_OJ_VOL_APPEAR_STEP_ED", 218, 2},
        {"IceVolcanoClimbingWall", "SE_OJ_VOL_CLIMB_WALL_ST", "SE_OJ_LV_VOL_CLIMB_WALL_MV", "SE_OJ_VOL_CLIMB_WALL_ED", 458, 2},
        {"IceUpDownShutterDoorA", "SE_OJ_MTL_SHUTTER_S_ST", "SE_OJ_LV_MTL_SHUTTER_S_MV", "SE_OJ_MTL_SHUTTER_S_ED", 144, 2},
        {"IceVolcanoMoveStepA", 0, "SE_OJ_LV_VOL_MOVE_STEP_FLOW", 0, -1, 0},
        {"HopperBeltConveyerRotatePartsA", 0, "SE_OJ_LV_HOPPER_CONVEYER_MV", 0, -1, 0},
        {"HopperBeltConveyerMovePartsA", 0, "SE_OJ_LV_HOPPER_CONVEYER_MV", 0, -1, 0},
        {"MechaKoopaPartsCaterpillar", 0, "SE_OJ_LV_M_KOOPA_CATER_L_MV", 0, -1, 0},
        {"BroadBeanMoveStepA", "SE_OJ_STONE_FLOOR_S_START", "SE_OJ_LV_STONE_FLOOR_S_MOVE", "SE_OJ_STONE_FLOOR_S_END", -1, 0},
        {"BroadBeanMoveStepB", "SE_OJ_STONE_FLOOR_S_START", "SE_OJ_LV_STONE_FLOOR_S_MOVE", "SE_OJ_STONE_FLOOR_S_END", -1, 0},
        {"RayGunPlanetPartsGear", 0, "SE_OJ_LV_RAYGUN_GEAR_ROT", 0, -1, 0},
        {"RayGunPlanetPartsScrew", 0, "SE_OJ_LV_RAYGUN_SCREW_ROT", "SE_OJ_RAYGUN_SCREW_STOP", -1, 2},
        {"SandStreamHighTowerMoveStepA", "SE_OJ_STONE_FLOOR_S_START", "SE_OJ_LV_STONE_FLOOR_S_MOVE", "SE_OJ_STONE_FLOOR_S_END", -1, 0},
        {"MechaKoopaPartsRollerA", 0, "SE_OJ_LV_M_KOOPA_CATER_S_MV", 0, -1, 0},
        {"MiniMechaKoopaPartsGear", 0, "SE_OJ_LV_MM_KOOPA_GEAR_ROT", 0, -1, 0},
        {"MiniMechaKoopaPartsCage", "SE_OJ_MM_KOOPA_CAGE_START", "SE_OJ_LV_MM_KOOPA_CAGE_ROT", "SE_OJ_MM_KOOPA_CAGE_STOP", -1, 2},
        {"MiniMechaKoopaPartsMoveStepA", "SE_OJ_MM_KOOPA_STEP_START", "SE_OJ_LV_MM_KOOPA_STEP_MOVE", "SE_OJ_MM_KOOPA_STEP_STOP", -1, 2},
        {"AsteroidBlockRotateStepA", 0, "SE_OJ_LV_AST_BLOCK_ROTATE", 0, -1, 0},
        {"SandUpDownTowerBreakableWallA", 0, 0, "SE_OJ_WOOD_WALL_BREAK", -1, 0},
        {"SandUpDownTowerBreakableWallB", 0, 0, "SE_OJ_WOOD_WALL_BREAK", -1, 0},
        {"HoleDeathSandMoveStepA", "SE_OJ_SAND_MOVE_STEP_ST", "SE_OJ_LV_SAND_MOVE_STEP_MV", "SE_OJ_SAND_MOVE_STEP_ED", -1, 0},
        {"CannonFortressMoveWall", "SE_OJ_CNFORT_MOVEWALL_ST", "SE_OJ_LV_CNFORT_MOVEWALL_MV", "SE_OJ_CNFORT_MOVEWALL_ED", -1, 0},
        {"MechaKoopaPartsBreast", "SE_OJ_M_KOOPA_BREAST_START", "SE_OJ_LV_M_KOOPA_BREAST_MV", "SE_OJ_M_KOOPA_BREAST_BREAK", -1, 1},
        {"ChoConveyorMoveChocoA", 0, "SE_OJ_LV_CHOCONVEYER_MOVE", 0, -1, 0},
        {"MechaKoopaPartsCollar", 0, "SE_OJ_LV_M_KOOPA_COLLAR_ROT", 0, -1, 0},
        {"MechaKoopaPartsMoveStepA", "SE_OJ_M_KOOPA_MV_STEP_ST", "SE_OJ_LV_M_KOOPA_MV_STEP_MV", "SE_OJ_M_KOOPA_MV_STEP_ED", -1, 0},
        {"ChoConveyorPlanetD", 0, "SE_OJ_LV_CHO_CONVEYOR_PLN_D", 0, -1, 0},
        {"SeaBottomBigDoorA", "SE_OJ_SEABTM_B_DOOR_START", "SE_OJ_LV_SEABTM_B_DOOR_MOVE", "SE_OJ_SEABTM_B_DOOR_STOP", -1, 0},
        {"SeaBottomBigDoorB", "SE_OJ_SEABTM_B_DOOR_START", "SE_OJ_LV_SEABTM_B_DOOR_MOVE", "SE_OJ_SEABTM_B_DOOR_STOP", -1, 0},
        {"SeaBottomBigDoorC", "SE_OJ_SEABTM_DOOR_C_START", "SE_OJ_LV_SEABTM_DOOR_C_MOVE", "SE_OJ_SEABTM_DOOR_C_STOP", -1, 0},
        {"WaterBazookaTowerMoveStepA", 0, "SE_OJ_LV_W_BZK_TW_STEP_A_MV", "SE_OJ_W_BZK_TW_STEP_A_ED", -1, 0},
        {"WaterBazookaTowerMoveStepB", "SE_OJ_W_BZK_TW_STEP_B_ST", "SE_OJ_LV_W_BZK_TW_STEP_B_MV", "SE_OJ_W_BZK_TW_STEP_B_ED", -1, 0},
        {"WaterBazookaTowerMoveStepC", "SE_OJ_W_BZK_TW_STEP_C_ST", "SE_OJ_LV_W_BZK_TW_STEP_C_MV", "SE_OJ_W_BZK_TW_STEP_C_ED", -1, 0},
        {"KoopaJrSmallShipA", "SE_OJ_KOOPA_JR_S_SHIP_ST", "SE_OJ_LV_KOOPA_JR_S_SHIP_MV", "SE_OJ_KOOPA_JR_S_SHIP_ED", -1, 0},
        {"HeavensDoorInsideRotatePartsA", 0, "SE_OJ_LV_HD_INSIDE_ROTATOR", 0, -1, 0},
        {"HeavensDoorInsideRotatePartsB", 0, "SE_OJ_LV_HD_INSIDE_ROTATOR", 0, -1, 0},
        {"HeavensDoorInsideRotatePartsC", 0, "SE_OJ_LV_HD_INSIDE_ROTATOR", 0, -1, 0},
        {"HeavensDoorMiddleRotatePartsA", 0, "SE_OJ_LV_HD_MID_ROTATOR_A", 0, -1, 0},
        {"HeavensDoorMiddleRotatePartsB", 0, "SE_OJ_LV_HD_MID_ROTATOR_B", 0, -1, 0},
        {"HeavensDoorInsideCage", "SE_OJ_HD_INSIDE_CAGE_BREAK", 0, 0, -1, 0},
        {"HeavensDoorInsidePlanetPartsA", "SE_OJ_HD_INSIDE_SUN_OFF", 0, 0, -1, 0},
        {"HeavensDoorAppearStepA", "SE_OJ_HD_STEP_APPEAR", 0, 0, -1, 0},
        {"ReverseGravityRoomPressGroundC", "SE_OJ_METAL_FLOOR_S_START", "SE_OJ_LV_METAL_FLOOR_S_MOVE", "SE_OJ_METAL_FLOOR_S_STOP", -1, 0},
        {"ReverseGravityRoomPressGroundD", "SE_OJ_METAL_FLOOR_L_START", "SE_OJ_LV_METAL_FLOOR_L_MOVE", "SE_OJ_METAL_FLOOR_L_STOP", -1, 0},
        {"BattleShipMovePartsB", "SE_OJ_BS_MOVE_PARTS_B_ST", "SE_OJ_LV_BS_MOVE_PARTS_B_MV", "SE_OJ_BS_MOVE_PARTS_B_ED", -1, 0},
        {"IceWaterUpDownParts", 0, "SE_AT_LV_ICE_WATER_UP_DOWN", 0, -1, 0},
        {"LavaUpDownParts", 0, "SE_AT_LV_LAVA_UP_DOWN", 0, -1, 0},
        {"KoopaVS2PartsStartMoveStepA", "SE_OJ_KP2_START_STEP_ST", "SE_OJ_LV_KP2_START_STEP_MV", "SE_OJ_KP2_START_STEP_ED", -1, 0},
        {"KoopaVS2PartsStartMoveStepB", "SE_OJ_KP2_START_STEP_ST", "SE_OJ_LV_KP2_START_STEP_MV", "SE_OJ_KP2_START_STEP_ED", -1, 0},
        {"KoopaVS2PartsStartMoveStepC", "SE_OJ_KP2_START_STEP_ST", "SE_OJ_LV_KP2_START_STEP_MV", "SE_OJ_KP2_START_STEP_ED", -1, 0},
        {"KoopaVS2PartsRollingStep", 0, "SE_OJ_LV_KP2_ROLL_STEP_MV", 0, -1, 0},
        {"KoopaVS2PartsSSquareMoveStep", "SE_OJ_KP2_S_SQR_STEP_ST", "SE_OJ_LV_KP2_S_SQR_STEP_MV", "SE_OJ_KP2_S_SQR_STEP_ED", -1, 0},
        {"KoopaVS2Parts2DMoveStepBarA", "SE_OJ_KP2_START_STEP_ST_F", "SE_OJ_LV_KP2_START_STEP_M_F", "SE_OJ_KP2_START_STEP_ED_F", -1, 0},
        {"KoopaVS2Parts2DMoveStepBarB", "SE_OJ_KP2_START_STEP_ST_F", "SE_OJ_LV_KP2_START_STEP_M_F", "SE_OJ_KP2_START_STEP_ED_F", -1, 0},
        {"KoopaVS2Parts2DMoveStepSBarB", "SE_OJ_KP2_START_STEP_ST_F", "SE_OJ_LV_KP2_START_STEP_M_F", "SE_OJ_KP2_START_STEP_ED_F", -1, 0},
        {"KoopaVS2Parts2DMoveStepConvex", "SE_OJ_KP2_START_STEP_ST_F", "SE_OJ_LV_KP2_START_STEP_M_F", "SE_OJ_KP2_START_STEP_ED_F", -1, 0},
        {"KoopaVS2Parts2DMoveStepLShape", "SE_OJ_KP2_START_STEP_ST_F", "SE_OJ_LV_KP2_START_STEP_M_F", "SE_OJ_KP2_START_STEP_ED_F", -1, 0},
        {"KoopaVS2Parts2DMoveStepCross", "SE_OJ_KP2_START_STEP_ST_F", "SE_OJ_LV_KP2_START_STEP_M_F", "SE_OJ_KP2_START_STEP_ED_F", -1, 0},
        {"KoopaVS2PartsSquareMoveStepA", 0, "SE_OJ_LV_KP2_START_STEP_MV", 0, -1, 0},
        {"KoopaVS2PartsSquareMoveStepB", 0, "SE_OJ_LV_KP2_START_STEP_MV", 0, -1, 0},
        {"KoopaVS2PartsJoinedMoveStep", "SE_OJ_KP2_JNT_STEP_ST", "SE_OJ_LV_KP2_JNT_STEP_MV", "SE_OJ_KP2_JNT_STEP_ED", -1, 0},
        {"HeavenlyBeachTimerStepA", "SE_OJ_O_APPEAR_BRDG_APPEAR", 0, 0, -1, 0},
        {"HeavenlyBeachTimerStepB", "SE_OJ_O_APPEAR_BRDG_APPEAR", 0, 0, -1, 0},
        {"IceLavaIslandLavaMovableStepA", 0, "SE_AT_LV_ICE_LAVA_LAVA_STEP", 0, -1, 0},
        {"ConcreteBlock", 0, 0, "SE_OJ_STRANGEROCK_BREAK_M", -1, 0},
        {"WindMillPropeller", 0, "SE_OJ_LV_WINDMILL_PROPELLER", 0, -1, 0},
        {"BiriBiriBegomanSpikePistonA", "SE_OJ_METAL_FLOOR_L_START", "SE_OJ_LV_METAL_FLOOR_L_MOVE", "SE_OJ_METAL_FLOOR_L_STOP", -1, 0},
        {"HeavenlyBeachUnderRock", "SE_OJ_HB_UNDERROCK_BREAK", "SE_OJ_LV_HB_UNDERROCK_WATER", 0, -1, 0},
        {"HeavenlyBeachTopRock", "SE_OJ_STRANGEROCK_BREAK_M", 0, 0, -1, 0},
        {"FlagDiscPlanetD", 0, "SE_OJ_LV_FLY_DISK_ROTATE", 0, -1, 0},
        {"IceVolcanoAppearStepB", "SE_OJ_IV_STEP_B_START", "SE_OJ_LV_IV_STEP_B_MVOE", "SE_OJ_IV_STEP_B_END", 210, 0},
        {"ShutterDoorE", "SE_OJ_MTL_SHUTTER_S_ST", "SE_OJ_LV_MTL_SHUTTER_S_MV", "SE_OJ_MTL_SHUTTER_S_ED", 173, 2},
        {"SnowBlockA", 0, 0, "SE_OJ_SNOW_MAN_MELT", -1, 0},
        {"ChallengeBallRotateBridgeA", "SE_OJ_CHAL_RT_BRIDGE_ST", "SE_OJ_LV_CHAL_RT_BRIDGE_ROT", "SE_OJ_CHAL_RT_BRIDGE_ED", -1, 0},
        {"ChallengeBallMoveGroundA", "SE_OJ_CHAL_MV_GND_L_ST", "SE_OJ_LV_CHAL_MV_GND_L_MV", "SE_OJ_CHAL_MV_GND_L_ED", -1, 0},
        {"ChallengeBallMoveGroundB", 0, "SE_OJ_LV_CHAL_MV_GND_S_MV", 0, -1, 0},
        {"ChallengeBallGearA", 0, "SE_OJ_LV_CHAL_GEAR_ROTATE", 0, -1, 0},
        {"ChallengeBallAccelCylinderA", 0, "SE_OJ_LV_CHAL_ACC_CYLINDER", 0, -1, 0},
        {"AstroDecoratePartsGearA", 0, "SE_OJ_LV_ASTRO_DECO_GEAR_MV", 0, -1, 0},
        {"DarkHopperRotateStepA", "SE_OJ_MYST_CIRCLE_PENDULUM", 0, 0, -1, 0},
        {"TrialBubbleRevolvingPartsA", 0, "SE_OJ_LV_TRIBBL_REVOLV_PRT", 0, -1, 0},
        {"TrialBubbleRotateWallA", 0, "SE_OJ_LV_TRIBBL_ROT_WALL", 0, -1, 0},
        {"TrialBubbleMoveWallA", "SE_OJ_TRIBBL_MOVEWALL_ST", "SE_OJ_LV_TRIBBL_MOVEWALL_MV", "SE_OJ_TRIBBL_MOVEWALL_ED", -1, 0},
        {"CubeBubbleExRotateWallL", 0, "SE_OJ_LV_CUBEBBL_ROT_WALL_L", 0, -1, 0},
        {"PowerStarKeeperA", 0, "SE_OJ_LV_POW_STAR_KEEPER_A", 0, -1, 0},
        {"PowerStarKeeperB", 0, "SE_OJ_LV_POW_STAR_KEEPER_B", 0, -1, 0},
        {"PowerStarKeeperC", 0, "SE_OJ_LV_POW_STAR_KEEPER_C", 0, -1, 0},
        {"KoopaVS3RotateStepsA", 0, "SE_OJ_LV_KP_VS3_ROT_STEP_A", 0, -1, 0},
        {"DeathPromenadeMovePartsSpuareA", "SE_OJ_DP_MOVE_STEP_START", "SE_OJ_LV_DP_MOVE_STEP_MOVE", "SE_OJ_DP_MOVE_STEP_START", -1, 0},
        {"DeathPromenadeRotateCircleL", 0, "SE_OJ_LV_DP_ROT_CIRCLE_L", 0, -1, 0},
        {"DeathPromenadeRotateCircleS", 0, "SE_OJ_LV_DP_ROT_CIRCLE_S", 0, -1, 0},
        {"MiniMechaKoopaPartsFan", 0, "SE_OJ_LV_MM_KOOPA_FAN", 0, -1, 0},
        {"ClockworkHandle", 0, "SE_OJ_LV_CLOCKWORK_HANDLE", 0, -1, 0},
        {"SunkenShip", 0, 0, "SE_OJ_SUNKEN_SHIP_BREAK", -1, 2}};

    const StageEffectCameraData* getStageEffectCameraParam(const char* pObjectName) {
        for (u32 i = 0; i < ARRAY_SIZE(sStageEffectCameraDataTable); i++) {
            const StageEffectCameraData* pTable = sStageEffectCameraDataTable;

            if (MR::isEqualString(pObjectName, pTable[i].mObjectName)) {
                return &sStageEffectCameraDataTable[i];
            }
        }

        return nullptr;
    }

    const StageEffectPadData* getStageEffectPadParam(const char* pObjectName) {
        for (u32 i = 0; i < ARRAY_SIZE(sStageEffectPadDataTable); i++) {
            const StageEffectPadData* pTable = sStageEffectPadDataTable;

            if (MR::isEqualString(pObjectName, pTable[i].mObjectName)) {
                return &sStageEffectPadDataTable[i];
            }
        }

        return nullptr;
    }

    const StageEffectSoundData* getStageEffectSeParam(const char* pObjectName) {
        for (u32 i = 0; i < ARRAY_SIZE(sStageEffectSeDataTable); i++) {
            const StageEffectSoundData* pTable = sStageEffectSeDataTable;

            if (MR::isEqualString(pObjectName, pTable[i].mObjectName)) {
                return &sStageEffectSeDataTable[i];
            }
        }

        return nullptr;
    }

    void shakeCamera(CameraShakeType type) {
        switch (type) {
        case CameraShakeType_Strong:
            MR::shakeCameraStrong();
            break;
        case CameraShakeType_Normal:
            MR::shakeCameraNormal();
            break;
        case CameraShakeType_NormalWeak:
            MR::shakeCameraNormalWeak();
            break;
        case CameraShakeType_Weak:
            MR::shakeCameraWeak();
            break;
        case CameraShakeType_VeryWeak:
            MR::shakeCameraVeryWeak();
            break;
        default:
            break;
        }
    }

    bool isDistanceValidShake(LiveActor* pActor, const char* pObjectName) {
        f32 shakeDistance;

        if (getStageEffectCameraParam(pObjectName) != nullptr) {
            shakeDistance = getStageEffectCameraParam(pObjectName)->mShakeDistance;
        } else {
            shakeDistance = 0.0f;
        }

        if (-1.0f == shakeDistance) {
            return MR::isOnPlayer(pActor);
        }

        f32 playerDistance = MR::calcDistanceToPlayer(pActor);
        bool isValidShake = false;

        if (0.0f < playerDistance) {
            if (getStageEffectCameraParam(pObjectName) != nullptr) {
                shakeDistance = getStageEffectCameraParam(pObjectName)->mShakeDistance;
            } else {
                shakeDistance = 0.0f;
            }

            if (playerDistance < shakeDistance) {
                isValidShake = true;
            }
        }

        return isValidShake;
    }
};  // namespace

namespace MR {
    const char* StageEffect::getStartSe(const char* pObjectName) {
        if (::getStageEffectSeParam(pObjectName) != nullptr) {
            return ::getStageEffectSeParam(pObjectName)->mStartSoundEffect;
        }

        return nullptr;
    }

    const char* StageEffect::getMovingSe(const char* pObjectName) {
        if (::getStageEffectSeParam(pObjectName) != nullptr) {
            return ::getStageEffectSeParam(pObjectName)->mMovingSoundEffect;
        }

        return nullptr;
    }

    const char* StageEffect::getStopSe(const char* pObjectName) {
        if (::getStageEffectSeParam(pObjectName) != nullptr) {
            return ::getStageEffectSeParam(pObjectName)->mStopSoundEffect;
        }

        return nullptr;
    }

    s32 StageEffect::getStopSeSteps(const char* pObjectName) {
        if (::getStageEffectSeParam(pObjectName) != nullptr) {
            return ::getStageEffectSeParam(pObjectName)->mStopSoundEffectSteps;
        }

        return -1;
    }

    bool StageEffect::isRiddleSeTypeStop(const char* pObjectName) {
        if (::getStageEffectSeParam(pObjectName) != nullptr) {
            return ::getStageEffectSeParam(pObjectName)->mSoundEffectType == 2;
        }

        return false;
    }

    bool StageEffect::isExistStageEffectSeData(const char* pObjectName) {
        return ::getStageEffectSeParam(pObjectName) != nullptr;
    }

    bool StageEffect::isExistStageEffectData(const char* pObjectName) {
        return ::getStageEffectCameraParam(pObjectName) != nullptr || ::getStageEffectPadParam(pObjectName) != nullptr ||
               ::getStageEffectSeParam(pObjectName) != nullptr;
    }

    void StageEffect::shakeStartCamera(LiveActor* pActor, const char* pObjectName) {
        if (::isDistanceValidShake(pActor, pObjectName)) {
            CameraShakeType type;

            if (::getStageEffectCameraParam(pObjectName) != nullptr) {
                type = ::getStageEffectCameraParam(pObjectName)->mStartShakeType;
            } else {
                type = CameraShakeType_None;
            }

            ::shakeCamera(type);
        }
    }

    void StageEffect::shakeStopCamera(LiveActor* pActor, const char* pObjectName) {
        if (::isDistanceValidShake(pActor, pObjectName)) {
            CameraShakeType type;

            if (::getStageEffectCameraParam(pObjectName) != nullptr) {
                type = ::getStageEffectCameraParam(pObjectName)->mStopShakeType;
            } else {
                type = CameraShakeType_None;
            }

            ::shakeCamera(type);
        }
    }

    void StageEffect::shakeCameraMoving(NameObj* pActor, const char* pObjectName) {
        f32 intensity;

        if (::getStageEffectCameraParam(pObjectName) != nullptr) {
            intensity = ::getStageEffectCameraParam(pObjectName)->mMovingShakeIntensity;
        } else {
            intensity = 0.0f;
        }

        if (intensity <= 0.0f) {
            return;
        }

        f32 speed;

        if (::getStageEffectCameraParam(pObjectName) != nullptr) {
            speed = ::getStageEffectCameraParam(pObjectName)->mMovingShakeSpeed;
        } else {
            speed = 0.0f;
        }

        MR::shakeCameraInfinity(pActor, intensity, speed);
    }

    void StageEffect::stopShakingCameraMoving(NameObj* pActor, const char* pObjectName) {
        f32 intensity;

        if (::getStageEffectCameraParam(pObjectName) != nullptr) {
            intensity = ::getStageEffectCameraParam(pObjectName)->mMovingShakeIntensity;
        } else {
            intensity = 0.0f;
        }

        if (intensity <= 0.0f) {
            return;
        }

        MR::stopShakingCamera(pActor);
    }

    void StageEffect::rumblePadStart(LiveActor* pActor, const char* pObjectName) {
        if (::isDistanceValidShake(pActor, pObjectName)) {
            const char* pRumblePattern;

            if (::getStageEffectPadParam(pObjectName) != nullptr) {
                pRumblePattern = ::getStageEffectPadParam(pObjectName)->mStartRumblePattern;
            } else {
                pRumblePattern = nullptr;
            }

            if (pRumblePattern != nullptr) {
                MR::tryRumblePad(pActor, pRumblePattern, 0);
            }
        }
    }

    void StageEffect::rumblePadStop(LiveActor* pActor, const char* pObjectName) {
        if (::isDistanceValidShake(pActor, pObjectName)) {
            const char* pRumblePattern;

            if (::getStageEffectPadParam(pObjectName) != nullptr) {
                pRumblePattern = ::getStageEffectPadParam(pObjectName)->mStopRumblePattern;
            } else {
                pRumblePattern = nullptr;
            }

            if (pRumblePattern != nullptr) {
                MR::tryRumblePad(pActor, pRumblePattern, 0);
            }
        }
    }

    void StageEffect::rumblePadMoving(LiveActor* pActor, const char* pObjectName) {
        const char* pRumblePattern;

        if (::getStageEffectPadParam(pObjectName) != nullptr) {
            pRumblePattern = ::getStageEffectPadParam(pObjectName)->mMovingRumblePattern;
        } else {
            pRumblePattern = nullptr;
        }

        if (pRumblePattern != nullptr) {
            MR::tryRumblePad(pActor, pRumblePattern, 0);
        }
    }

    bool StageEffect::tryStageEffectStart(LiveActor* pActor, const char* pObjectName) {
        bool isExistStageEffectData = false;

        if (::getStageEffectCameraParam(pObjectName) != nullptr || ::getStageEffectPadParam(pObjectName) != nullptr ||
            ::getStageEffectSeParam(pObjectName) != nullptr) {
            isExistStageEffectData = true;
        }

        if (!isExistStageEffectData) {
            return false;
        }

        if (MR::isRegisteredEffect(pActor, "Start")) {
            MR::emitEffect(pActor, "Start");
        }

        const char* pStartSoundEffect;

        if (::getStageEffectSeParam(pObjectName) != nullptr) {
            pStartSoundEffect = ::getStageEffectSeParam(pObjectName)->mStartSoundEffect;
        } else {
            pStartSoundEffect = nullptr;
        }

        if (pStartSoundEffect) {
            MR::startSound(pActor, pStartSoundEffect);
        }

        bool isPlayRiddleSound;

        if (::getStageEffectSeParam(pObjectName) != nullptr) {
            isPlayRiddleSound = ::getStageEffectSeParam(pObjectName)->mSoundEffectType == 1;
        } else {
            isPlayRiddleSound = false;
        }

        if (isPlayRiddleSound) {
            MR::startSystemSE("SE_SY_READ_RIDDLE_S");
        }

        rumblePadStart(pActor, pObjectName);
        shakeStartCamera(pActor, pObjectName);
        return true;
    }

    bool StageEffect::tryStageEffectMoving(LiveActor* pActor, const char* pObjectName) {
        bool isExistStageEffectData = false;

        if (::getStageEffectCameraParam(pObjectName) != nullptr || ::getStageEffectPadParam(pObjectName) != nullptr ||
            ::getStageEffectSeParam(pObjectName) != nullptr) {
            isExistStageEffectData = true;
        }

        if (!isExistStageEffectData) {
            return false;
        }

        const char* pMovingSoundEffect;

        if (::getStageEffectSeParam(pObjectName) != nullptr) {
            pMovingSoundEffect = ::getStageEffectSeParam(pObjectName)->mMovingSoundEffect;
        } else {
            pMovingSoundEffect = nullptr;
        }

        if (pMovingSoundEffect) {
            MR::startLevelSound(pActor, pMovingSoundEffect);
        }

        rumblePadMoving(pActor, pObjectName);
        return true;
    }

    bool StageEffect::tryStageEffectStop(LiveActor* pActor, const char* pObjectName) {
        bool isExistStageEffectData = false;

        if (::getStageEffectCameraParam(pObjectName) != nullptr || ::getStageEffectPadParam(pObjectName) != nullptr ||
            ::getStageEffectSeParam(pObjectName) != nullptr) {
            isExistStageEffectData = true;
        }

        if (!isExistStageEffectData) {
            return false;
        }

        if (MR::isRegisteredEffect(pActor, "Stop")) {
            MR::emitEffect(pActor, "Stop");
        }

        const char* pStopSoundEffect;

        if (::getStageEffectSeParam(pObjectName) != nullptr) {
            pStopSoundEffect = ::getStageEffectSeParam(pObjectName)->mStopSoundEffect;
        } else {
            pStopSoundEffect = nullptr;
        }

        if (pStopSoundEffect) {
            MR::startSound(pActor, pStopSoundEffect);
        }

        bool isPlayRiddleSound;

        if (::getStageEffectSeParam(pObjectName) != nullptr) {
            isPlayRiddleSound = ::getStageEffectSeParam(pObjectName)->mSoundEffectType == 2;
        } else {
            isPlayRiddleSound = false;
        }

        if (isPlayRiddleSound) {
            MR::startSystemSE("SE_SY_READ_RIDDLE_S");
        }

        rumblePadStop(pActor, pObjectName);
        shakeStopCamera(pActor, pObjectName);
        return true;
    }
};  // namespace MR
