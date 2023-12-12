#include <revolution/nand.h>
#include <revolution/esp.h>
#include <revolution/fs.h>
#include <revolution/os.h>
#include <cstdio>
#include <cstring>
#include <wstring.h>

const char* __NANDVersion = "<< RVL_SDK - NAND \trelease build: Dec 11 2007 01:35:48 (0x4199_60831) >>";
enum LibState {
    STATE_NOT_INITIALIZED, 
    STATE_WORKING, 
    STATE_INITIALIZED
};

static enum LibState s_libState = STATE_NOT_INITIALIZED;
static char s_currentDir[64] = "/" __attribute__ ((aligned(32)));
static char s_homeDir[64] = "" __attribute__ ((aligned(32)));

static BOOL nandOnShutdown(BOOL final, u32 event);
void nandConvertPath(char *, const char *, const char *);
static void nandShutdownCallback(ISFSError, void *);

static OSShutdownFunctionInfo s_shutdownFuncInfo =
{
    nandOnShutdown,
    255
};

void nandRemoveTailToken(char* newpath, const char* oldpath) {
    if (oldpath[0] == '/' && oldpath[1] == '\0') {
        newpath[0] = '/';
        newpath[1] = '\0';
    }
    else {
        int i = 0;

        for (i = (int)(strlen(oldpath)) - 1; i >= 0; --i) {
            if (oldpath[i] == '/') {
                if (i != 0) {
                    strncpy(newpath, oldpath, (u32)i);
                    newpath[i] = '\0';
                    break;
                }
                else {
                    newpath[0] = '/';
                    newpath[1] = '\0';
                    break;
                }
            }
        }
    }
}

void nandGetHeadToken(char* token, char* newpath, const char* oldpath) {
    unsigned int i = 0;

    for (i = 0; i <= strlen(oldpath); ++i) {
        if (oldpath[i] == '/') {
            strncpy(token, oldpath, i);
            token[i] = '\0';

            if (oldpath[i + 1] == '\0') {
                newpath[0] = '\0';
            }
            else {
                strcpy(newpath, oldpath + i + 1);
            }

            break;
        }
        else if (oldpath[i] == '\0') {
            strncpy(token, oldpath, i);
            token[i] = '\0';
            newpath[0] = '\0';
            break;
        }
    }
}

void nandGetRelativeName(char* name, const char* path) {
    if (strcmp("/", path) == 0) {
        strcpy(name, "");
    }
    else {
        int i = 0;

        for (i = (int)(strlen(path) - 1); i >= 0; --i) {
            if (path[i] == '/') {
                break;
            }
        }

        strcpy(name, path + i + 1);
    }
}

void nandConvertPath(char* abspath, const char* wd, const char* relpath) {
    char token[128];
    char new_relpath[128];

    if (strlen(relpath) == 0) {
        strcpy(abspath, wd);
        return;
    }

    nandGetHeadToken(token, new_relpath, relpath);

    if (strcmp(token, ".") == 0) {
        nandConvertPath(abspath, wd, new_relpath);
    }
    else if (strcmp(token, "..") == 0) {
        char new_wd[128];
        nandRemoveTailToken(new_wd, wd);
        nandConvertPath(abspath, new_wd, new_relpath);
    }
    else if (token[0] != '\0') {
        char new_wd[128];
        if (strcmp(wd, "/") == 0) {
            sprintf(new_wd, "/%s", token);
        }
        else {
            sprintf(new_wd, "%s/%s", wd, token);
        }

        nandConvertPath(abspath, new_wd, new_relpath);
    }
    else {
        strcpy(abspath, wd);
    }
}

inline BOOL nandIsRelativePath(const char* path) {
    if (path[0] == '/') {
        return FALSE;
    }
    else {
        return TRUE;
    }
}

