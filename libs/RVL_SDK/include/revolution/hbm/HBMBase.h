#ifndef HBM_BASE_H
#define HBM_BASE_H

#include <revolution/kpad.h>
#include <revolution/mem.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    HBM_SELECT_NULL = -1,
    HBM_SELECT_HOMEBTN = 0,
    HBM_SELECT_BTN1,
    HBM_SELECT_BTN2,
    HBM_SELECT_BTN3,
    HBM_SELECT_BTN4,
    HBM_SELECT_MAX
} HBMSelectBtnNum;

enum {
    HBMSE_HOME_BUTTON = 0,
    HBMSE_RETURN_APP,
    HBMSE_GOTO_MENU,
    HBMSE_RESET_APP,
    HBMSE_FOCUS,
    HBMSE_SELECT,
    HBMSE_CANCEL,
    HBMSE_OPEN_CONTROLLER,
    HBMSE_CLOSE_CONTROLLER,
    HBMSE_VOLUME_PLUS,
    HBMSE_VOLUME_MINUS,
    HBMSE_VOLUME_PLUS_LIMIT,
    HBMSE_VOLUME_MINUS_LIMIT,
    HBMSE_NOTHING_DONE,
    HBMSE_VIBE_ON,
    HBMSE_VIBE_OFF,
    HBMSE_START_CONNECT_WINDOW,
    HBMSE_CONNECTED,
    HBMSE_CONNECTED2,
    HBMSE_CONNECTED3,
    HBMSE_CONNECTED4,
    HBMSE_END_CONNECT_WINDOW
};

enum {
    HBMSEV_BEFORE_INIT_SOUND,
    HBMSEV_INIT_SOUND,
    HBMSEV_BEGIN_EXIT_ANIM,
    HBMSEV_BEGIN_BLACKOUT,
    HBMSEV_END_MENU,
    HBMSEV_PLAY_SOUND
};

enum {
    HBMSEV_RET_NONE = 0,
    HBMSEV_RET_PLAY_SOUND
};

enum {
    HBMMSG_NOSAVE_DEFAULT = 0,
    HBMMSG_NOSAVE_WIIMENU = 1,
    HBMMSG_NOSAVE_RESET = 2,
    HBMMSG_NOSAVE_ALL = -1
};

typedef struct {
    /* 0x00 */ KPADStatus* kpad;
    /* 0x04 */ Vec2 pos;
    /* 0x0C */ u32 use_devtype;
} HBMKPadData;

typedef struct {
    /* 0x00 */ HBMKPadData wiiCon[WPAD_MAX_CONTROLLERS];
} HBMControllerData;

typedef int (*HBMSoundCallback)(int evt, int num);

typedef struct {
    /* 0x00 */ void* layoutBuf;
    /* 0x04 */ void* spkSeBuf;
    /* 0x08 */ void* msgBuf;
    /* 0x0C */ void* configBuf;
    /* 0x10 */ void* mem;
    /* 0x14 */ HBMSoundCallback sound_callback;
    /* 0x18 */ int backFlag;
    /* 0x1C */ int region;
    /* 0x20 */ int cursor;
    /* 0x24 */ int messageFlag;
    /* 0x28 */ u32 configBufSize;
    /* 0x2C */ u32 memSize;
    /* 0x30 */ f32 frameDelta;
    /* 0x34 */ Vec2 adjust;
    /* 0x3C */ MEMAllocator* pAllocator;
} HBMDataInfo;

#ifdef __cplusplus
}
#endif

#endif  // HBM_BASE_H
