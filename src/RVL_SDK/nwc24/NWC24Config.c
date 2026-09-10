#include <revolution/DVD.h>
#include <revolution/nwc24.h>
#include <revolution/nwc24/NWC24Internal.h>
#include <revolution/os.h>

#define CONFIG_MAGIC FOURCC('W', 'c', 'C', 'f')
#define CONFIG_VERSION 8
#define CONFIG_MAX_ID_COUNT 32

#define COMPANY_CODE_NINTENDO 01

static NWC24iConfig* config = NULL;
static BOOL ConfigModified = FALSE;

static const char* MBoxDir = "/shared2/wc24/mbox";
static const char* ConfigFile = "/shared2/wc24/nwc24msg.cfg";
static const char* CfgBakFile = "/shared2/wc24/nwc24msg.cbk";

#define UserId (*(u64*)0x800031C0)
static u16 VirtualGroupId = 1;

static u32 GetConfigCheckSum(void);
static NWC24Err CheckConfig(void) NO_INLINE;
static NWC24Err GenerateUserId(NWC24UserId* pUserId);

NWC24Err NWC24GetMyUserId(NWC24UserId* pUserId) {
    NWC24Err scdErr;
    NWC24Err result;

    result = NWC24_OK;

    if (NWC24IsMsgLibOpened() || NWC24IsMsgLibOpenedByTool()) {
        *pUserId = config->userId;
    } else {
        *pUserId = UserId;
        scdErr = *pUserId == 0 ? NWC24_ERR_NULL : NWC24iCheckUserIdCRC(*pUserId);
        if (scdErr == NWC24_OK) {
            return scdErr;
        }
        scdErr = NWC24SuspendScheduler();
        if (scdErr < 0) {
            return scdErr;
        }

        result = NWC24GenerateNewUserId(pUserId);
        if (result == NWC24_ERR_ID_GENERATED || result == NWC24_ERR_ID_REGISTERED) {
            result = NWC24_OK;
        }

        scdErr = NWC24ResumeScheduler();
        if (scdErr < 0) {
            result = result != NWC24_OK ? result : scdErr;
        }
    }

    return result;
}

NWC24Err NWC24GenerateNewUserId(NWC24UserId* pUserId) {
    NWC24Err open;
    NWC24Err close;

    open = NWC24BlockOpenMsgLib(TRUE);
    if (open < 0) {
        return open;
    }

    open = GenerateUserId(pUserId);
    close = NWC24BlockOpenMsgLib(FALSE);

    return open >= 0 ? close : open;
}

NWC24Err NWC24iConfigOpen(void) {
    NWC24Err err;
    ConfigModified = 0;
    config = (NWC24iConfig*)NWC24WorkP->config;

    err = NWC24iConfigReload();

    if (err != NWC24_ERR_FILE_NOEXISTS && err == NWC24_OK) {
        UserId = config->userId;
        DCStoreRange((void*)0x800031C0, 0x20);
    }

    return err;
}

NWC24Err NWC24iConfigReload(void) {
    NWC24Err result;
    NWC24Err close;
    NWC24File file;

    if (NWC24WorkP == NULL) {
        return NWC24_ERR_LIB_NOT_OPENED;
    }

    result = NWC24FOpen(&file, ConfigFile, NWC24_OPEN_NAND_R);

    if (result == NWC24_OK) {
        result = NWC24FRead(config, sizeof(NWC24iConfig), &file);
        close = NWC24FClose(&file);
        result = result != NWC24_OK ? result : close;
    }

    if (result == NWC24_OK) {
        result = CheckConfig();

        if (result == NWC24_OK) {
            ConfigModified = FALSE;
            return result;
        }
    }

    result = NWC24FOpen(&file, CfgBakFile, NWC24_OPEN_NAND_R);

    if (result == NWC24_OK) {
        result = NWC24FRead(config, sizeof(NWC24iConfig), &file);
        close = NWC24FClose(&file);
        result = result != NWC24_OK ? result : close;
    }

    if (result == NWC24_OK) {
        result = CheckConfig();

        if (result == NWC24_OK) {
            ConfigModified = TRUE;
        }
    }

    return result;
}