BOOL nandIsPrivatePath(const char* path) {
    if (strncmp(path, "/shared2", 8) == 0) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

BOOL nandIsUnderPrivatePath(const char* path) {
    if ((strncmp(path, "/shared2/", 9) == 0) && (path[9] != '\0')) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

BOOL nandIsInitialized(void) {
    if (s_libState == STATE_INITIALIZED) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

void nandReportErrorCode(const ISFSError err) NO_INLINE {
    return;
}

s32 nandConvertErrorCode(const ISFSError err) {
    const int ERRMAP[]=
    {
        ISFS_ERROR_OK,             NAND_RESULT_OK,
        ISFS_ERROR_ACCESS,         NAND_RESULT_ACCESS,
        ISFS_ERROR_CORRUPT,        NAND_RESULT_CORRUPT,
        ISFS_ERROR_ECC_CRIT,       NAND_RESULT_ECC_CRIT,
        ISFS_ERROR_EXISTS,         NAND_RESULT_EXISTS,
        ISFS_ERROR_HMAC,           NAND_RESULT_AUTHENTICATION,
        ISFS_ERROR_INVALID,        NAND_RESULT_INVALID,
        ISFS_ERROR_MAXBLOCKS,      NAND_RESULT_MAXBLOCKS,
        ISFS_ERROR_MAXFD,          NAND_RESULT_MAXFD,
        ISFS_ERROR_MAXFILES,       NAND_RESULT_MAXFILES,
        ISFS_ERROR_MAXDEPTH,       NAND_RESULT_MAXDEPTH,
        ISFS_ERROR_NOEXISTS,       NAND_RESULT_NOEXISTS,
        ISFS_ERROR_NOTEMPTY,       NAND_RESULT_NOTEMPTY,
        ISFS_ERROR_NOTREADY,       NAND_RESULT_UNKNOWN,
        ISFS_ERROR_OPENFD,         NAND_RESULT_OPENFD,
        ISFS_ERROR_UNKNOWN,        NAND_RESULT_UNKNOWN,
        ISFS_ERROR_BUSY,           NAND_RESULT_BUSY,
        ISFS_ERROR_SHUTDOWN,       NAND_RESULT_FATAL_ERROR,

        IOS_ERROR_ACCESS,          NAND_RESULT_ACCESS,
        IOS_ERROR_EXISTS,          NAND_RESULT_EXISTS,
        IOS_ERROR_INTR,            NAND_RESULT_UNKNOWN,
        IOS_ERROR_INVALID,         NAND_RESULT_INVALID,
        IOS_ERROR_MAX,             NAND_RESULT_UNKNOWN,
        IOS_ERROR_NOEXISTS,        NAND_RESULT_NOEXISTS,
        IOS_ERROR_QEMPTY,          NAND_RESULT_UNKNOWN,
        IOS_ERROR_QFULL,           NAND_RESULT_BUSY,
        IOS_ERROR_UNKNOWN,         NAND_RESULT_UNKNOWN,
        IOS_ERROR_NOTREADY,        NAND_RESULT_UNKNOWN,
        IOS_ERROR_ECC,             NAND_RESULT_UNKNOWN,
        IOS_ERROR_ECC_CRIT,        NAND_RESULT_ECC_CRIT,
        IOS_ERROR_BADBLOCK,        NAND_RESULT_UNKNOWN,

        IOS_ERROR_INVALID_OBJTYPE, NAND_RESULT_UNKNOWN,
        IOS_ERROR_INVALID_RNG,     NAND_RESULT_UNKNOWN,
        IOS_ERROR_INVALID_FLAG,    NAND_RESULT_UNKNOWN,
        IOS_ERROR_INVALID_FORMAT,  NAND_RESULT_UNKNOWN,
        IOS_ERROR_INVALID_VERSION, NAND_RESULT_UNKNOWN,
        IOS_ERROR_INVALID_SIGNER,  NAND_RESULT_UNKNOWN,
        IOS_ERROR_FAIL_CHECKVALUE, NAND_RESULT_UNKNOWN,
        IOS_ERROR_FAIL_INTERNAL,   NAND_RESULT_UNKNOWN,
        IOS_ERROR_FAIL_ALLOC,      NAND_RESULT_ALLOC_FAILED,
        IOS_ERROR_INVALID_SIZE,    NAND_RESULT_UNKNOWN,
    };

    int i = 0;

    if (err >= 0) {
        return err;
    }

    for (; i < sizeof(ERRMAP) / 4; i = i + 2) {
        if (ERRMAP[i] == err) {
            if (err == ISFS_ERROR_ECC_CRIT || err == ISFS_ERROR_HMAC || err == ISFS_ERROR_UNKNOWN || err == IOS_ERROR_UNKNOWN || err == IOS_ERROR_ECC_CRIT) {
                char buf[128] __attribute__ ((aligned(64)));
                sprintf(buf, "ISFS error code: %d", err);
                NANDLoggingAddMessageAsync(0, buf);
            }

            nandReportErrorCode(err);
            return ERRMAP[i + 1];
        }
    }

    OSReport("CAUTION!  Unexpected error code [%d] was found.\n", err);
    {
        char buf[128] __attribute__ ((aligned(64)));
        sprintf(buf, "ISFS unexpected error code: %d", err);
        NANDLoggingAddMessageAsync(0, buf);
    }

    nandReportErrorCode(err);
    return -64;
}

void nandGenerateAbsPath(char* absPath, const char* path) {
    if (strlen(path) == 0) {
        strcpy(absPath, "");
    }
    else if (nandIsRelativePath(path)) {
        nandConvertPath(absPath, s_currentDir, path);
    }
    else {
        u32 len = 0xFFFFFFFF;
        strcpy(absPath, path);
        
        len = strlen(absPath);
        if (len > 0) {
            if ((absPath[len - 1] == '/') && (len - 1 != 0)) {
                absPath[len - 1] = '\0';
            }
        }
    }
}

void nandGetParentDirectory(char* parentDir, const char* absPath) {
    int i = 0;
    for (i = (int)strlen(absPath); i >= 0; --i) {
        if (absPath[i] == '/') {
            break;
        }
    }

    if (i == 0) {
        strcpy(parentDir, "/");
    }
    else {
        strncpy(parentDir, absPath, (u32)i);
        parentDir[i] = '\0';
    }
}

s32 NANDInit(void) {
    BOOL enabled = OSDisableInterrupts();

    if (s_libState == STATE_WORKING) {
        OSRestoreInterrupts(enabled);
        return -3;
    }
    else if (s_libState == 2) {
        OSRestoreInterrupts(enabled);
        return 0;
    }
    else {
        ISFSError result = -117;
        s_libState = STATE_WORKING;
        OSRestoreInterrupts(enabled);

        result = ISFS_OpenLib();
        if (result == 0) {
            s32 rv;
            ESTitleId id;

            rv = ESP_InitLib();

            if (rv == 0) {
                rv = ESP_GetTitleId(&id);
            }

            if (rv == 0) {
                rv = ESP_GetDataDir(id, s_homeDir);
            }

            if (rv == 0) {
                strcpy(s_currentDir, s_homeDir);
            }

            ESP_CloseLib();

            if (rv != 0) {
                OSReport("Failed to set home directory.\n");
            }

            OSRegisterShutdownFunction(&s_shutdownFuncInfo);
            enabled = OSDisableInterrupts();
            s_libState = STATE_INITIALIZED;
            OSRestoreInterrupts(enabled);
            OSRegisterVersion(__NANDVersion);
            return 0;
        }
        else {
            enabled = OSDisableInterrupts();
            s_libState = STATE_NOT_INITIALIZED;
            OSRestoreInterrupts(enabled);
            return nandConvertErrorCode(result);
        }
    }
}

static BOOL nandOnShutdown(BOOL final, u32 event) {
    if (!final) {
        if (event == 2) {
            volatile BOOL flag = FALSE;
            OSTime t = OSGetTime();
            ISFS_ShutdownAsync(nandShutdownCallback, (void*)&flag);

            while (OSTicksToMilliseconds(OSGetTime() - t) < 500) {
                if (flag) {
                    break;
                }
            }
        }

        return TRUE;
    }
    else {
        return TRUE;
    }
}

static void nandShutdownCallback(ISFSError result, void* ctxt) {
    (void)result;
    *(BOOL*)ctxt = TRUE;
}

s32 NANDGetHomeDir(char* path) {
    if (!nandIsInitialized()) {
        return -128;
    }

    strcpy(path, s_homeDir);
    return 0;
}

void nandCallback(ISFSError result, void* ctxt) {
    NANDCommandBlock* b = (NANDCommandBlock*)ctxt;
    ((NANDCallback)(b->callback))(nandConvertErrorCode(result), b);
}

void nandGetTypeCallback(ISFSError result, void* ctxt);

static ISFSError nandGetType(const char* path, u8* type, NANDCommandBlock* block, const BOOL async_flag, const BOOL privilege_flag) {
    if (strlen(path) == 0) {
        return ISFS_ERROR_INVALID;
    }

    if (async_flag) {
        nandGenerateAbsPath(block->absPath, path);
        if (!privilege_flag && nandIsUnderPrivatePath(block->absPath)) {
            return ISFS_ERROR_ACCESS;
        }
        else {
            block->type = type;
            return ISFS_ReadDirAsync((u8*)(block->absPath), NULL, &(block->num), nandGetTypeCallback, block);
        }
    }
    else {
        char absPath[64] = "";
        nandGenerateAbsPath(absPath, path);

        if (!privilege_flag && nandIsUnderPrivatePath(absPath)) {
            return ISFS_ERROR_ACCESS;
        }
        else {
            u32 dmy = 0;
            ISFSError err = ISFS_ReadDir((u8*)absPath, NULL, &dmy);

            if ((err == ISFS_ERROR_OK) || (err == ISFS_ERROR_ACCESS)) {
                *type = 2;
                err = ISFS_ERROR_OK;
            }
            else if (err == ISFS_ERROR_INVALID) {
                *type = 1;
                err = ISFS_ERROR_OK;
            }

            return err;
        }
    }
}

s32 NANDPrivateGetTypeAsync(const char* path, u8* type, NANDCallback cb, NANDCommandBlock* block) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    block->callback = cb;
    return nandConvertErrorCode(nandGetType(path, type, block, TRUE, TRUE));
}

void nandGetTypeCallback(ISFSError result, void* ctxt) {
    NANDCommandBlock* b = (NANDCommandBlock*)ctxt;

    if ((result == ISFS_ERROR_OK) || (result == ISFS_ERROR_ACCESS)) {
        *(b->type) = 2;
        result = ISFS_ERROR_OK;
    }
    else if (result == ISFS_ERROR_INVALID) {
        *(b->type) = 1;
        result = ISFS_ERROR_OK;
    }

    ((NANDCallback)(b->callback))(nandConvertErrorCode(result), b);
}

const char* nandGetHomeDir(void) {
    return s_homeDir;
}

void NANDInitBanner(NANDBanner* bnr, u32 const flag, const u16* title, const u16* comment) {
    memset(bnr, 0, sizeof(NANDBanner));
    bnr->signature = 0x5749424E;
    bnr->flag = flag;

    if (wcscmp((wchar_t*)title, L"") == 0) {
        wcsncpy(bnr->comment[0], L" ", 32);
    }
    else {
        wcsncpy(bnr->comment[0], (wchar_t*)title, 32);
    }

    if (wcscmp((wchar_t*)comment, L"") == 0) {
        wcsncpy(bnr->comment[1], L" ", 32);
    }
    else {
        wcsncpy(bnr->comment[1], (wchar_t*)comment, 32);
    }
}