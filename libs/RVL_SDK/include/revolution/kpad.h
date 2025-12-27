#ifndef KPAD_H
#define KPAD_H

#include <revolution/mtx.h>
#include <revolution/types.h>
#include <revolution/wpad.h>

#ifdef __cplusplus
extern "C" {
#endif

#define KPAD_BUTTON_MASK 0x0000ffff
#define KPAD_BUTTON_RPT 0x80000000

#define KPAD_BTN_RPT_TIME_MAX 20000
#define KPAD_BTN_NO_RPT_DELAY 40000

#define KPAD_CMOS_RESO_WX 128
#define KPAD_CMOS_RESO_WY 96
#define KPAD_CMOS_RESO_CX (KPAD_CMOS_RESO_WX / 2)
#define KPAD_CMOS_RESO_CY (KPAD_CMOS_RESO_WY / 2)

#define KPAD_DPD_RESO_WX WPAD_DPD_IMG_RESO_WX
#define KPAD_DPD_RESO_WY WPAD_DPD_IMG_RESO_WY
#define KPAD_DPD_RESO_CX (KPAD_DPD_RESO_WX / 2)
#define KPAD_DPD_RESO_CY (KPAD_DPD_RESO_WY / 2)

#define KPAD_USE_OBJECTS 2

typedef struct Vec2 {
    f32 x;
    f32 y;
} Vec2;

typedef enum KPADPlayMode { KPAD_PLAY_MODE_LOOSE = 0, KPAD_PLAY_MODE_TIGHT } KPADPlayMode;

typedef union KPADEXStatus {
    struct {
        Vec2 stick;
        Vec acc;
        f32 acc_value;
        f32 acc_speed;
    } fs;

    struct {
        u32 hold;
        u32 trig;
        u32 release;
        Vec2 lstick;
        Vec2 rstick;
        f32 ltrigger;
        f32 rtrigger;
    } cl;
} KPADEXStatus;

typedef struct KPADStatus {
    u32 hold;        // 0x00
    u32 trig;        // 0x04
    u32 release;     // 0x08
    Vec acc;         // 0x0C
    f32 acc_value;   // 0x18
    f32 acc_speed;   // 0x1C
    Vec2 pos;        // 0x20
    Vec2 vec;        // 0x28
    f32 speed;       // 0x30
    Vec2 horizon;    // 0x34
    Vec2 hori_vec;   // 0x3C
    f32 hori_speed;  // 0x44
    f32 dist;        // 0x48
    f32 dist_vec;
    f32 dist_speed;
    Vec2 acc_vertical;
    u8 dev_type;
    s8 wpad_err;
    s8 dpd_valid_fg;
    u8 data_format;
    KPADEXStatus ex_status;
} KPADStatus;

typedef struct KPADUnifiedWpadStatus {
    union {
        WPADStatus core;
        WPADFSStatus fs;
        WPADCLStatus cl;
    } u;
    u8 fmt;
    u8 padding;
} KPADUnifiedWpadStatus;

typedef struct {
    Vec2 center;
    s8 error_fg;
    s8 state_fg;
    s8 _fg_1;
    s8 _fg_2;
} KPADObject;

typedef union {
    KPADStatus k;
    KPADUnifiedWpadStatus w;
} KPADTmpStatus;

typedef void (*KPADControlDpdCallback)(s32 chan, s32 reason);
typedef WPADCallback KPADCallback;
typedef WPADChannel KPADChannel;

typedef struct {
    KPADStatus status;

    f32 pos_play_radius;
    f32 pos_sensitivity;

    f32 hori_play_radius;
    f32 hori_sensitivity;

    f32 dist_play_radius;
    f32 dist_sensitivity;

    f32 acc_play_radius;
    f32 acc_sensitivity;
    f32 dist_org;
    Vec2 accXY_nrm_hori;
    Vec2 sec_nrm_hori;
    Vec2 center_org;
    f32 dpd2pos_scale;
    KPADObject kobj_sample[4];
    KPADObject kobj_regular[2];
    s16 valid_objs;
    u8 bufIdx;
    u8 bufCount;
    KPADUnifiedWpadStatus uniRingBuf[120];
    f32 sec_length;
    Vec2 sec_nrm;
    f32 sec_dist;
    f32 trust_sec_length;
    Vec hard_acc;
    Vec2 obj_horizon;
    Vec2 acc_horizon;
    Vec2 ah_circle_pos;
    u16 ah_circle_ct;
    u8 dpd_valid2_ct;
    s8 _fg_4;
    u16 btn_repeat_time;
    u16 btn_repeat_next;
    u16 btn_repeat_delay;
    u16 btn_repeat_pulse;
    u16 btn_cl_repeat_time;
    u16 btn_cl_repeat_next;
    KPADControlDpdCallback dpd_ctrl_callback;
    f32 acc_scale_x;
    f32 acc_scale_y;
    f32 acc_scale_z;
    f32 fs_acc_scale_x;
    f32 fs_acc_scale_y;
    f32 fs_acc_scale_z;
    Vec2 kobj_frame_min;
    Vec2 kobj_frame_max;
    f32 err_dist_speed_1;
    f32 err_dist_speedM_1;
    f32 ah_circle_radius2;
    f32 dist_vv1;
    f32 err_dist_min;
    WPADSamplingCallback appSamplingCallback;
    u8 readLocked;
    u8 resetReq;
    u8 exResetReq;
    u8 dpdIssued;
    u8 dpdEnabled;
    u8 dpdCmd;
    u8 dpdPreCallbackDone;
    u8 dpdPostCallbackDone;
    u8 aimReq;
    u8 aimEnabled;
    u8 fsAccRevise;
    u8 padding;
    KPADPlayMode pos_play_mode;
    KPADPlayMode hori_play_mode;
    KPADPlayMode dist_play_mode;
    KPADPlayMode acc_play_mode;

} KPADInsideStatus;

void KPADInit();

void KPADSetBtnRepeat(s32, f32, f32);
void KPADSetSensorHeight(s32, f32);
void KPADSetPosParam(s32, f32, f32);
void KPADSetHoriParam(s32, f32, f32);
void KPADSetDistParam(s32, f32, f32);

void KPADReset(void);
s32 KPADRead(s32, KPADStatus[], u32);

extern KPADInsideStatus inside_kpads[];

#ifdef __cplusplus
}
#endif

#endif  // KPAD_H
