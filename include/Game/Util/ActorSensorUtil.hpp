#pragma once

#include <JSystem/JGeometry/TVec.hpp>
#include <revolution.h>

class HitSensor;
class LiveActor;

enum {
    ACTMES_PLAYER_ATTACK_START = 0x0,
    ACTMES_PLAYER_PUNCH = 0x1,
    ACTMES_PLAYER_TRAMPLE = 0x2,
    ACTMES_PLAYER_HIP_DROP = 0x3,
    ACTMES_PLAYER_HIP_DROP_FLOOR = 0x4,
    ACTMES_PLAYER_UPPER_PUNCH = 0x5,
    ACTMES_JET_TURTLE_ATTACK = 0x6,
    ACTMES_FOO_ATTACK = 0x7,
    ACTMES_FIREBALL_ATTACK = 0x8,
    ACTMES_SEARCHLIGHT_ATTACK = 0x9,
    ACTMES_FREEZE_ATTACK = 0xA,
    ACTMES_INVINCIBLE_ATTACK = 0xB,
    ACTMES_STAR_PIECE_ATTACK = 0xC,
    ACTMES_IS_STAR_PIECE_REFLECT = 0xD,
    ACTMES_IS_LOCKON_STAR_PIECE_SHOOT = 0xE,
    ACTMES_STAR_PIECE_GIFT = 0xF,
    ACTMES_STAR_PIECE_GIFT_1 = 0x10,
    ACTMES_STAR_PIECE_GIFT_2 = 0x11,
    ACTMES_STAR_PIECE_GIFT_3 = 0x12,
    ACTMES_STAR_PIECE_GIFT_4 = 0x13,
    ACTMES_STAR_PIECE_GIFT_5 = 0x14,
    ACTMES_STAR_PIECE_GIFT_6 = 0x15,
    ACTMES_STAR_PIECE_GIFT_7 = 0x16,
    ACTMES_STAR_PIECE_GIFT_8 = 0x17,
    ACTMES_STAR_PIECE_GIFT_9 = 0x18,
    ACTMES_STAR_PIECE_GIFT_MAX = 0x19,
    ACTMES_PLAYER_ATTACK_END = 0x1A,
    ACTMES_IS_PUNCH_ENABLE = 0x1B,
    ACTMES_HITMARK_EMIT = 0x1C,
    ACTMES_INVALID_HIT = 0x1D,
    ACTMES_TAKE = 0x1E,
    ACTMES_TAKEN = 0x1F,
    ACTMES_PUT = 0x20,
    ACTMES_APART = 0x21,
    ACTMES_THROW = 0x22,
    ACTMES_DAMAGEDROP = 0x23,
    ACTMES_RUSHDROP = 0x24,
    ACTMES_ATTACKDROP = 0x25,
    ACTMES_HANG_CANCEL = 0x26,
    ACTMES_SHAKE_OFF_BEE = 0x27,
    ACTMES_TAKE_TOUCH = 0x28,
    ACTMES_PUSH = 0x29,
    ACTMES_PUSH_FORCE = 0x2A,
    ACTMES_KICK = 0x2B,
    ACTMES_JUMP = 0x2C,
    ACTMES_TOUCHJUMP = 0x2D,
    ACTMES_BILLIARDJUMP = 0x2E,
    ACTMES_AWAYJUMP = 0x2F,
    ACTMES_BODY_ATTACK = 0x30,
    ACTMES_TORNADO_ATTACK = 0x31,
    ACTMES_TORNADO_STORM_RANGE = 0x32,
    ACTMES_SPIN_STORM_RANGE = 0x33,
    ACTMES_GIANT_ATTACK = 0x34,
    ACTMES_BALL_TOUCH = 0x35,
    ACTMES_BALL_TOUCH_WALL = 0x36,
    ACTMES_BALL_TOUCH_GROUND = 0x37,
    ACTMES_BALL_DASH = 0x38,
    ACTMES_BALL_DASH_WALL = 0x39,
    ACTMES_BALL_DASH_GROUND = 0x3A,
    ACTMES_HAMMER_STAMP = 0x3B,
    ACTMES_BIG_BUBBLE_MERGE = 0x3C,
    ACTMES_BIG_BUBBLE_TOUCH = 0x3D,
    ACTMES_BIG_BUBBLE_ASSIMILATE = 0x3E,
    ACTMES_REFLECT = 0x3F,
    ACTMES_REFLECT_V = 0x40,
    ACTMES_IS_BROKEN = 0x41,
    ACTMES_PASS_RING = 0x42,
    ACTMES_FOUNTAINJUMP = 0x43,
    ACTMES_TERESA_PLAYER_TOUCH = 0x44,
    ACTMES_RUSH_PLAYER_TOUCH = 0x45,
    ACTMES_BOMB_TERESA_TONGUE_TOUCH = 0x46,
    ACTMES_PUDDLE_TOUCH_GROUND = 0x47,
    ACTMES_WARP = 0x48,
    ACTMES_ASK_HIDDEN_BY_SNOW = 0x49,
    ACTMES_NOTIFY_DISCOVER_SNOW = 0x4A,
    ACTMES_ENEMY_ATTACK_START = 0x4B,
    ACTMES_ENEMY_ATTACK_FLIP_VERYWEAK = 0x4C,
    ACTMES_ENEMY_ATTACK_FLIP_WEAK = 0x4D,
    ACTMES_ENEMY_ATTACK_FLIP_WEAK_JUMP = 0x4E,
    ACTMES_ENEMY_ATTACK_FLIP_JUMP = 0x4F,
    ACTMES_ENEMY_ATTACK_FLIP = 0x50,
    ACTMES_ENEMY_ATTACK_FLIP_ROT = 0x51,
    ACTMES_ENEMY_ATTACK_FLIP_MAXIMUM = 0x52,
    ACTMES_ENEMY_ATTACK = 0x53,
    ACTMES_ENEMY_ATTACK_STRONG = 0x54,
    ACTMES_ENEMY_ATTACK_MAXIMUM = 0x55,
    ACTMES_ENEMY_ATTACK_EXPLOSION = 0x56,
    ACTMES_ENEMY_ATTACK_FIREBACK = 0x57,
    ACTMES_ENEMY_ATTACK_FIRERUN = 0x58,
    ACTMES_ENEMY_ATTACK_FIRERUN_STRONG = 0x59,
    ACTMES_ENEMY_ATTACK_ELECTRIC = 0x5A,
    ACTMES_ENEMY_ATTACK_HEATBEAM = 0x5B,
    ACTMES_ENEMY_ATTACK_ACID = 0x5C,
    ACTMES_ENEMY_ATTACK_FREEZE = 0x5D,
    ACTMES_ENEMY_ATTACK_CRUSH = 0x5E,
    ACTMES_ENEMY_ATTACK_EXTRA_DAMAGE = 0x5F,
    ACTMES_ENEMY_ATTACK_COUNTER_SPIN = 0x60,
    ACTMES_ENEMY_ATTACK_COUNTER_HIPDROP = 0x61,
    ACTMES_TO_ENEMY_ATTACK_BLOW = 0x62,
    ACTMES_TO_ENEMY_ATTACK_TRAMPLE = 0x63,
    ACTMES_TO_ENEMY_ATTACK_SHOCK_WAVE = 0x64,
    ACTMES_ENEMY_ATTACK_END = 0x65,
    ACTMES_SUCCESS = 0x66,
    ACTMES_FAILURE = 0x67,
    ACTMES_GROUP_MOVE_START = 0x68,
    ACTMES_GROUP_MOVE_STOP = 0x69,
    ACTMES_GROUP_ATTACK = 0x6A,
    ACTMES_GROUP_SHOW = 0x6B,
    ACTMES_GROUP_HIDE = 0x6C,
    ACTMES_GROUP_LOD_LOW = 0x6D,
    ACTMES_GROUP_LOD_HIGH = 0x6E,
    ACTMES_START_DEMO = 0x6F,
    ACTMES_END_DEMO = 0x70,
    ACTMES_FORCE_KILL = 0x71,
    ACTMES_PAUSE_OFF = 0x72,
    ACTMES_INHALE_BLACK_HOLE = 0x73,
    ACTMES_START_POWER_STAR_GET = 0x74,
    ACTMES_RUSH_PUNCH_RIGHT = 0x75,
    ACTMES_RUSH_PUNCH_LEFT = 0x76,
    ACTMES_RUSH_KICK_RIGHT = 0x77,
    ACTMES_RUSH_KICK_LEFT = 0x78,
    ACTMES_RUSH_THRUST = 0x79,
    ACTMES_RUSH_CRUSH = 0x7A,
    ACTMES_RUSH_SHOOT_UP = 0x7B,
    ACTMES_RUSH_AIR_STRIKE = 0x7C,
    ACTMES_RUSH_GUARD = 0x7D,
    ACTMES_RUSH_GUARD_RIGHT = 0x7E,
    ACTMES_RUSH_GUARD_LEFT = 0x7F,
    ACTMES_RUSH_LIFT_UP = 0x80,
    ACTMES_RUSH_ROTATE_SWING = 0x81,
    ACTMES_RUSH_THROW = 0x82,
    ACTMES_RUSH_PULL_OUT_SUCCESS = 0x83,
    ACTMES_RUSH_PULL_OUT_MISS = 0x84,
    ACTMES_RUSH_FAILURE = 0x85,
    ACTMES_RUSH_EVEN = 0x86,
    ACTMES_ITEM_GET = 0x87,
    ACTMES_IS_PULL_ENABLE = 0x88,
    ACTMES_ITEM_PULL = 0x89,
    ACTMES_ITEM_SHOW = 0x8A,
    ACTMES_ITEM_HIDE = 0x8B,
    ACTMES_ITEM_START_MOVE = 0x8C,
    ACTMES_ITEM_END_MOVE = 0x8D,
    ACTMES_OPD_SHOOT = 0x8E,
    ACTMES_IS_OPD_SITE_IN = 0x8F,
    ACTMES_RESET = 0x90,
    ACTMES_RUSH_BEGIN = 0x91,
    ACTMES_AUTORUSH_BEGIN = 0x92,
    ACTMES_RUSH_CANCEL = 0x93,
    ACTMES_RUSH_FORCE_CANCEL = 0x94,
    ACTMES_RUSH_DAMAGE_CANCEL = 0x95,
    ACTMES_RUSH_END = 0x96,
    ACTMES_IS_RUSH_ENABLE = 0x97,
    ACTMES_IS_RUSH_TAKEOVER = 0x98,
    ACTMES_IS_RUSH_REQUEST = 0x99,
    ACTMES_SLING_SHOOT_START_BIND = 0x9A,
    ACTMES_SLING_SHOOT_ACTOR_HANG_START = 0x9B,
    ACTMES_SLING_SHOOT_ACTOR_HANG_END = 0x9C,
    ACTMES_SLING_SHOOT_ACTOR_APART = 0x9D,
    ACTMES_SLING_SHOOT_ATTACK = 0x9E,
    ACTMES_SLING_SHOOT_PASS_THROUGH_ENABLE = 0x9F,
    ACTMES_SLING_SHOOT_IS_LAST_DAMAGE = 0xA0,
    ACTMES_UPDATE_BASEMTX = 0xA1,
    ACTMES_IS_CAMERA_FIX_POINT = 0xA2,
    ACTMES_BOSSCRAB_SHOOT_DOWN = 0xA3,
    ACTMES_BOSSCRAB_CANNON_BROKEN = 0xA4,
    ACTMES_IKSPIDER_ACTIVE = 0xA5,
    ACTMES_IKSPIDER_CATCH = 0xA6,
    ACTMES_IKSPIDER_LAST_LEG = 0xA7,
    ACTMES_IKSPIDER_RELEASE = 0xA8,
    ACTMES_TRIPODBOSS_LEG_IS_DAMAGE = 0xA9,
    ACTMES_TRIPODBOSS_STARTED = 0xAA,
    ACTMES_DISPERSE_BOMB_TERESA = 0xAB,
    ACTMES_BREAK_POLTA_GROUND_ROCK = 0xAC,
    ACTMES_SPHERE_PLAYER_BINDED = 0xAD,
    ACTMES_SET_UP_JUMP_HOLE = 0xAE,
    ACTMES_SHOOT_JUMP_HOLE = 0xAF,
    ACTMES_END_JUMP_HOLE = 0xB0,
    ACTMES_END_RAIL_DASH = 0xB1,
    ACTMES_END_BALL_RAIL = 0xB2,
    ACTMES_END_BALL_BIND = 0xB3,
    ACTMES_FLOOR_TOUCH = 0xB4,
    ACTMES_WALL_TOUCH = 0xB5,
    ACTMES_CEIL_TOUCH = 0xB6,
    ACTMES_REQUEST_CLAP = 0xB7,
    ACTMES_DINO_PACKUN_PUNCHED_BALL = 0xB8,
    ACTMES_DINO_PACKUN_PULLED_TAIL = 0xB9,
    ACTMES_DINO_PACKUN_BALL_ATTACK = 0xBA,
    ACTMES_TOMB_SPIDER_BATTLE_START = 0xBB,
    ACTMES_TOMB_SPIDER_DEMO_START = 0xBC,
    ACTMES_TOMB_SPIDER_DEMO_END = 0xBD,
    ACTMES_SKELETAL_FISH_BOSS_BATTLE_START = 0xBE,
    ACTMES_SKELETAL_FISH_BOSS_BATTLE_END = 0xBF,
    ACTMES_KOOPA_HIP_DROP_ATTACK = 0xC0,
    ACTMES_KOOPA_PLAYER_DAMAGE = 0xC1,
    ACTMES_KOOPA_PLATE_DAMAGE = 0xC2,
    ACTMES_TICO_RAIL_TOUCH = 0xC3,
    ACTMES_IS_REFLECTION_G_CAPTURE = 0xC4,
    ACTMES_IS_NO_BREAK_G_CAPTURE = 0xC5,
    ACTMES_TURNIP_ATTACK = 0xC6,
    ACTMES_PAKKUN_BALL_ATTACK = 0xC7,
    ACTMES_IS_PUNCH_LEFT = 0xC8,
    ACTMES_IS_PUNCH_CENTER = 0xC9,
    ACTMES_MAPPARTS_ON_PLAYER = 0xCA,
    ACTMES_MAPPARTS_START_ROTATE_AT_POINT = 0xCB,
    ACTMES_MAPPARTS_END_ROTATE_AT_POINT = 0xCC,
    ACTMES_MAPPARTS_START_ROTATE_BETWEEN_POINTS = 0xCD,
    ACTMES_MAPPARTS_BREAK_START = 0xCE,
    ACTMES_MAPPARTS_DISAPPEAR_WITH_BLINK = 0xCF,
    ACTMES_NPC_EVENT_START = 0xD0,
    ACTMES_NPC_EVENT_END = 0xD1,
    ACTMES_NPC_EVENT_TALK_ENABLE = 0xD2,
    ACTMES_NPC_EVENT_TALK_DISABLE = 0xD3,
    ACTMES_NPC_TALK_NEXT = 0xD4,
    ACTMES_MENU_GAME_START = 0xD5,
    ACTMES_MENU_APPEAR_FROM_BLOCK = 0xD6,
    ACTMES_MENU_BLOCK_BREAK = 0xD7,
    ACTMES_MENU_GOAL_POLE_START = 0xD8,
    ACTMES_MENU_GOAL_POLE_END = 0xD9,
    ACTMES_DPD_TOUCH = 0xDA,
    ACTMES_DPD_ATTACK_TARGET_LOCK = 0xDB,
    ACTMES_DPD_ATTACK_TARGET_UNLOCK = 0xDC,
    ACTMES_DPD_TARGET_END = 0xDD,
    ACTMES_DPD_BUTTON_ACTION = 0xDE,
    ACTMES_DPD_SWING_ACTION = 0xDF,
    ACTMES_SPHERE_SELECTOR_SELECT_START = 0xE0,
    ACTMES_SPHERE_SELECTOR_SELECT_END = 0xE1,
    ACTMES_SPHERE_SELECTOR_CONFIRM_START = 0xE2,
    ACTMES_SPHERE_SELECTOR_CONFIRM_CANCEL = 0xE3,
    ACTMES_SPHERE_SELECTOR_CONFIRMED = 0xE4,
    ACTMES_SPHERE_SELECTOR_TARGET_SELECTED = 0xE5,
    ACTMES_TUTORIAL_START = 0xE6,
    ACTMES_TUTORIAL_NEXT = 0xE7,
    ACTMES_TUTORIAL_PREV = 0xE8,
    ACTMES_TUTORIAL_PASS = 0xE9,
    ACTMES_TUTORIAL_END = 0xEA,
    ACTMES_TUTORIAL_OMIT = 0xEB,
    ACTMES_RACE_READY = 0xEC,
    ACTMES_RACE_START = 0xED,
    ACTMES_RACE_GOAL = 0xEE,
    ACTMES_RACE_RESET = 0xEF,
    ACTMES_HEAVENSDOOR_RUNAWAY_RABBIT_WAIT = 0xF0,
    ACTMES_HEAVENSDOOR_RUNAWAY_RABBIT_START = 0xF1,
    ACTMES_LAUNCHER_BREAKABLE = 0xF2,
    ACTMES_PLANT_GROUP_EMIT_ITEM = 0xF3,
};