NWC24Err NWC24iConfigFlush(void) {
    NWC24Err result;
    NWC24Err close;
    NWC24File file;

    if (NWC24WorkP == NULL) {
        return NWC24_ERR_LIB_NOT_OPENED;
    }

    if (!ConfigModified) {
        return NWC24_OK;
    }

    config->checksum = GetConfigCheckSum();

    result = NWC24FOpen(&file, ConfigFile, NWC24_OPEN_NAND_W);

    if (result == NWC24_OK) {
        result = NWC24FWrite(config, sizeof(NWC24iConfig), &file);
        close = NWC24FClose(&file);
        result = result != NWC24_OK ? result : close;

        ConfigModified = FALSE;
    }

    if (result != NWC24_OK) {
        return result;
    }

    result = NWC24FOpen(&file, CfgBakFile, NWC24_OPEN_NAND_W);

    if (result == NWC24_OK) {
        result = NWC24FWrite(config, sizeof(NWC24iConfig), &file);
        close = NWC24FClose(&file);
        result = result != NWC24_OK ? result : close;

        ConfigModified = FALSE;
    }

    return result;
}

const char* NWC24GetAccountDomain(void) {
    return config->acctDomain;
}

const char* NWC24GetMBoxDir(void) {
    OSIOSRev rev;
    __OSGetIOSRev(&rev);

    if (rev.major == 0 && (rev.minor == 7 && rev.micro == 0 || rev.minor < 7)) {
        OSPanic(__FILE__, 695, "stopped.");
    }

    return MBoxDir;
}

u32 NWC24GetAppId(void) {
    u32 appId = *(u32*)OSGetAppGamename();

    if (appId == 0 && OSGetAppType() == 0) {
        appId = *(u32*)DVDGetCurrentDiskID()->gameName;
    }

    return appId;
}

u16 NWC24GetGroupId(void) {
    NANDStatus stat;
    u16 groupId;
    char* pHomeDir;

    groupId = VirtualGroupId;

    switch (OSGetAppType()) {
    case 0x40: {
        groupId = COMPANY_CODE_NINTENDO;
        break;
    }

    case 0x80: {
        groupId = *(u16*)DVDGetCurrentDiskID()->company;
        break;
    }

    case 0x81: {
        pHomeDir = NWC24WorkP->pathWork;

        if (NANDGetHomeDir(pHomeDir) == NAND_RESULT_OK) {
            if (NANDGetStatus(pHomeDir, &stat) == NAND_RESULT_OK) {
                groupId = stat.groupId;
            }
        }
        break;
    }
    }

    return groupId;
}

static u32 GetConfigCheckSum(void) {
    u32 i;
    u32 csum;
    u32* pData;
    u32 dataWords;

    csum = 0;
    pData = (u32*)config;
    dataWords = (sizeof(NWC24iConfig) - sizeof(config->checksum)) / sizeof(u32);

    for (i = 0; i < dataWords; i++) {
        csum += *pData++;
    }

    return csum;
}

static NWC24Err CheckConfig(void) {
    if (config->magic != CONFIG_MAGIC) {
        return NWC24_ERR_BROKEN;
    }

    if (config->checksum != GetConfigCheckSum()) {
        return NWC24_ERR_BROKEN;
    }

    if (config->createCount >= CONFIG_MAX_ID_COUNT) {
        return NWC24_ERR_BROKEN;
    }

    if (config->version != CONFIG_VERSION) {
        return NWC24_ERR_VER_MISMATCH;
    }

    return NWC24_OK;
}

static NWC24Err GenerateUserId(NWC24UserId* pUserId) {
    NWC24Err result;
    u32 dummy = 0;

    if (pUserId == NULL) {
        return NWC24_ERR_INVALID_VALUE;
    }

    *pUserId = 9999999999999999;
    result = NWC24iRequestGenerateUserId(pUserId, &dummy);
    UserId = *pUserId;
    DCStoreRange((void*)0x800031C0, 0x20);
    return result;
}
