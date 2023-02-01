#ifndef ESP_H
#define ESP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>

typedef struct {
    u32 code;
    u32 playLimit;
} ESLpEntry;

typedef struct {
    u8 version;
    u64 ticketID;
    u32 devID;
    u64 titleID;
    u8 mask[2];
    u16 ticketVer;
    u32 accTitleID;
    u32 accTitleMask;
    u8 license;
    u8 res[48];
    u8 what;
    u8 another_mask[64];
    ESLpEntry limits[8];
} ESTicketView;

s32 ESP_InitLib(void);
s32 ESP_CloseLib(void);
s32 ESP_LaunchTitle(u64, ESTicketView *);

#ifdef __cplusplus
}
#endif

#endif // ESP_H