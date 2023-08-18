#ifndef KPAD_H
#define KPAD_H

#include <revolution.h>
#include <revolution/mtx.h>

#ifdef __cplusplus
extern "C" {
#endif

struct Vec2 {
    f32 x;
    f32 y;
};

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
    u32 hold;
    u32 trig;
    u32 release;
    Vec acc;
    f32 acc_value;
    f32 acc_speed;
    Vec2 pos;
    Vec2 vec;
    f32 speed;
    Vec2 horizon;
    Vec2 hori_vec;
    f32 hori_speed;
    f32 dist;
    f32 dist_vec;
    f32 dist_speed;
    Vec2 acc_vertical;
    u8 dev_type;
    s8 wpad_err;
    s8 dpd_valid_fg;
    u8 data_format;
    KPADEXStatus ex_status;
} KPADStatus;

void KPADSetBtnRepeat(s32, f32, f32);
void KPADSetSensorHeight(s32, f32);
void KPADSetPosParam(s32, f64, f64);
void KPADSetHoriParam(s32, f64, f64);
void KPADSetDistParam(s32, f64, f64);

#ifdef __cplusplus
}
#endif

#endif // KPAD_H