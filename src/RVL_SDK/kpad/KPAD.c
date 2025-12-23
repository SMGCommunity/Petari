#include "revolution/kpad.h"
#include "revolution/mtx.h"
#include <cmath>
#include <cstdio>

static Vec2 icenter_org = {0.000f, 0.000f};
static f32 idist_org = 1.000f;
static Vec2 iaccXY_nrm_hori = {0.000f, -1.000f};
static Vec2 isec_nrm_hori = {1.000f, 0.000f};
f32 kp_obj_interval = 0.200f;

f32 kp_acc_horizon_pw = 0.050f;
f32 kp_ah_circle_radius = 0.070f;
f32 kp_ah_circle_pw = 0.060f;
u16 kp_ah_circle_ct = 100;
BOOL kp_stick_clamp_cross = FALSE;

f32 kp_err_outside_frame = 0.050f;
f32 kp_err_dist_min;
f32 kp_err_dist_max = 3.000f;
f32 kp_err_dist_speed = 0.040f;
f32 kp_err_first_inpr = 0.900f;
f32 kp_err_next_inpr = 0.900f;
f32 kp_err_acc_inpr = 0.900f;
f32 kp_err_up_inpr = 0.700f;
f32 kp_err_near_pos = 0.100f;

static Vec2 kobj_frame_min, kobj_frame_max;
static f32 kp_err_dist_speed_1;
static f32 kp_err_dist_speedM_1;
static f32 kp_ah_circle_radius2;
static f32 kp_dist_vv1;

static s32 kp_fs_fstick_min = 15;
static s32 kp_fs_fstick_max = 71;
static s32 kp_cl_stick_min = 60;
static s32 kp_cl_stick_max = 308;
static s32 kp_cl_trigger_min = 30;
static s32 kp_cl_trigger_max = 180;
static s32 kp_gc_mstick_min = 15;
static s32 kp_gc_mstick_max = 77;
static s32 kp_gc_cstick_min = 15;
static s32 kp_gc_cstick_max = 64;
static s32 kp_gc_trigger_min = 30;
static s32 kp_gc_trigger_max = 180;
static f32 kp_rm_acc_max = 3.4f;
static f32 kp_fs_acc_max = 2.1f;

KPADInsideStatus inside_kpads[4];
static Vec2 Vec2_0 = {0.0f, 0.0f};
static Mtx kp_fs_rot;
static f32 kp_fs_revise_deg = 24.0f;

static const char* __KPADVersion = "<< RVL_SDK - KPAD \trelease build: Jun  3 2008 11:53:11 (0x4201_134) >>";

void KPADSetBtnRepeat(s32 chan, f32 delay_sec, f32 pulse_sec) {
    KPADInsideStatus* kp = &inside_kpads[chan];
    if (pulse_sec) {
        kp->btn_repeat_delay = (u16)(s32)(delay_sec * 200.0f + 0.5f);
        kp->btn_repeat_pulse = (u16)(s32)(pulse_sec * 200.0f + 0.5f);
    } else {
        kp->btn_repeat_delay = KPAD_BTN_NO_RPT_DELAY;
        kp->btn_repeat_pulse = 0;
    }

    kp->btn_repeat_time = 0;
    kp->btn_repeat_next = kp->btn_repeat_delay;
    kp->btn_cl_repeat_time = 0;
    kp->btn_cl_repeat_next = kp->btn_repeat_delay;
}

void KPADSetPosParam(s32 chan, f32 play_radius, f32 sensitivity) {
    inside_kpads[chan].pos_play_radius = play_radius;
    inside_kpads[chan].pos_sensitivity = sensitivity;
}

void KPADSetHoriParam(s32 chan, f32 play_radius, f32 sensitivity) {
    inside_kpads[chan].hori_play_radius = play_radius;
    inside_kpads[chan].hori_sensitivity = sensitivity;
}

void KPADSetDistParam(s32 chan, f32 play_radius, f32 sensitivity) {
    inside_kpads[chan].dist_play_radius = play_radius;
    inside_kpads[chan].dist_sensitivity = sensitivity;
}

void KPADSetAccParam(s32 chan, f32 play_radius, f32 sensitivity) {
    inside_kpads[chan].acc_play_radius = play_radius;
    inside_kpads[chan].acc_sensitivity = sensitivity;
}

void KPADEnableAimingMode(s32 chan) {
    inside_kpads[chan].aimReq = TRUE;
    inside_kpads[chan].aimEnabled = TRUE;
}

void KPADSetObjInterval(f32 interval) {
    BOOL enabled = OSDisableInterrupts();
    kp_obj_interval = interval;
    kp_err_dist_min = interval / 0.383864f;
    kp_dist_vv1 = interval / 0.383864f;
    OSRestoreInterrupts(enabled);
}

static void calc_dpd2pos_scale(KPADInsideStatus* kp) {
    f32 scale;
    f32 sx, sy;
    sx = 1.0f;
    sy = (f32)KPAD_DPD_RESO_WY / (f32)KPAD_DPD_RESO_WX;

    scale = sqrt(sx * sx + sy * sy);

    if (kp->center_org.x < 0.0f) {
        sx += kp->center_org.x;
    } else {
        sx -= kp->center_org.x;
    }

    if (kp->center_org.y < 0.0f) {
        sy += kp->center_org.y;
    } else {
        sy -= kp->center_org.y;
    }

    kp->dpd2pos_scale = scale / ((sx < sy) ? sx : sy);
}

void reset_kpad(KPADInsideStatus* kp) {
    KPADObject* op;
    KPADStatus* sp = &kp->status;
    KPADEXStatus* ep = &kp->status.ex_status;

    kp->resetReq = FALSE;

    kp->kobj_frame_min.x = -1.0f + kp_err_outside_frame;
    kp->kobj_frame_max.x = 1.0f - kp_err_outside_frame;
    kp->kobj_frame_min.y = -((f32)KPAD_DPD_RESO_WY / KPAD_DPD_RESO_WX) + kp_err_outside_frame;
    kp->kobj_frame_max.y = ((f32)KPAD_DPD_RESO_WY / KPAD_DPD_RESO_WX) - kp_err_outside_frame;

    kp->err_dist_speed_1 = 1.0f / kp_err_dist_speed;
    kp->err_dist_speedM_1 = -1.0f / kp_err_dist_speed;
    kp->ah_circle_radius2 = kp_ah_circle_radius * kp_ah_circle_radius;

    kp->err_dist_min = kp_err_dist_min;
    kp->dist_vv1 = kp_dist_vv1;

    sp->hold = sp->trig = sp->release = 0;
    kp->btn_repeat_time = 0;
    kp->btn_repeat_next = kp->btn_repeat_delay;

    sp->dpd_valid_fg = 0;
    kp->dpd_valid2_ct = 0;

    sp->pos = sp->vec = Vec2_0;
    sp->speed = 0.0f;

    sp->horizon.x = kp->acc_horizon.x = kp->obj_horizon.x = 1.0f;
    sp->horizon.y = kp->acc_horizon.y = kp->obj_horizon.y = 0.0f;
    sp->hori_vec = Vec2_0;
    sp->hori_speed = 0.0f;

    sp->acc_vertical.x = 1.0f;
    sp->acc_vertical.y = 0.0f;

    sp->dist = kp->dist_org;
    sp->dist_vec = sp->dist_speed = 0.0f;

    kp->sec_dist = sp->dist;
    kp->sec_length = kp->trust_sec_length = kp->dist_vv1 / kp->sec_dist;
    kp->sec_nrm = kp->sec_nrm_hori;

    sp->acc.x = sp->acc.z = 0.0f;
    sp->acc.y = -1.0f;
    sp->acc_value = 1.0f;
    sp->acc_speed = 0.0f;
    kp->hard_acc = sp->acc;

    kp->ah_circle_pos = kp->acc_horizon;
    kp->ah_circle_ct = kp_ah_circle_ct;

    kp->valid_objs = 0;

    op = &kp->kobj_sample[WPAD_DPD_MAX_OBJECTS - 1];
    do {
        op->error_fg = -1;
    } while (--op >= kp->kobj_sample);

    op = &kp->kobj_regular[KPAD_USE_OBJECTS - 1];
    do {
        op->error_fg = -1;
    } while (--op >= kp->kobj_regular);

    kp->bufCount = 0;
    kp->exResetReq = TRUE;
}