enum {
    ATYPE_PLAYER_START = 0x0,
    ATYPE_PLAYER = 0x1,
    ATYPE_PUPPETEER = 0x2,
    ATYPE_PLAYER_END = 0x3,
    ATYPE_NPC_START = 0x4,
    ATYPE_NPC = 0x5,
    ATYPE_NPC_END = 0x6,
    ATYPE_ANIMAL = 0x7,
    ATYPE_RIDE_START = 0x8,
    ATYPE_RIDE = 0x9,
    ATYPE_SPHERE_PLAYER = 0xA,
    ATYPE_SPHERE_PLAYER_HIT = 0xB,
    ATYPE_SPRING_ATTACKER = 0xC,
    ATYPE_SPRING_ATTACKER_KINOPIO = 0xD,
    ATYPE_SPRING_ATTACKER_KINOPIO_BIND = 0xE,
    ATYPE_JET_TURTLE = 0xF,
    ATYPE_JET_TURTLE_SLOW = 0x10,
    ATYPE_SPECIAL_WEAPON = 0x11,
    ATYPE_RIDE_END = 0x12,
    ATYPE_ENEMY_START = 0x13,
    ATYPE_ENEMY = 0x14,
    ATYPE_ENEMY_SIMPLE = 0x15,
    ATYPE_ENEMY_ATTACK = 0x16,
    ATYPE_COCO_NUT = 0x17,
    ATYPE_WATER_PRESSURE_BULLET = 0x18,
    ATYPE_BOMBHEI = 0x19,
    ATYPE_TAKOBO = 0x1A,
    ATYPE_KURIBO = 0x1B,
    ATYPE_KARIKARI = 0x1C,
    ATYPE_BEGOMAN = 0x1D,
    ATYPE_KILLER_TARGET_ENEMY = 0x1E,
    ATYPE_PLAYER_AUTO_JUMP = 0x1F,
    ATYPE_MOGUCCHI_REFUSE_TERRITORY = 0x20,
    ATYPE_BIG_BUBBLE = 0x21,
    ATYPE_NOKONOKO = 0x22,
    ATYPE_PUKUPUKU = 0x23,
    ATYPE_UNIZO = 0x24,
    ATYPE_SAMBO_HEAD = 0x25,
    ATYPE_SAMBO_BODY = 0x26,
    ATYPE_HOMING_FIRE = 0x27,
    ATYPE_HOMING_FIRE_SHOOTER = 0x28,
    ATYPE_HOMING_KILLER = 0x29,
    ATYPE_ROCK = 0x2A,
    ATYPE_WANWAN = 0x2B,
    ATYPE_TRIPODBOSS_GUARD_WALL = 0x2C,
    ATYPE_TRIPODBOSS_KILLER_GENERATER = 0x2D,
    ATYPE_TOMB_SPIDER_BODY = 0x2E,
    ATYPE_TOMB_SPIDER_EYE = 0x2F,
    ATYPE_TOMB_SPIDER_HIP = 0x30,
    ATYPE_TOMB_SPIDER_MOUTH = 0x31,
    ATYPE_TOMB_SPIDER_ATTACKER = 0x32,
    ATYPE_TOMB_SPIDER_GLAND_FRONT_L = 0x33,
    ATYPE_TOMB_SPIDER_GLAND_FRONT_L_ATTACKER = 0x34,
    ATYPE_TOMB_SPIDER_GLAND_FRONT_R = 0x35,
    ATYPE_TOMB_SPIDER_GLAND_FRONT_R_ATTACKER = 0x36,
    ATYPE_TOMB_SPIDER_GLAND_REAR_L = 0x37,
    ATYPE_TOMB_SPIDER_GLAND_REAR_L_ATTACKER = 0x38,
    ATYPE_TOMB_SPIDER_GLAND_REAR_R = 0x39,
    ATYPE_TOMB_SPIDER_GLAND_REAR_R_ATTACKER = 0x3A,
    ATYPE_TOMB_SPIDER_VITAL_SPOT_C = 0x3B,
    ATYPE_TOMB_SPIDER_VITAL_SPOT_L = 0x3C,
    ATYPE_TOMB_SPIDER_VITAL_SPOT_R = 0x3D,
    ATYPE_TOMB_SPIDER_COCOON = 0x3E,
    ATYPE_SAND_GOLEM_BLOCK = 0x3F,
    ATYPE_SAND_GOLEM = 0x40,
    ATYPE_KOOPA_ATTACK = 0x41,
    ATYPE_KOOPA_FIRE = 0x42,
    ATYPE_KOOPA_BALL = 0x43,
    ATYPE_ENEMY_END = 0x44,
    ATYPE_MAPOBJ_START = 0x45,
    ATYPE_MAP_OBJ = 0x46,
    ATYPE_MAP_OBJ_SIMPLE = 0x47,
    ATYPE_MAP_OBJ_MOVE_COLLISION = 0x48,
    ATYPE_CLIP_FIELD_MAP_PARTS = 0x49,
    ATYPE_COIN = 0x4A,
    ATYPE_COIN_RED = 0x4B,
    ATYPE_STAR_PIECE = 0x4C,
    ATYPE_KINOKO_ONEUP = 0x4D,
    ATYPE_COINTHROW = 0x4E,
    ATYPE_SWITCH = 0x4F,
    ATYPE_MORPH_ITEM = 0x50,
    ATYPE_GOLF_CUP = 0x51,
    ATYPE_BREAKABLE_CAGE = 0x52,
    ATYPE_BEE_FLOWER = 0x53,
    ATYPE_KILLER_TARGET_MAPOBJ = 0x54,
    ATYPE_PUNCH_BOX = 0x55,
    ATYPE_WATER_BAZOOKA_CAPSULE = 0x56,
    ATYPE_ICEJUMP_WALL = 0x57,
    ATYPE_KEY_SWITCH_AVOID = 0x58,
    ATYPE_KAMECK_BARRIER = 0x59,
    ATYPE_KOOPA_PUSH = 0x5A,
    ATYPE_KOOPA_RECEIVER = 0x5B,
    ATYPE_KOOPA_DAMAGE_PLATE = 0x5C,
    ATYPE_KOOPA_COIN_PLATE = 0x5D,
    ATYPE_KOOPA_PLATE = 0x5E,
    ATYPE_MAPOBJ_END = 0x5F,
    ATYPE_AUTO_RUSH_OBJ_START = 0x60,
    ATYPE_BINDER = 0x61,
    ATYPE_TRANSFERABLE_BINDER = 0x62,
    ATYPE_PRIOR_BINDER = 0x63,
    ATYPE_SPIN_DRIVER_BIND = 0x64,
    ATYPE_SPIN_CANNON_BIND = 0x65,
    ATYPE_SPHERE_PLAYER_BIND = 0x66,
    ATYPE_POWER_STAR_BIND = 0x67,
    ATYPE_G_CAPTURE_BIND = 0x68,
    ATYPE_SPRING_ATTACKER_BIND = 0x69,
    ATYPE_WATER_PRESSURE_BULLET_BIND = 0x6A,
    ATYPE_MARIO_LAUNCHER_BIND = 0x6B,
    ATYPE_QUESTION_COIN_BIND = 0x6C,
    ATYPE_SCENARIO_STARTER_BIND = 0x6D,
    ATYPE_AUTO_RUSH_OBJ_END = 0x6E,
    ATYPE_RUSH_OBJ_START = 0x6F,
    ATYPE_RUSH_ENEMY = 0x70,
    ATYPE_COCO_NUT_BIND = 0x71,
    ATYPE_TURNIP_BIND = 0x72,
    ATYPE_SWITCH_BIND = 0x73,
    ATYPE_RUSH_OBJ_END = 0x74,
    ATYPE_PRESS_OBJ_START = 0x75,
    ATYPE_MAP_OBJ_PRESS = 0x76,
    ATYPE_PRESS_OBJ_END = 0x77,
    ATYPE_SPHERE_MOVER_OBJ_START = 0x78,
    ATYPE_JUMP_HOLE = 0x79,
    ATYPE_SPHERE_DASH = 0x7A,
    ATYPE_BALL_RAIL = 0x7B,
    ATYPE_SPHERE_MOVER_OBJ_END = 0x7C,
    ATYPE_SPECIAL_START = 0x7D,
    ATYPE_SYSTEM_SENSOR = 0x7E,
    ATYPE_EYE = 0x7F,
    ATYPE_PUSH = 0x80,
    ATYPE_DEMO = 0x81,
    ATYPE_SWITCH_MESSENGER = 0x82,
    ATYPE_MESSAGE_SENSOR = 0x83,
    ATYPE_RECEIVER = 0x84,
    ATYPE_SPECIAL_END = 0x85,
};

