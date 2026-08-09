#include "revolution/nwc24/NWC24Config.h"
#include "revolution/nwc24/NWC24FileApi.h"
#include "revolution/nwc24/NWC24Structs.h"

static NWC24MsgConfig* config;
static int ConfigModified;
static NWC24WorkMem* NWC24WorkP;

static char* MBoxDir = "/shared2/wc24/mbox";
static const char* ConfigFile = "/shared2/wc24/nwc24msg.cfg";
static const char* CfgBakFile = "/shared2/wc24/nwc24msg.cbk";

static u64 NWC24UserId : 0x800031C0;

static NWC24Err CheckConfig(void);

NWC24Err NWC24iConfigOpen(void) {
    NWC24Err err;
    ConfigModified = 0;
    config = (NWC24MsgConfig*)NWC24WorkP->ConfigCache;

    err = NWC24iConfigReload();

    if (err != NWC24_ERR_FILE_NOEXISTS && err == NWC24_OK) {
        NWC24UserId = config->myUserId;
        DCStoreRange((void*)0x800031C0, 0x20);
    }

    return err;
}

NWC24Err NWC24iConfigReload(void) {
    NWC24Err err, err2;
    NWC24File cfgFile;

    if (NWC24WorkP == NULL) {
        return NWC24_ERR_LIB_NOT_OPENED;
    }

    err = NWC24FOpen(&cfgFile, ConfigFile, 2);

    if (err == NWC24_OK) {
        err2 = NWC24FRead(config, 1024, &cfgFile);
        err = NWC24FClose(&cfgFile);

        if (err2 != NWC24_OK) {
            err = err2;
        }
    }

    if (!err && (err = CheckConfig()) == NWC24_OK) {
        ConfigModified = 0;
    } else {
        err = NWC24FOpen(&cfgFile, CfgBakFile, 2);

        if (err == NWC24_OK) {
            err2 = NWC24FRead(config, 1024, &cfgFile);
            err = NWC24FClose(&cfgFile);

            if (err2 != NWC24_OK) {
                err = err2;
            }
        }

        if (err == NWC24_OK) {
            err = CheckConfig();

            if (err == NWC24_OK) {
                ConfigModified = 1;
            }
        }
    }

    return err;
}

char* NWC24GetAccountDomain(void) {
    return config->domain;
}

char* NWC24GetMBoxDir(void) {
    const char* retval;
    OSIOSRev rev;
    __OSGetIOSRev(&rev);

    if (!rev.major && (rev.minor == 7 && !rev.micro || rev.minor < 7)) {
        OSPanic(__FILE__, 695, "stopped.");
    }

    return MBoxDir;
}

u32 NWC24GetAppId(void) {
    u32 gameName = *(u32*)OSGetAppGamename();

    if (gameName == 0 && OSGetAppType() == 0) {
        gameName = *(u32*)DVDGetCurrentDiskID()->gameName;
    }

    return gameName;
}

static u16 VirtualGroupId = 1;

// https://decomp.me/scratch/VyP5V
u16 NWC24GetGroupId(void) {
    u16 groupId = VirtualGroupId;
    u8 appType;
    char* path;

    appType = OSGetAppType();

    switch (appType) {
    case 0x40:
        return 1;
    case 0x80:
        return *(u16*)DVDGetCurrentDiskID()->company;
    case 0x82: {
        NANDStatus status;
        path = NWC24WorkP->StrBuffer_FilePath;
        if (!NANDGetHomeDir(NWC24WorkP->StrBuffer_FilePath) && !NANDGetStatus(path, &status)) {
            return *(u16*)&path[4];
        }
    }
    }

    return groupId;
}