void KPADSetSensorHeight(s32 chan, f32 level) {
    KPADInsideStatus* kp = &inside_kpads[chan];

    kp->center_org.x = 0.0f;
    kp->center_org.y = -level;
    calc_dpd2pos_scale(kp);
}

void calc_button_repeat(KPADInsideStatus* kp, u32 dev_type, u32 count) {
    KPADStatus* sp = &kp->status;
    KPADEXStatus* ep = &kp->status.ex_status;

    if (sp->trig != 0 || sp->release != 0) {
        kp->btn_repeat_time = 0;
        kp->btn_repeat_next = kp->btn_repeat_delay;
        if (sp->trig && kp->btn_repeat_pulse) {
            sp->hold |= KPAD_BUTTON_RPT;
        }
    } else if (sp->hold != 0) {
        kp->btn_repeat_time += count;
        if (kp->btn_repeat_time >= KPAD_BTN_NO_RPT_DELAY) {
            kp->btn_repeat_time -= KPAD_BTN_NO_RPT_DELAY;
        }

        if (kp->btn_repeat_time >= kp->btn_repeat_next) {
            sp->hold |= KPAD_BUTTON_RPT;
            kp->btn_repeat_next += kp->btn_repeat_pulse;
            if (kp->btn_repeat_time >= KPAD_BTN_RPT_TIME_MAX) {
                kp->btn_repeat_time -= KPAD_BTN_RPT_TIME_MAX;
                kp->btn_repeat_next -= KPAD_BTN_RPT_TIME_MAX;
            }
        }
    }

    if (dev_type == WPAD_DEV_CLASSIC) {
        if (ep->cl.trig != 0 || ep->cl.release != 0) {
            kp->btn_cl_repeat_time = 0;
            kp->btn_cl_repeat_next = kp->btn_repeat_delay;
            if (ep->cl.trig && kp->btn_repeat_pulse) {
                ep->cl.hold |= KPAD_BUTTON_RPT;
            }
        } else if (ep->cl.hold != 0) {
            kp->btn_cl_repeat_time += count;
            if (kp->btn_cl_repeat_time >= KPAD_BTN_NO_RPT_DELAY) {
                kp->btn_cl_repeat_time -= KPAD_BTN_NO_RPT_DELAY;
            }

            if (kp->btn_cl_repeat_time >= kp->btn_cl_repeat_next) {
                ep->cl.hold |= KPAD_BUTTON_RPT;
                kp->btn_cl_repeat_next += kp->btn_repeat_pulse;
                if (kp->btn_cl_repeat_time >= KPAD_BTN_RPT_TIME_MAX) {
                    kp->btn_cl_repeat_time -= KPAD_BTN_RPT_TIME_MAX;
                    kp->btn_cl_repeat_next -= KPAD_BTN_RPT_TIME_MAX;
                }
            }
        }
    }
}

void calc_acc(KPADInsideStatus* kp, f32* acc, f32 acc2) {
    f32 f1, f2;

    f2 = acc2 - *acc;

    if (kp->acc_play_mode == KPAD_PLAY_MODE_LOOSE) {
        if (f2 < 0.0f) {
            f1 = -f2;
        } else {
            f1 = f2;
        }

        if (f1 >= kp->acc_play_radius) {
            f1 = 1.0f;
        } else {
            f1 /= kp->acc_play_radius;
            f1 *= f1;
            f1 *= f1;
        }
        f1 *= kp->acc_sensitivity;
        *acc += f1 * f2;
    } else {
        if (f2 < -kp->acc_play_radius) {
            *acc += (f2 + kp->acc_play_radius) * kp->acc_sensitivity;
        } else if (f2 > kp->acc_play_radius) {
            *acc += (f2 - kp->acc_play_radius) * kp->acc_sensitivity;
        }
    }
}

void calc_acc_horizon(KPADInsideStatus* kp) {
    f32 f1, vx, vy, ax, ay;
    f1 = sqrt(kp->hard_acc.x * kp->hard_acc.x + kp->hard_acc.y * kp->hard_acc.y);
    if (f1 == 0.0f || f1 >= 2.0f)
        return;
    ax = kp->hard_acc.x / f1;
    ay = kp->hard_acc.y / f1;

    if (f1 > 1.0f) {
        f1 = 2.0f - f1;
    }
    f1 *= f1 * kp_acc_horizon_pw;

    vx = kp->accXY_nrm_hori.x * ax + kp->accXY_nrm_hori.y * ay;
    vy = kp->accXY_nrm_hori.y * ax - kp->accXY_nrm_hori.x * ay;
    ax = (vx - kp->acc_horizon.x) * f1 + kp->acc_horizon.x;
    ay = (vy - kp->acc_horizon.y) * f1 + kp->acc_horizon.y;

    f1 = sqrt(ax * ax + ay * ay);
    if (f1 == 0.0f)
        return;
    kp->acc_horizon.x = ax / f1;
    kp->acc_horizon.y = ay / f1;

    kp->ah_circle_pos.x += (kp->acc_horizon.x - kp->ah_circle_pos.x) * kp_ah_circle_pw;
    kp->ah_circle_pos.y += (kp->acc_horizon.y - kp->ah_circle_pos.y) * kp_ah_circle_pw;

    vx = kp->acc_horizon.x - kp->ah_circle_pos.x;
    vy = kp->acc_horizon.y - kp->ah_circle_pos.y;
    if (vx * vx + vy * vy <= kp->ah_circle_radius2) {
        if (kp->ah_circle_ct)
            --kp->ah_circle_ct;
    } else {
        kp->ah_circle_ct = kp_ah_circle_ct;
    }
}

static void clamp_trigger(f32* trigger, s32 tr, s32 min, s32 max) {
    if (tr <= min) {
        *trigger = 0.0f;
    } else if (tr >= max) {
        *trigger = 1.0f;
    } else {
        *trigger = (f32)(tr - min) / (f32)(max - min);
    }
}

void calc_acc_vertical(KPADInsideStatus* kp) {
    KPADStatus* sp = &kp->status;
    f32 f1, f2, ax, ay;

    ax = sqrt(f2 = kp->hard_acc.x * kp->hard_acc.x + kp->hard_acc.y * kp->hard_acc.y);
    ay = -kp->hard_acc.z;
    f1 = sqrt(f2 + ay * ay);
    if (f1 == 0.0f || f1 >= 2.0f)
        return;
    ax /= f1;
    ay /= f1;

    if (f1 > 1.0f) {
        f1 = 2.0f - f1;
    }
    f1 *= f1 * kp_acc_horizon_pw;

    ax = (ax - sp->acc_vertical.x) * f1 + sp->acc_vertical.x;
    ay = (ay - sp->acc_vertical.y) * f1 + sp->acc_vertical.y;

    f1 = sqrt(ax * ax + ay * ay);
    if (f1 == 0.0f)
        return;
    sp->acc_vertical.x = ax / f1;
    sp->acc_vertical.y = ay / f1;
}