namespace MR {
    HitSensor* addHitSensor(LiveActor* pActor, const char* pName, u32 type, u16 groupSize, f32 radius, const TVec3f& rOffset);
    HitSensor* addHitSensorBinder(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset);
    HitSensor* addHitSensorTransferableBinder(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset);
    HitSensor* addHitSensorPriorBinder(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset);
    HitSensor* addHitSensorRide(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset);
    HitSensor* addHitSensorMapObj(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset);
    HitSensor* addHitSensorMapObjPress(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset);
    HitSensor* addHitSensorMapObjSimple(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset);
    HitSensor* addHitSensorMapObjMoveCollision(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset);
    HitSensor* addHitSensorEnemy(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset);
    HitSensor* addHitSensorEnemySimple(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset);
    HitSensor* addHitSensorEnemyAttack(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset);
    HitSensor* addHitSensorNpc(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset);
    HitSensor* addHitSensorEye(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset);
    HitSensor* addHitSensorPush(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset);
    HitSensor* addHitSensorPosBinder(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f* pPos, const TVec3f& rOffset);
    HitSensor* addHitSensorPosRide(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f* pPos, const TVec3f& rOffset);
    HitSensor* addHitSensorPosMapObj(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f* pPos, const TVec3f& rOffset);
    HitSensor* addHitSensorPosEye(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f* pPos, const TVec3f& rOffset);
    HitSensor* addHitSensorMtx(LiveActor* pActor, const char* pName, u32 type, u16 groupSize, f32 radius, MtxPtr pMtx, const TVec3f& rOffset);
    HitSensor* addHitSensorMtxRide(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, MtxPtr pMtx, const TVec3f& rOffset);
    HitSensor* addHitSensorMtxMapObj(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, MtxPtr pMtx, const TVec3f& rOffset);
    HitSensor* addHitSensorMtxEnemy(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, MtxPtr pMtx, const TVec3f& rOffset);
    HitSensor* addHitSensorMtxEnemyAttack(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, MtxPtr pMtx, const TVec3f& rOffset);
    HitSensor* addHitSensorMtxNpc(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, MtxPtr pMtx, const TVec3f& rOffset);
    HitSensor* addHitSensorMtxAnimal(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, MtxPtr pMtx, const TVec3f& rOffset);
    HitSensor* addHitSensorAtJoint(LiveActor* pActor, const char* pName, const char* pJointName, u32 type, u16 groupSize, f32 radius,
                                   const TVec3f& rOffset);
    HitSensor* addHitSensorAtJointRide(LiveActor* pActor, const char* pName, const char* pJointName, u16 groupSize, f32 radius,
                                       const TVec3f& rOffset);
    HitSensor* addHitSensorAtJointMapObj(LiveActor* pActor, const char* pName, const char* pJointName, u16 groupSize, f32 radius,
                                         const TVec3f& rOffset);
    HitSensor* addHitSensorAtJointMapObjSimple(LiveActor* pActor, const char* pName, const char* pJointName, u16 groupSize, f32 radius,
                                               const TVec3f& rOffset);
    HitSensor* addHitSensorAtJointEnemy(LiveActor* pActor, const char* pName, const char* pJointName, u16 groupSize, f32 radius,
                                        const TVec3f& rOffset);
    HitSensor* addHitSensorAtJointEnemySimple(LiveActor* pActor, const char* pName, const char* pJointName, u16 groupSize, f32 radius,
                                              const TVec3f& rOffset);
    HitSensor* addHitSensorAtJointEnemyAttack(LiveActor* pActor, const char* pName, const char* pJointName, u16 groupSize, f32 radius,
                                              const TVec3f& rOffset);
    HitSensor* addHitSensorAtJointNpc(LiveActor* pActor, const char* pName, const char* pJointName, u16 groupSize, f32 radius, const TVec3f& rOffset);
    HitSensor* addHitSensorAtJointEye(LiveActor* pActor, const char* pName, const char* pJointName, u16 groupSize, f32 radius, const TVec3f& rOffset);
    HitSensor* addHitSensorCallback(LiveActor* pActor, const char* pName, u32 type, u16 groupSize, f32 radius);
    HitSensor* addHitSensorCallbackBinder(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius);
    HitSensor* addHitSensorCallbackPriorBinder(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius);
    HitSensor* addHitSensorCallbackRide(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius);
    HitSensor* addHitSensorCallbackMapObj(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius);
    HitSensor* addHitSensorCallbackMapObjSimple(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius);
    HitSensor* addHitSensorCallbackEnemy(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius);
    HitSensor* addHitSensorCallbackEnemyAttack(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius);
    HitSensor* addHitSensorCallbackEye(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius);
    HitSensor* addBodyMessageSensorReceiver(LiveActor* pActor);
    HitSensor* addBodyMessageSensorMapObj(LiveActor* pActor);
    HitSensor* addBodyMessageSensorMapObjPress(LiveActor* pActor);
    HitSensor* addBodyMessageSensorMapObjMoveCollision(LiveActor* pActor);
    HitSensor* addBodyMessageSensorEnemy(LiveActor* pActor);
    HitSensor* addMessageSensorReceiver(LiveActor* pActor, const char* pName);
    HitSensor* addMessageSensorMapObj(LiveActor* pActor, const char* pName);
    HitSensor* addMessageSensorMapObjMoveCollision(LiveActor* pActor, const char* pName);
    HitSensor* addMessageSensorEnemy(LiveActor* pActor, const char* pName);
    bool tryUpdateHitSensorsAll(LiveActor* pActor);
    void updateHitSensorsAll(LiveActor* pActor);
    bool isSensorType(const HitSensor* pSensor, u32 type);
    HitSensor* getSensorWithIndex(LiveActor* pActor, int index);
    HitSensor* getTaking(const LiveActor* pActor);
    HitSensor* getTaken(const LiveActor* pActor);
    void setSensorPos(HitSensor* pSensor, const TVec3f& rPos);
    void setSensorOffset(LiveActor* pActor, const char* pName, const TVec3f& rOffset);
    void setSensorRadius(LiveActor* pActor, const char* pName, f32 radius);
    void setHitSensorApart(HitSensor* pSender, HitSensor* pReceiver);
    void validateHitSensors(LiveActor* pActor);
    void invalidateHitSensors(LiveActor* pActor);
    void validateHitSensor(LiveActor* pActor, const char* pName);
    void invalidateHitSensor(LiveActor* pActor, const char* pName);
    bool isValidHitSensor(LiveActor* pActor, const char* pName);
    void clearHitSensors(LiveActor* pActor);
    HitSensor* getSensor(LiveActor* pActor, int index);
    LiveActor* getSensorHost(const HitSensor* pSensor);
    bool isSensor(const HitSensor* pSensor, const char* pName);
    bool isSensorPlayer(const HitSensor* pSensor);
    bool isSensorBinder(const HitSensor* pSensor);
    bool isSensorRide(const HitSensor* pSensor);
    bool isSensorPlayerOrRide(const HitSensor* pSensor);
    bool isSensorEnemy(const HitSensor* pSensor);
    bool isSensorEnemyAttack(const HitSensor* pSensor);
    bool isSensorNpc(const HitSensor* pSensor);
    bool isSensorMapObj(const HitSensor* pSensor);
    bool isSensorAutoRush(const HitSensor* pSensor);
    bool isSensorRush(const HitSensor* pSensor);
    bool isSensorPressObj(const HitSensor* pSensor);
    bool isSensorEye(const HitSensor* pSensor);
    bool isSensorPush(const HitSensor* pSensor);
    bool isSensorItem(const HitSensor* pSensor);
    bool tryGetItem(HitSensor* pSender, HitSensor* pReceiver);
    const TVec3f& getSensorPos(const HitSensor* pSensor);
    void calcSensorDirection(TVec3f* pDir, const HitSensor* pSensor1, const HitSensor* pSensor2);
    void calcSensorDirectionNormalize(TVec3f* pDir, const HitSensor* pSensor1, const HitSensor* pSensor2);
    void calcSensorHorizonNormalize(TVec3f* pHorizon, const TVec3f& rGravity, const HitSensor* pSensor1, const HitSensor* pSensor2);
    HitSensor* getMessageSensor();
    bool sendArbitraryMsg(u32 msg, HitSensor* pReceiver, HitSensor* pSender);
    bool sendMsgPush(HitSensor* pReceiver, HitSensor* pSender);
    bool sendMsgPlayerTrample(HitSensor* pReceiver, HitSensor* pSender);
    bool sendMsgPlayerPunch(HitSensor* pReceiver, HitSensor* pSender);
    bool sendMsgJump(HitSensor* pReceiver, HitSensor* pSender);
    bool sendMsgTouchJump(HitSensor* pReceiver, HitSensor* pSender);
    bool sendMsgTaken(HitSensor* pReceiver, HitSensor* pSender);
    bool sendMsgKick(HitSensor* pReceiver, HitSensor* pSender);
    bool sendMsgAwayJump(HitSensor* pReceiver, HitSensor* pSender);
    bool sendMsgEnemyAttackMsgToDir(u32 msg, HitSensor* pReceiver, HitSensor* pSender, const TVec3f& rDir);
    bool sendMsgEnemyAttackFlipWeak(HitSensor* pReceiver, HitSensor* pSender);
    bool sendMsgEnemyAttackFlipWeakJump(HitSensor* pReceiver, HitSensor* pSender);
    bool sendMsgEnemyAttackFlip(HitSensor* pReceiver, HitSensor* pSender);
    bool sendMsgEnemyAttackFlipToDir(HitSensor* pReceiver, HitSensor* pSender, const TVec3f& rDir);
    bool sendMsgEnemyAttackFlipJump(HitSensor* pReceiver, HitSensor* pSender);
    bool sendMsgEnemyAttackFlipRot(HitSensor* pReceiver, HitSensor* pSender);
    bool sendMsgEnemyAttackFlipMaximum(HitSensor* pReceiver, HitSensor* pSender);
    bool sendMsgEnemyAttackFlipMaximumToDir(HitSensor* pReceiver, HitSensor* pSender, const TVec3f& rDir);
    bool sendMsgEnemyAttack(HitSensor* pReceiver, HitSensor* pSender);
    bool sendMsgEnemyAttackStrong(HitSensor* pReceiver, HitSensor* pSender);
    bool sendMsgEnemyAttackStrongToDir(HitSensor* pReceiver, HitSensor* pSender, const TVec3f& rDir);
    bool sendMsgEnemyAttackFire(HitSensor* pReceiver, HitSensor* pSender);
    bool sendMsgEnemyAttackFireStrong(HitSensor* pReceiver, HitSensor* pSender);
    bool sendMsgEnemyAttackElectric(HitSensor* pReceiver, HitSensor* pSender);
    bool sendMsgEnemyAttackFreeze(HitSensor* pReceiver, HitSensor* pSender);
    bool sendMsgEnemyAttackHeatBeam(HitSensor* pReceiver, HitSensor* pSender);
    bool sendMsgEnemyAttackExplosion(HitSensor* pReceiver, HitSensor* pSender);
    bool sendMsgEnemyAttackCounterSpin(HitSensor* pReceiver, HitSensor* pSender);
    bool sendMsgEnemyAttackCounterHipDrop(HitSensor* pReceiver, HitSensor* pSender);
    bool sendMsgLockOnStarPieceShoot(HitSensor* pReceiver, HitSensor* pSender);
    bool sendMsgStarPieceAttack(HitSensor* pReceiver, HitSensor* pSender);
    bool sendMsgStarPieceGift(HitSensor* pReceiver, HitSensor* pSender, u32 num);
    bool sendMsgEnemyAttackMaximum(HitSensor* pReceiver, HitSensor* pSender);
    bool sendMsgEnemyAttackMaximumToDir(HitSensor* pReceiver, HitSensor* pSender, const TVec3f& rDir);
    bool sendMsgEnemyAttackToBindedSensor(LiveActor* pActor, HitSensor* pSender);
    bool sendMsgEnemyAttackExplosionToBindedSensor(LiveActor* pActor, HitSensor* pSender);
    bool sendSimpleMsgToActor(u32 msg, LiveActor* pActor);
    bool sendMsgToBindedSensor(u32 msg, LiveActor* pActor, HitSensor* pSender);
    bool sendMsgToBindedSensor(u32 msg, HitSensor* pSender);
    bool sendMsgToGroundSensor(u32 msg, HitSensor* pSender);
    bool sendMsgToWallSensor(u32 msg, HitSensor* pSender);
    bool sendMsgStartDemo(LiveActor* pActor);
    bool sendMsgToEnemyAttackBlow(HitSensor* pReceiver, HitSensor* pSender);
    bool sendMsgToEnemyAttackTrample(HitSensor* pReceiver, HitSensor* pSender);
    bool sendMsgToEnemyAttackBlowOrTrample(HitSensor* pReceiver, HitSensor* pSender, f32 ratio);
    bool sendMsgToEnemyAttackShockWave(HitSensor* pReceiver, HitSensor* pSender);
    void sendMsgToAllLiveActor(u32 msg, LiveActor* pActor);
    void sendMsgToGroupMember(u32 msg, LiveActor* pActor, HitSensor* pSensor, const char* pName);
    void sendMsgExplosionToNearActor(HitSensor* pSender, f32 distance);
    bool isInSpinStormRange(u32 msg, HitSensor* pSender, HitSensor* pReceiver, f32 range);
    bool receiveItemShowMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    bool receiveItemHideMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    HitSensor* getGroundSensor(const LiveActor* pActor);
    HitSensor* getRoofSensor(const LiveActor* pActor);
    HitSensor* getWallSensor(const LiveActor* pActor);
    bool isMsgPlayerHitAll(u32 msg);
    bool isMsgPlayerSpinAttack(u32 msg);
    bool isMsgPlayerTrample(u32 msg);
    bool isMsgPlayerHipDrop(u32 msg);
    bool isMsgPlayerHipDropFloor(u32 msg);
    bool isMsgPlayerUpperPunch(u32 msg);
    bool isMsgPlayerKick(u32 msg);
    bool isMsgJetTurtleAttack(u32 msg);
    bool isMsgFireBallAttack(u32 msg);
    bool isMsgSearchlightAttack(u32 msg);
    bool isMsgFreezeAttack(u32 msg);
    bool isMsgInvincibleAttack(u32 msg);
    bool isMsgInvalidHit(u32 msg);
    bool isMsgAutoRushBegin(u32 msg);
    bool isMsgRushBegin(u32 msg);
    bool isMsgUpdateBaseMtx(u32 msg);
    bool isMsgRushCancel(u32 msg);
    bool isMsgIsRushTakeOver(u32 msg);
    bool isMsgFloorTouch(u32 msg);
    bool isMsgWallTouch(u32 msg);
    bool isMsgCeilTouch(u32 msg);
    bool isMsgItemGet(u32 msg);
    bool isMsgItemPull(u32 msg);
    bool isMsgItemShow(u32 msg);
    bool isMsgItemHide(u32 msg);
    bool isMsgItemStartMove(u32 msg);
    bool isMsgItemEndMove(u32 msg);
    bool isMsgInhaleBlackHole(u32 msg);
    bool isMsgEnemyAttack(u32 msg);
    bool isMsgEnemyAttackFire(u32 msg);
    bool isMsgEnemyAttackFireStrong(u32 msg);
    bool isMsgEnemyAttackElectric(u32 msg);
    bool isMsgExplosionAttack(u32 msg);
    bool isMsgToEnemyAttackBlow(u32 msg);
    bool isMsgToEnemyAttackTrample(u32 msg);
    bool isMsgToEnemyAttackShockWave(u32 msg);
    bool isMsgSpinStormRange(u32 msg);
    bool isMsgTutorialStart(u32 msg);
    bool isMsgTutorialNext(u32 msg);
    bool isMsgTutorialPrev(u32 msg);
    bool isMsgTutorialPass(u32 msg);
    bool isMsgTutorialOmit(u32 msg);
    bool isMsgRaceReady(u32 msg);
    bool isMsgRaceStart(u32 msg);
    bool isMsgRaceReset(u32 msg);
    bool isMsgLockOnStarPieceShoot(u32 msg);
    bool isMsgBallDashWall(u32 msg);
    bool isMsgBallDashGround(u32 msg);
    bool isMsgStartPowerStarGet(u32 msg);
    bool isMsgTouchPlantItem(u32 msg);
    bool isMsgHitmarkEmit(u32 msg);
    bool isMsgStarPieceAttack(u32 msg);
    bool isMsgStarPieceReflect(u32 msg);
    bool isMsgStarPieceGift(u32 msg);
    s32 getNumStarPieceGift(u32 msg);
    void calcPosBetweenSensors(TVec3f* pPos, const HitSensor* pSensor1, const HitSensor* pSensor2, f32 offset);
    bool tryForceKillIfMsgStartPowerStarGet(LiveActor* pActor, u32 msg);
    HitSensor* addBodyMessageSensor(LiveActor* pActor, u32 type);
};  // namespace MR