static f32 clamp_acc(f32 acc, f32 clamp) {
    if (acc < 0.0f) {
        if (acc < -clamp)
            return -clamp;
    } else {
        if (acc > clamp)
            return clamp;
    }
    return acc;
}

void read_kpad_acc(KPADInsideStatus* kp, KPADUnifiedWpadStatus* uwp) {
    KPADStatus* sp = &kp->status;
    Vec fsrc;
    Vec vec;

    switch (uwp->fmt) {
    case WPAD_FMT_CORE_ACC:
    case WPAD_FMT_CORE_ACC_DPD:
    case WPAD_FMT_FREESTYLE_ACC:
    case WPAD_FMT_FREESTYLE_ACC_DPD:
    case WPAD_FMT_CLASSIC_ACC:
    case WPAD_FMT_CLASSIC_ACC_DPD:
        break;

    default:
        return;
    }

    kp->hard_acc.x = clamp_acc((f32)(s32)-uwp->u.core.accX * kp->acc_scale_x, kp_rm_acc_max);
    kp->hard_acc.y = clamp_acc((f32)(s32)-uwp->u.core.accZ * kp->acc_scale_z, kp_rm_acc_max);
    kp->hard_acc.z = clamp_acc((f32)(s32)uwp->u.core.accY * kp->acc_scale_y, kp_rm_acc_max);

    vec = sp->acc;

    calc_acc(kp, &sp->acc.x, kp->hard_acc.x);
    calc_acc(kp, &sp->acc.y, kp->hard_acc.y);
    calc_acc(kp, &sp->acc.z, kp->hard_acc.z);
    sp->acc_value = sqrt(sp->acc.x * sp->acc.x + sp->acc.y * sp->acc.y + sp->acc.z * sp->acc.z);

    vec.x -= sp->acc.x;
    vec.y -= sp->acc.y;
    vec.z -= sp->acc.z;
    sp->acc_speed = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    calc_acc_horizon(kp);
    calc_acc_vertical(kp);
    if (uwp->u.fs.err != WPAD_ERR_NONE || uwp->u.fs.dev != WPAD_DEV_FREESTYLE ||
        (uwp->fmt != WPAD_FMT_FREESTYLE_ACC && uwp->fmt != WPAD_FMT_FREESTYLE_ACC_DPD)) {
        return;
    }

    fsrc.x = clamp_acc((f32)(s32)-uwp->u.fs.fsAccX * kp->fs_acc_scale_x, kp_fs_acc_max);
    fsrc.y = clamp_acc((f32)(s32)-uwp->u.fs.fsAccZ * kp->fs_acc_scale_z, kp_fs_acc_max);
    fsrc.z = clamp_acc((f32)(s32)uwp->u.fs.fsAccY * kp->fs_acc_scale_y, kp_fs_acc_max);

    if (kp->fsAccRevise) {
        PSMTXMultVec(kp_fs_rot, &fsrc, &fsrc);
    }

    vec = sp->ex_status.fs.acc;

    calc_acc(kp, &sp->ex_status.fs.acc.x, fsrc.x);
    calc_acc(kp, &sp->ex_status.fs.acc.y, fsrc.y);
    calc_acc(kp, &sp->ex_status.fs.acc.z, fsrc.z);
    sp->ex_status.fs.acc_value = sqrt(sp->ex_status.fs.acc.x * sp->ex_status.fs.acc.x + sp->ex_status.fs.acc.y * sp->ex_status.fs.acc.y +
                                      sp->ex_status.fs.acc.z * sp->ex_status.fs.acc.z);

    vec.x -= sp->ex_status.fs.acc.x;
    vec.y -= sp->ex_status.fs.acc.y;
    vec.z -= sp->ex_status.fs.acc.z;
    sp->ex_status.fs.acc_speed = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

static f32 calc_horizon(KPADInsideStatus* kp, Vec2* p1, Vec2* p2, Vec2* hori) {
    f32 f1, f2, vx, vy;

    vx = p2->x - p1->x;
    vy = p2->y - p1->y;
    f2 = sqrt(vx * vx + vy * vy);
    f1 = 1.0f / f2;
    vx *= f1;
    vy *= f1;

    hori->x = kp->sec_nrm_hori.x * vx + kp->sec_nrm_hori.y * vy;
    hori->y = kp->sec_nrm_hori.y * vx - kp->sec_nrm_hori.x * vy;

    return kp->dist_vv1 * f1;
}

s8 select_2obj_first(KPADInsideStatus* kp) {
    KPADObject *op1, *op2, *rp1, *rp2;
    Vec2 hori;
    f32 f1, max = kp_err_first_inpr;

    op1 = kp->kobj_sample;
    do {
        if (op1->error_fg != 0)
            continue;

        op2 = op1 + 1;
        do {
            if (op2->error_fg != 0)
                continue;

            f1 = calc_horizon(kp, &op1->center, &op2->center, &hori);

            if (f1 <= kp->err_dist_min || f1 >= kp_err_dist_max)
                continue;

            f1 = kp->acc_horizon.x * hori.x + kp->acc_horizon.y * hori.y;
            if (f1 < 0.0f) {
                if (-f1 > max) {
                    max = -f1;
                    rp1 = op2;
                    rp2 = op1;
                }
            } else {
                if (f1 > max) {
                    max = f1;
                    rp1 = op1;
                    rp2 = op2;
                }
            }

        } while (++op2 <= &kp->kobj_sample[WPAD_DPD_MAX_OBJECTS - 1]);
    } while (++op1 < &kp->kobj_sample[WPAD_DPD_MAX_OBJECTS - 1]);

    if (max == kp_err_first_inpr)
        return 0;

    kp->kobj_regular[0] = *rp1;
    kp->kobj_regular[1] = *rp2;

    return 2;
}

s8 select_2obj_continue(KPADInsideStatus* kp) {
    KPADObject *op1, *op2, *rp1, *rp2;
    Vec2 nrm;
    s32 rev_fg;
    f32 f1, f2, f3, vx, vy, min = 2.0f;
    op1 = kp->kobj_sample;
    do {
        if (op1->error_fg != 0)
            continue;

        op2 = op1 + 1;
        do {
            if (op2->error_fg != 0)
                continue;
            vx = op2->center.x - op1->center.x;
            vy = op2->center.y - op1->center.y;
            f3 = sqrt(vx * vx + vy * vy);
            f1 = 1.0f / f3;
            nrm.x = vx * f1;
            nrm.y = vy * f1;

            f1 *= kp->dist_vv1;
            if (f1 <= kp->err_dist_min || f1 >= kp_err_dist_max)
                continue;

            f1 -= kp->sec_dist;
            if (f1 < 0.0f) {
                f1 *= kp->err_dist_speedM_1;
            } else {
                f1 *= kp->err_dist_speed_1;
            }
            if (f1 >= 1.0f)
                continue;

            f2 = kp->sec_nrm.x * nrm.x + kp->sec_nrm.y * nrm.y;
            if (f2 < 0.0f) {
                f2 = -f2;
                rev_fg = 1;
            } else {
                rev_fg = 0;
            }
            if (f2 <= kp_err_next_inpr)
                continue;
            f2 = (1.0f - f2) / (1.0f - kp_err_next_inpr);
            f1 += f2;
            if (f1 < min) {
                min = f1;
                if (rev_fg) {
                    rp1 = op2;
                    rp2 = op1;
                } else {
                    rp1 = op1;
                    rp2 = op2;
                }
            }

        } while (++op2 <= &kp->kobj_sample[WPAD_DPD_MAX_OBJECTS - 1]);
    } while (++op1 < &kp->kobj_sample[WPAD_DPD_MAX_OBJECTS - 1]);

    if (min == 2.0f)
        return 0;

    kp->kobj_regular[0] = *rp1;
    kp->kobj_regular[1] = *rp2;

    return 2;
}

s8 select_1obj_first(KPADInsideStatus* kp) {
    KPADObject* op1;
    f32 vx, vy;
    Vec2 p1, p2;

    vx = kp->sec_nrm_hori.x * kp->acc_horizon.x + kp->sec_nrm_hori.y * kp->acc_horizon.y;
    vy = kp->sec_nrm_hori.y * kp->acc_horizon.x - kp->sec_nrm_hori.x * kp->acc_horizon.y;

    vx *= kp->trust_sec_length;
    vy *= kp->trust_sec_length;

    op1 = kp->kobj_sample;
    do {
        if (op1->error_fg != 0)
            continue;

        p1.x = op1->center.x - vx;
        p1.y = op1->center.y - vy;
        p2.x = op1->center.x + vx;
        p2.y = op1->center.y + vy;

        if (p1.x <= kp->kobj_frame_min.x || p1.x >= kp->kobj_frame_max.x || p1.y <= kp->kobj_frame_min.y || p1.y >= kp->kobj_frame_max.y) {
            if (p2.x > kp->kobj_frame_min.x && p2.x < kp->kobj_frame_max.x && p2.y > kp->kobj_frame_min.y && p2.y < kp->kobj_frame_max.y) {
                kp->kobj_regular[1] = *op1;
                kp->kobj_regular[0].center = p1;
                kp->kobj_regular[0].error_fg = 0;
                kp->kobj_regular[0].state_fg = -1;

                return -1;
            }
        } else {
            if (p2.x <= kp->kobj_frame_min.x || p2.x >= kp->kobj_frame_max.x || p2.y <= kp->kobj_frame_min.y || p2.y >= kp->kobj_frame_max.y) {
                kp->kobj_regular[0] = *op1;

                kp->kobj_regular[1].center = p2;
                kp->kobj_regular[1].error_fg = 0;
                kp->kobj_regular[1].state_fg = -1;

                return -1;
            }
        }

    } while (++op1 < &kp->kobj_sample[WPAD_DPD_MAX_OBJECTS]);

    return 0;
}

s8 select_1obj_continue(KPADInsideStatus* kp) {
    KPADObject *op1, *op2, *rp1, *rp2;
    f32 f1, vx, vy;
    f32 min = kp_err_near_pos * kp_err_near_pos;

    op1 = kp->kobj_regular;
    do {
        if (op1->error_fg != 0)
            continue;
        if (op1->state_fg != 0)
            continue;

        op2 = kp->kobj_sample;
        do {
            if (op2->error_fg != 0)
                continue;

            vx = op1->center.x - op2->center.x;
            vy = op1->center.y - op2->center.y;
            f1 = vx * vx + vy * vy;
            if (f1 < min) {
                min = f1;
                rp1 = op1;
                rp2 = op2;
            }
        } while (++op2 < &kp->kobj_sample[WPAD_DPD_MAX_OBJECTS]);
    } while (++op1 < &kp->kobj_regular[KPAD_USE_OBJECTS]);

    if (min == kp_err_near_pos * kp_err_near_pos)
        return 0;

    *rp1 = *rp2;
    kp->sec_nrm.x = kp->sec_nrm_hori.x * kp->acc_horizon.x + kp->sec_nrm_hori.y * kp->acc_horizon.y;
    kp->sec_nrm.y = kp->sec_nrm_hori.y * kp->acc_horizon.x - kp->sec_nrm_hori.x * kp->acc_horizon.y;

    vx = kp->sec_length * kp->sec_nrm.x;
    vy = kp->sec_length * kp->sec_nrm.y;
    if (rp1 == &kp->kobj_regular[0]) {
        kp->kobj_regular[1].center.x = rp1->center.x + vx;
        kp->kobj_regular[1].center.y = rp1->center.y + vy;
        kp->kobj_regular[1].error_fg = 0;
        kp->kobj_regular[1].state_fg = -1;
    } else {
        kp->kobj_regular[0].center.x = rp1->center.x - vx;
        kp->kobj_regular[0].center.y = rp1->center.y - vy;
        kp->kobj_regular[0].error_fg = 0;
        kp->kobj_regular[0].state_fg = -1;
    }

    if (kp->status.dpd_valid_fg < 0) {
        return -1;
    } else {
        return 1;
    }
}

void calc_dpd_variable(KPADInsideStatus* kp, s8 valid_fg_next) {
    KPADStatus* sp = &kp->status;
    f32 f1, f2, dist;
    Vec2 pos, vec;

    if (valid_fg_next == 0) {
        sp->dpd_valid_fg = 0;
        return;
    }

    pos.x = kp->sec_nrm_hori.x * kp->sec_nrm.x + kp->sec_nrm_hori.y * kp->sec_nrm.y;
    pos.y = kp->sec_nrm_hori.y * kp->sec_nrm.x - kp->sec_nrm_hori.x * kp->sec_nrm.y;

    if (sp->dpd_valid_fg == 0) {
        sp->horizon = pos;
        sp->hori_vec = Vec2_0;
        sp->hori_speed = 0.0f;
    } else {
        vec.x = pos.x - sp->horizon.x;
        vec.y = pos.y - sp->horizon.y;
        f1 = sqrt(vec.x * vec.x + vec.y * vec.y);

        if (kp->hori_play_mode == KPAD_PLAY_MODE_LOOSE) {
            if (f1 >= kp->hori_play_radius) {
                f1 = 1.0f;
            } else {
                f1 /= kp->hori_play_radius;
                f1 *= f1;
                f1 *= f1;
            }
            f1 *= kp->hori_sensitivity;
            vec.x = f1 * vec.x + sp->horizon.x;
            vec.y = f1 * vec.y + sp->horizon.y;
            f1 = sqrt(vec.x * vec.x + vec.y * vec.y);
            vec.x /= f1;
            vec.y /= f1;

            sp->hori_vec.x = vec.x - sp->horizon.x;
            sp->hori_vec.y = vec.y - sp->horizon.y;
            sp->hori_speed = sqrt(sp->hori_vec.x * sp->hori_vec.x + sp->hori_vec.y * sp->hori_vec.y);

            sp->horizon = vec;
        } else {
            if (f1 > kp->hori_play_radius) {
                f1 = (f1 - kp->hori_play_radius) / f1 * kp->hori_sensitivity;
                vec.x = vec.x * f1 + sp->horizon.x;
                vec.y = vec.y * f1 + sp->horizon.y;
                f1 = sqrt(vec.x * vec.x + vec.y * vec.y);
                vec.x /= f1;
                vec.y /= f1;

                sp->hori_vec.x = vec.x - sp->horizon.x;
                sp->hori_vec.y = vec.y - sp->horizon.y;
                sp->hori_speed = sqrt(sp->hori_vec.x * sp->hori_vec.x + sp->hori_vec.y * sp->hori_vec.y);

                sp->horizon = vec;
            } else {
                sp->hori_vec = Vec2_0;
                sp->hori_speed = 0.0f;
            }
        }
    }

    dist = kp->dist_vv1 / kp->sec_length;

    if (sp->dpd_valid_fg == 0) {
        sp->dist = dist;
        sp->dist_vec = 0.0f;
        sp->dist_speed = 0.0f;
    } else {
        f2 = dist - sp->dist;
        if (f2 < 0.0f) {
            f1 = -f2;
        } else {
            f1 = f2;
        }

        if (kp->dist_play_mode == KPAD_PLAY_MODE_LOOSE) {
            if (f1 >= kp->dist_play_radius) {
                f1 = 1.0f;
            } else {
                f1 /= kp->dist_play_radius;
                f1 *= f1;
                f1 *= f1;
            }
            f1 *= kp->dist_sensitivity;

            sp->dist_vec = f1 * f2;
            if (sp->dist_vec < 0.0f) {
                sp->dist_speed = -sp->dist_vec;
            } else {
                sp->dist_speed = sp->dist_vec;
            }

            sp->dist += sp->dist_vec;
        } else {
            if (f1 > kp->dist_play_radius) {
                f1 = (f1 - kp->dist_play_radius) / f1 * kp->dist_sensitivity;
                sp->dist_vec = f1 * f2;
                if (sp->dist_vec < 0.0f) {
                    sp->dist_speed = -sp->dist_vec;
                } else {
                    sp->dist_speed = sp->dist_vec;
                }

                sp->dist += sp->dist_vec;
            } else {
                sp->dist_vec = 0.0f;
                sp->dist_speed = 0.0f;
            }
        }
    }

    pos.x = (kp->kobj_regular[0].center.x + kp->kobj_regular[1].center.x) * 0.5f;
    pos.y = (kp->kobj_regular[0].center.y + kp->kobj_regular[1].center.y) * 0.5f;

    f1 = kp->sec_nrm.x * kp->sec_nrm_hori.x + kp->sec_nrm.y * kp->sec_nrm_hori.y;
    f2 = -kp->sec_nrm.y * kp->sec_nrm_hori.x + kp->sec_nrm.x * kp->sec_nrm_hori.y;
    vec.x = f1 * pos.x - f2 * pos.y;
    vec.y = f2 * pos.x + f1 * pos.y;

    vec.x = (kp->center_org.x - vec.x) * kp->dpd2pos_scale;
    vec.y = (kp->center_org.y - vec.y) * kp->dpd2pos_scale;

    pos.x = -kp->accXY_nrm_hori.y * vec.x + kp->accXY_nrm_hori.x * vec.y;
    pos.y = -kp->accXY_nrm_hori.x * vec.x - kp->accXY_nrm_hori.y * vec.y;
    if (sp->dpd_valid_fg == 0) {
        sp->pos = pos;
        sp->vec = Vec2_0;
        sp->speed = 0.0f;
    } else {
        vec.x = pos.x - sp->pos.x;
        vec.y = pos.y - sp->pos.y;
        f1 = sqrt(vec.x * vec.x + vec.y * vec.y);

        if (kp->pos_play_mode == KPAD_PLAY_MODE_LOOSE) {
            // looks like a change was made here
            // todo -- find it
            if (f1 >= kp->pos_play_radius) {
                f1 = 1.0f;
            } else {
                f1 /= kp->pos_play_radius;
                f1 *= f1;
                f1 *= f1;
            }
            f1 *= kp->pos_sensitivity;

            sp->vec.x = f1 * vec.x;
            sp->vec.y = f1 * vec.y;
            sp->speed = sqrt(sp->vec.x * sp->vec.x + sp->vec.y * sp->vec.y);

            sp->pos.x += sp->vec.x;
            sp->pos.y += sp->vec.y;
        } else {
            if (f1 > kp->pos_play_radius) {
                f1 = (f1 - kp->pos_play_radius) / f1 * kp->pos_sensitivity;
                sp->vec.x = f1 * vec.x;
                sp->vec.y = f1 * vec.y;
                sp->speed = sqrt(sp->vec.x * sp->vec.x + sp->vec.y * sp->vec.y);

                sp->pos.x += sp->vec.x;
                sp->pos.y += sp->vec.y;
            } else {
                sp->vec = Vec2_0;
                sp->speed = 0.0f;
            }
        }
    }

    sp->dpd_valid_fg = valid_fg_next;
}

void calc_obj_horizon(KPADInsideStatus* kp) {
    f32 f1, vx, vy;

    vx = kp->kobj_regular[1].center.x - kp->kobj_regular[0].center.x;
    vy = kp->kobj_regular[1].center.y - kp->kobj_regular[0].center.y;
    kp->sec_length = sqrt(vx * vx + vy * vy);

    f1 = 1.0f / kp->sec_length;
    kp->sec_dist = kp->dist_vv1 * f1;

    kp->sec_nrm.x = (vx *= f1);
    kp->sec_nrm.y = (vy *= f1);

    kp->obj_horizon.x = kp->sec_nrm_hori.x * vx + kp->sec_nrm_hori.y * vy;
    kp->obj_horizon.y = kp->sec_nrm_hori.y * vx - kp->sec_nrm_hori.x * vy;
}

void get_kobj(KPADInsideStatus* kp, DPDObject* wobj_p) {
    const f32 dpd_scale = 2.0f / (f32)KPAD_DPD_RESO_WX;
    const f32 dpd_cx = (f32)(KPAD_DPD_RESO_WX - 1) / (f32)KPAD_DPD_RESO_WX;
    const f32 dpd_cy = (f32)(KPAD_DPD_RESO_WY - 1) / (f32)KPAD_DPD_RESO_WX;

    KPADObject* kobj_p;

    kobj_p = &kp->kobj_sample[WPAD_DPD_MAX_OBJECTS - 1];
    do {
        if (wobj_p->size) {
            kobj_p->center.x = (f32)(s32)wobj_p->x * dpd_scale - dpd_cx;
            kobj_p->center.y = (f32)(s32)wobj_p->y * dpd_scale - dpd_cy;

            kobj_p->error_fg = 0;
            kobj_p->state_fg = 0;
        } else {
            kobj_p->error_fg = -1;
        }

        --wobj_p;
    } while (--kobj_p >= kp->kobj_sample);
}

void check_kobj_outside_frame(KPADInsideStatus* kp, KPADObject* kobj_t) {
    KPADObject* kobj_p = &kobj_t[WPAD_DPD_MAX_OBJECTS - 1];

    do {
        if (kobj_p->error_fg < 0)
            continue;

        if (kobj_p->center.x <= kp->kobj_frame_min.x || kobj_p->center.x >= kp->kobj_frame_max.x || kobj_p->center.y <= kp->kobj_frame_min.y ||
            kobj_p->center.y >= kp->kobj_frame_max.y) {
            kobj_p->error_fg |= 1;
        }
    } while (--kobj_p >= kobj_t);
}

void check_kobj_same_position(KPADObject* kobj_t) {
    KPADObject *op1, *op2;

    op1 = kobj_t;
    do {
        if (op1->error_fg != 0)
            continue;

        op2 = op1 + 1;
        do {
            if (op2->error_fg != 0)
                continue;

            if (op1->center.x == op2->center.x && op1->center.y == op2->center.y) {
                op2->error_fg |= 2;
            }
        } while (++op2 <= &kobj_t[WPAD_DPD_MAX_OBJECTS - 1]);
    } while (++op1 < &kobj_t[WPAD_DPD_MAX_OBJECTS - 1]);
}

void read_kpad_dpd(KPADInsideStatus* kp, KPADUnifiedWpadStatus* uwp) {
    KPADStatus* sp = &kp->status;
    KPADObject* op1;
    s8 valid_fg_next;

    if (uwp->fmt == WPAD_FMT_CORE_ACC_DPD || uwp->fmt == WPAD_FMT_FREESTYLE_ACC_DPD || uwp->fmt == WPAD_FMT_CLASSIC_ACC_DPD) {
        get_kobj(kp, &uwp->u.core.obj[WPAD_DPD_MAX_OBJECTS - 1]);
    } else {
        op1 = &kp->kobj_sample[WPAD_DPD_MAX_OBJECTS - 1];
        do {
            op1->error_fg = -1;
        } while (--op1 >= kp->kobj_sample);
    }

    check_kobj_outside_frame(kp, kp->kobj_sample);
    check_kobj_same_position(kp->kobj_sample);
    kp->valid_objs = 0;
    op1 = &kp->kobj_sample[WPAD_DPD_MAX_OBJECTS - 1];
    do {
        if (op1->error_fg == 0)
            ++kp->valid_objs;
    } while (--op1 >= kp->kobj_sample);

    if (sp->acc_vertical.x <= kp_err_up_inpr)
        goto LABEL_select_NG;

    if (sp->dpd_valid_fg == 2 || sp->dpd_valid_fg == -2) {
        if (kp->valid_objs >= 2) {
            valid_fg_next = select_2obj_continue(kp);
            if (valid_fg_next)
                goto LABEL_select_OK;
        }
        if (kp->valid_objs >= 1) {
            valid_fg_next = select_1obj_continue(kp);
            if (valid_fg_next)
                goto LABEL_select_OK;
        }
    } else if (sp->dpd_valid_fg == 1 || sp->dpd_valid_fg == -1) {
        if (kp->valid_objs >= 2) {
            valid_fg_next = select_2obj_first(kp);
            if (valid_fg_next)
                goto LABEL_select_OK;
        }
        if (kp->valid_objs >= 1) {
            valid_fg_next = select_1obj_continue(kp);
            if (valid_fg_next)
                goto LABEL_select_OK;
        }
    } else {
        if (kp->valid_objs >= 2) {
            valid_fg_next = select_2obj_first(kp);

            if (valid_fg_next)
                goto LABEL_select_OK;
        }
        if (kp->valid_objs == 1) {
            valid_fg_next = select_1obj_first(kp);
            if (valid_fg_next)
                goto LABEL_select_OK;
        }
    }

LABEL_select_NG:
    valid_fg_next = 0;

LABEL_select_OK:
    if (valid_fg_next) {
        calc_obj_horizon(kp);
        if (kp->ah_circle_ct == 0) {
            if (kp->obj_horizon.x * kp->acc_horizon.x + kp->obj_horizon.y * kp->acc_horizon.y <= kp_err_acc_inpr) {
                valid_fg_next = 0;

                kp->kobj_regular[0].error_fg = kp->kobj_regular[1].error_fg = 1;
            }
        }

        if (sp->dpd_valid_fg == 2 && valid_fg_next == 2) {
            if (kp->dpd_valid2_ct == 200) {
                kp->trust_sec_length = kp->sec_length;
            } else {
                ++kp->dpd_valid2_ct;
            }
        } else {
            kp->dpd_valid2_ct = 0;
        }
    } else {
        kp->dpd_valid2_ct = 0;
    }

    calc_dpd_variable(kp, valid_fg_next);
}

void clamp_stick_circle(Vec2* stick, s32 sx, s32 sy, s32 min, s32 max) {
    f32 length;
    f32 fx = (f32)sx;
    f32 fy = (f32)sy;
    f32 fmin = (f32)min;
    f32 fmax = (f32)max;

    length = sqrt(fx * fx + fy * fy);

    if (length <= fmin) {
        stick->x = stick->y = 0.0f;

    } else if (length >= fmax) {
        stick->x = fx / length;
        stick->y = fy / length;

    } else {
        length = (length - fmin) / (fmax - fmin) / length;
        stick->x = fx * length;
        stick->y = fy * length;
    }
}

void clamp_stick_cross(Vec2* stick, s32 sx, s32 sy, s32 min, s32 max) {
    f32 length;

    if (sx < 0) {
        clamp_trigger(&stick->x, -sx, min, max);
        stick->x = -stick->x;
    } else {
        clamp_trigger(&stick->x, sx, min, max);
    }
    if (sy < 0) {
        clamp_trigger(&stick->y, -sy, min, max);
        stick->y = -stick->y;
    } else {
        clamp_trigger(&stick->y, sy, min, max);
    }

    length = stick->x * stick->x + stick->y * stick->y;
    if (length > 1.0f) {
        length = sqrt(length);
        stick->x /= length;
        stick->y /= length;
    }
}

void read_kpad_stick(KPADInsideStatus* kp, KPADUnifiedWpadStatus* uwp) {
    KPADEXStatus* ep = &kp->status.ex_status;
    void (*clampStickFuncp)(Vec2* stick, s32 sx, s32 sy, s32 min, s32 max);

    clampStickFuncp = kp_stick_clamp_cross ? clamp_stick_circle : clamp_stick_cross;

    if (uwp->u.fs.dev == WPAD_DEV_FREESTYLE &&
        (uwp->fmt == WPAD_FMT_FREESTYLE || uwp->fmt == WPAD_FMT_FREESTYLE_ACC || uwp->fmt == WPAD_FMT_FREESTYLE_ACC_DPD)) {
        if (kp->exResetReq) {
            kp->exResetReq = FALSE;

            ep->fs.stick = Vec2_0;
            ep->fs.acc.x = ep->fs.acc.z = 0.0f;
            ep->fs.acc.y = -1.0f;
            ep->fs.acc_value = 1.0f;
            ep->fs.acc_speed = 0.0f;
        }

        clampStickFuncp(&ep->fs.stick, uwp->u.fs.fsStickX, uwp->u.fs.fsStickY, kp_fs_fstick_min, kp_fs_fstick_max);
    } else if (uwp->u.cl.dev == WPAD_DEV_CLASSIC &&
               (uwp->fmt == WPAD_FMT_CLASSIC || uwp->fmt == WPAD_FMT_CLASSIC_ACC || uwp->fmt == WPAD_FMT_CLASSIC_ACC_DPD)) {
        if (kp->exResetReq) {
            kp->exResetReq = FALSE;

            ep->cl.lstick = Vec2_0;
            ep->cl.rstick = Vec2_0;
            ep->cl.ltrigger = ep->cl.rtrigger = 0.0f;
            ep->cl.hold = ep->cl.trig = ep->cl.release = 0x00000000;
            kp->btn_cl_repeat_time = 0;
            kp->btn_cl_repeat_next = kp->btn_repeat_delay;
        }

        clampStickFuncp(&ep->cl.lstick, uwp->u.cl.clLStickX, uwp->u.cl.clLStickY, kp_cl_stick_min, kp_cl_stick_max);
        clampStickFuncp(&ep->cl.rstick, uwp->u.cl.clRStickX, uwp->u.cl.clRStickY, kp_cl_stick_min, kp_cl_stick_max);
        clamp_trigger(&ep->cl.ltrigger, uwp->u.cl.clTriggerL, kp_cl_trigger_min, kp_cl_trigger_max);
        clamp_trigger(&ep->cl.rtrigger, uwp->u.cl.clTriggerR, kp_cl_trigger_min, kp_cl_trigger_max);
    }
}

void read_kpad_button(KPADInsideStatus* kp, u32 dev_type, u32 count, u32 core, u32 fs, u32 cl) {
    KPADStatus* sp = &kp->status;
    KPADEXStatus* ep = &kp->status.ex_status;
    u32 old_fg, change_fg;
    u32 cl_old_fg, cl_change_fg;
    old_fg = sp->hold & KPAD_BUTTON_MASK;
    sp->hold = (core & (KPAD_BUTTON_MASK & ~(WPAD_BUTTON_Z | WPAD_BUTTON_C))) | (fs & (WPAD_BUTTON_Z | WPAD_BUTTON_C));
    change_fg = sp->hold ^ old_fg;
    sp->trig = change_fg & sp->hold;
    sp->release = change_fg & old_fg;

    if (dev_type == WPAD_DEV_CLASSIC) {
        cl_old_fg = ep->cl.hold & KPAD_BUTTON_MASK;
        ep->cl.hold = cl & KPAD_BUTTON_MASK;
        cl_change_fg = ep->cl.hold ^ cl_old_fg;
        ep->cl.trig = cl_change_fg & ep->cl.hold;
        ep->cl.release = cl_change_fg & cl_old_fg;
    }

    calc_button_repeat(kp, dev_type, count);
}

static void KPADiSamplingCallback(s32 chan);
static void KPADiControlDpdCallback(s32 chan, s32 result);

// https://decomp.me/scratch/UtYxQ
s32 KPADRead(s32 chan, KPADStatus samplingBufs[], u32 length) {
    KPADTmpStatus* tp = (KPADTmpStatus*)samplingBufs;
    KPADUnifiedWpadStatus uwStatus;
    KPADInsideStatus* kp = &inside_kpads[chan];
    KPADUnifiedWpadStatus* uwp;
    s32 wpad_err;
    s32 idx;
    u32 copy_ct;
    u32 return_ct = 0;
    u32 bufCount;
    BOOL enabled;
    u32 lastCoreButton;
    u32 lastFsButton;
    u32 lastClButton;
    u32 lastDev;

    if (WPADGetStatus() != WPAD_STATE_SETUP) {
        return 0;
    }

    enabled = OSDisableInterrupts();
    if (kp->readLocked) {
        OSRestoreInterrupts(enabled);
        return 0;
    }
    kp->readLocked = TRUE;

    wpad_err = WPADProbe(chan, NULL);
    if (wpad_err == WPAD_ERR_NO_CONTROLLER && kp->dpd_ctrl_callback && kp->dpdPreCallbackDone && !kp->dpdPostCallbackDone) {
        KPADiControlDpdCallback(chan, WPAD_ERR_NONE);
    }

    OSRestoreInterrupts(enabled);

    if (kp->resetReq) {
        kp->status.wpad_err = (s8)wpad_err;
        reset_kpad(kp);
    }

    WPADSetSamplingCallback(chan, KPADiSamplingCallback);

    if (kp->bufCount && samplingBufs && length) {
        enabled = OSDisableInterrupts();

        bufCount = kp->bufCount;
        if (bufCount > length) {
            copy_ct = length;
        } else {
            copy_ct = bufCount;
        }
        kp->bufCount = 0;
        return_ct = copy_ct;

        tp += copy_ct;

        idx = (s32)(kp->bufIdx - copy_ct);
        if (idx < 0) {
            idx += 120;
        }

        --tp;
        while (--copy_ct) {
            --tp;
            tp->w = kp->uniRingBuf[idx];
            idx++;
            if (idx >= 120) {
                idx = 0;
            }
        }
        uwStatus = kp->uniRingBuf[idx];

        OSRestoreInterrupts(enabled);
        {
            WPADAcc core1G = {1, 1, 1};
            WPADAcc fs1G = {1, 1, 1};

            WPADGetAccGravityUnit(chan, WPAD_DEV_CORE, &core1G);
            if (core1G.x * core1G.y * core1G.z != 0) {
                kp->acc_scale_x = 1.0f / core1G.x;
                kp->acc_scale_y = 1.0f / core1G.y;
                kp->acc_scale_z = 1.0f / core1G.z;
            } else {
                kp->acc_scale_x = 1.0f / 100;
                kp->acc_scale_y = 1.0f / 100;
                kp->acc_scale_z = 1.0f / 100;
            }

            WPADGetAccGravityUnit(chan, WPAD_DEV_FREESTYLE, &fs1G);
            if (fs1G.x * fs1G.y * fs1G.z != 0) {
                kp->fs_acc_scale_x = 1.0f / fs1G.x;
                kp->fs_acc_scale_y = 1.0f / fs1G.y;
                kp->fs_acc_scale_z = 1.0f / fs1G.z;
            } else {
                kp->fs_acc_scale_x = 1.0f / 200;
                kp->fs_acc_scale_y = 1.0f / 200;
                kp->fs_acc_scale_z = 1.0f / 200;
            }
        }

        copy_ct = return_ct;
        lastCoreButton = lastFsButton = lastClButton = KPAD_BUTTON_MASK;
        lastDev = WPAD_DEV_NOT_FOUND;
        tp = (KPADTmpStatus*)samplingBufs + copy_ct;
        --tp;
        do {
            --tp;
            uwp = (copy_ct > 1) ? &tp->w : &uwStatus;
            switch (uwp->u.core.err) {
            case WPAD_ERR_NONE:
                lastDev = uwp->u.core.dev;
                if (lastDev == WPAD_DEV_FREESTYLE) {
                    lastFsButton = uwp->u.fs.button;
                    lastClButton = 0;
                } else if (lastDev == WPAD_DEV_CLASSIC) {
                    lastFsButton = 0;
                    lastClButton = uwp->u.cl.clButton;
                } else {
                    lastFsButton = lastClButton = 0;
                }
            case WPAD_ERR_CORRUPTED:
            case WPAD_ERR_BUSY:
                lastCoreButton = uwp->u.core.button;
                break;

            default:
                break;
            }
        } while (--copy_ct);

        if (lastCoreButton == KPAD_BUTTON_MASK) {
            do {
                memcpy(samplingBufs, &kp->status, sizeof(KPADStatus));
                samplingBufs++;
            } while (--return_ct);
            goto finish;
        }
        if (lastFsButton == KPAD_BUTTON_MASK) {
            lastFsButton = kp->status.hold;
        }
        if (lastClButton == KPAD_BUTTON_MASK) {
            lastClButton = kp->status.ex_status.cl.hold;
        }
        read_kpad_button(kp, lastDev, bufCount, lastCoreButton, lastFsButton, lastClButton);

        copy_ct = return_ct;
        tp = (KPADTmpStatus*)samplingBufs + copy_ct;
        --tp;

        do {
            --tp;
            uwp = (copy_ct > 1) ? &tp->w : &uwStatus;

            kp->status.wpad_err = uwp->u.core.err;
            if (kp->status.dev_type != uwp->u.core.dev) {
                if (uwp->u.core.err == WPAD_ERR_NONE || uwp->u.core.err == WPAD_ERR_BUSY || uwp->u.core.err == WPAD_ERR_NO_CONTROLLER) {
                    kp->status.dev_type = uwp->u.core.dev;
                    kp->exResetReq = TRUE;
                }
            }
            kp->status.data_format = uwp->fmt;

            switch (uwp->u.core.err) {
            case WPAD_ERR_NONE:
                read_kpad_stick(kp, uwp);
            case WPAD_ERR_CORRUPTED:
                read_kpad_acc(kp, uwp);
                read_kpad_dpd(kp, uwp);
                break;

            default:
                kp->status.dpd_valid_fg = FALSE;
                break;
            }
            tp[1].k = kp->status;

        } while (--copy_ct);
    }

finish:
    kp->readLocked = FALSE;
    return (s32)return_ct;
}

void KPADInit(void) {
    s32 i;
    KPADInsideStatus* kp;
    GXColor black = {0, 0, 0, 0};
    GXColor white = {255, 255, 255, 255};
    u32 idx;

    WPADInit();
    memset(&inside_kpads, 0, sizeof(inside_kpads));
    kp_err_dist_max = (f32)(1.0f + (f32)WPADGetDpdSensitivity());

    i = 0;
    do {
        kp = &inside_kpads[i];
        kp->dpdEnabled = TRUE;
        kp->dpdCmd = 0;
        kp->status.dev_type = WPAD_DEV_NOT_FOUND;
        kp->status.data_format = WPAD_FMT_CORE;
        kp->dist_org = idist_org;
        kp->accXY_nrm_hori = iaccXY_nrm_hori;
        kp->sec_nrm_hori = isec_nrm_hori;
        kp->center_org = icenter_org;
        calc_dpd2pos_scale(kp);
        kp->pos_play_radius = kp->hori_play_radius = kp->dist_play_radius = kp->acc_play_radius = 0.0f;
        kp->pos_sensitivity = kp->hori_sensitivity = kp->dist_sensitivity = kp->acc_sensitivity = 1.0f;
        kp->pos_play_mode = kp->hori_play_mode = kp->dist_play_mode = kp->acc_play_mode = KPAD_PLAY_MODE_LOOSE;

        KPADSetBtnRepeat(i, 0.0f, 0.0f);
        KPADEnableAimingMode(i);
        kp->fsAccRevise = 0;

        //----- ��]�s��̏�����
        MTXRowCol(kp_fs_rot, 0, 0) = 1;
        MTXRowCol(kp_fs_rot, 0, 1) = 0;
        MTXRowCol(kp_fs_rot, 0, 2) = 0;
        MTXRowCol(kp_fs_rot, 0, 3) = 0;
        MTXRowCol(kp_fs_rot, 1, 0) = 0;
        MTXRowCol(kp_fs_rot, 1, 1) = (f32)cos(MTXDegToRad(kp_fs_revise_deg));
        MTXRowCol(kp_fs_rot, 1, 2) = (f32)-sin(MTXDegToRad(kp_fs_revise_deg));
        MTXRowCol(kp_fs_rot, 1, 3) = 0;
        MTXRowCol(kp_fs_rot, 2, 0) = 0;
        MTXRowCol(kp_fs_rot, 2, 1) = (f32)sin(MTXDegToRad(kp_fs_revise_deg));
        MTXRowCol(kp_fs_rot, 2, 2) = (f32)cos(MTXDegToRad(kp_fs_revise_deg));
        MTXRowCol(kp_fs_rot, 2, 3) = 0;

        idx = 0;
        do {
            kp->uniRingBuf[idx].u.core.err = WPAD_ERR_NO_CONTROLLER;
        } while (++idx < 120);

    } while (++i < WPAD_MAX_CONTROLLERS);

    KPADReset();
    OSRegisterVersion(__KPADVersion);
}

void KPADReset(void) {
    s32 chan;
    KPADSetObjInterval(kp_obj_interval);

    chan = WPAD_MAX_CONTROLLERS - 1;
    do {
        if (WPADGetStatus() == WPAD_STATE_SETUP) {
            WPADStopMotor(chan);
        }
        inside_kpads[chan].resetReq = TRUE;
    } while (--chan >= 0);
}

static void KPADiSamplingCallback(s32 chan) {
    KPADInsideStatus* kp = &inside_kpads[chan];
    KPADUnifiedWpadStatus* uwp;
    u32 idx;
    u32 type;
    u32 curDpd;
    f32 aimClbr;
    static struct {
        u8 dpd;
        u8 fmt;
    } table[] = {{0, WPAD_FMT_CORE_ACC},          {3, WPAD_FMT_CORE_ACC_DPD}, {0, WPAD_FMT_FREESTYLE_ACC},
                 {0, WPAD_FMT_FREESTYLE_ACC_DPD}, {0, WPAD_FMT_CLASSIC_ACC},  {1, WPAD_FMT_CLASSIC_ACC_DPD}};

    ASSERT((0 <= chan) && (chan < WPAD_MAX_CONTROLLERS));

    if (WPADProbe(chan, &type) == WPAD_ERR_NO_CONTROLLER) {
        goto finish;
    }

    idx = kp->bufIdx;
    if (idx >= 120) {
        idx = 0;
    }

    uwp = &kp->uniRingBuf[idx];
    WPADRead(chan, &uwp->u);
    uwp->fmt = (u8)WPADGetDataFormat(chan);

    kp->bufIdx = (u8)(idx + 1);
    if (kp->bufCount < 120) {
        kp->bufCount++;
    }

    if (kp->aimReq) {
        if (kp->aimEnabled) {
            if (WPAD_SENSOR_BAR_POS_TOP == WPADGetSensorBarPosition()) {
                aimClbr = 0.2f;
            } else {
                aimClbr = -0.2f;
            }
        } else {
            aimClbr = 0.0f;
        }
        KPADSetSensorHeight(chan, aimClbr);

        kp->aimReq = FALSE;
    }

    switch (type) {
    case WPAD_DEV_CORE:
    case WPAD_DEV_FUTURE:
    case WPAD_DEV_NOT_SUPPORTED:
    case WPAD_DEV_UNKNOWN:
        idx = 0;
        break;

    case WPAD_DEV_FREESTYLE:
        idx = 2;
        break;

    case WPAD_DEV_CLASSIC:
        idx = 4;
        break;

    default:
        goto finish;
    }

    if (kp->dpdEnabled) {
        idx += 1;
    }

    curDpd = (u32)(WPADIsDpdEnabled(chan) ? kp->dpdCmd : 0);

    if (curDpd != table[idx].dpd) {
        if (kp->dpd_ctrl_callback && !kp->dpdPreCallbackDone) {
            kp->dpdPreCallbackDone = TRUE;
            kp->dpd_ctrl_callback(chan, 0);
            kp->dpdPostCallbackDone = FALSE;
        }
        if (!kp->dpdIssued) {
            kp->dpdIssued = TRUE;
            if (WPADControlDpd(chan, table[idx].dpd, KPADiControlDpdCallback) == WPAD_ERR_NONE) {
                kp->dpdCmd = table[idx].dpd;
            }
        }
    } else {
        if (uwp->fmt != table[idx].fmt) {
            WPADSetDataFormat(chan, table[idx].fmt);
        }
    }

finish:
    if (kp->appSamplingCallback) {
        kp->appSamplingCallback(chan);
    }
}

void KPADiControlDpdCallback(s32 chan, s32 result) {
    KPADInsideStatus* kp = &inside_kpads[chan];

    if (result == WPAD_ERR_NONE) {
        if (kp->dpd_ctrl_callback && !kp->dpdPostCallbackDone) {
            kp->dpdPostCallbackDone = TRUE;
            kp->dpd_ctrl_callback(chan, 1);
            kp->dpdPreCallbackDone = FALSE;
        }
    }
    kp->dpdIssued = FALSE;
}
