#include <revolution/os.h>
#include <revolution/dvd.h>
#include <revolution/os/OSBootInfo.h>
#include <cstring>

#define MIN(a, b) (((a) > (b))? (b) : (a))

extern DVDErrorInfo __ErrorInfo;
static DVDCommandBlock* executing;

static DVDBB2 BB2 __attribute__ ((aligned(32)));
static DVDDiskID CurrDiskID __attribute__ ((aligned(32)));

static DVDDiskID* IDShouldBe;
static OSBootInfo* bootInfo;
static BOOL autoInvalidation = TRUE;

static DVDCommandBlock DummyCommandBlock;
DVDCommandBlock __DVDStopMotorCommandBlock;
DVDCommandBlock __DVDRestartMotorCommandBlock;
extern OSThreadQueue __DVDThreadQueue;

static vu32 CurrCommand;
static vu32 Canceling = FALSE;
static DVDCBCallback CancelCallback;
static vu32 LastError;
static vs32 NumInternalRetry = 0;
static volatile BOOL ResetRequired;
static volatile BOOL PauseFlag;
static volatile BOOL PausingFlag;
static volatile BOOL AutoFinishing = FALSE;
static volatile BOOL FatalErrorFlag = FALSE;
static vu32 WaitingForCoverOpen  = FALSE;
static vu32 WaitingForCoverClose = FALSE;
static vu32 MotorStopped = FALSE;
static vu32 ChangedDisc = FALSE;
static volatile BOOL PreparingCover = FALSE;
static u32 MotorState;
static volatile OSTime LastResetEnd;
static vu32 CoverState;

static vu32 ResumeFromHere = 0;
static vu32 CancelLastError;
static volatile BOOL FirstTimeInBootrom = FALSE;

static OSAlarm ResetAlarm;
static OSAlarm CoverAlarm;

extern BOOL __OSInIPL;

static vu32 Breaking = FALSE;

static u32 __DVDNumTmdBytes  __attribute__ ((aligned(32)));
static u8  __DVDGameTocBuffer[OSRoundUp32B(sizeof(DVDGameTOC) * 4)] __attribute__ ((aligned(32)));
static u8  __DVDPartInfoBuffer[OSRoundUp32B(sizeof(DVDPartitionInfo) * 4)] __attribute__ ((aligned(32)));
static u8 __DVDTmdBuffer[OSRoundUp32B(sizeof(ESTitleMeta))]  __attribute__ ((aligned(32)));
static u8 __DVDTicketViewBuffer[OSRoundUp32B(sizeof(ESTicketView))] __attribute__ ((aligned(32)));

vu16   __OSDeviceCode : (OS_BASE_CACHED | 0x30E6);
vu8 __OSLockedFlag : (OS_BASE_CACHED | 0x3187);
vu32 __OSLaunchPartitionType : (OS_BASE_CACHED | 0x3194);

typedef void (*stateFunc)(DVDCommandBlock* block);
stateFunc LastState;

void defaultOptionalCommandChecker(DVDCommandBlock* block, DVDLowCallback cb) {
    return;
}

static DVDOptionalCommandChecker checkOptionalCommand = defaultOptionalCommandChecker;

volatile u32 CommandInfoCounter = 0;

__declspec(weak) void StampCommand(u32 command, u32 offset, u32 length) {
    BOOL enabled = OSDisableInterrupts();

    if (CommandInfoCounter >= 5) {
        CommandInfoCounter = 0;
    }

    __ErrorInfo.lastCommand[CommandInfoCounter].command = command;
    __ErrorInfo.lastCommand[CommandInfoCounter].offset = offset;
    __ErrorInfo.lastCommand[CommandInfoCounter].length = length;
    __ErrorInfo.lastCommand[CommandInfoCounter].tick = OSGetTick();
    CommandInfoCounter++;
    OSRestoreInterrupts(enabled);
}

void StampIntType(u32 intType) {
    BOOL enabled = OSDisableInterrupts();

    if (CommandInfoCounter == 0) {
        __ErrorInfo.lastCommand[4].intType = intType;
    }
    else {
        __ErrorInfo.lastCommand[CommandInfoCounter - 1].intType = intType;
    }

    OSRestoreInterrupts(enabled);
}

volatile u32  __DVDLayoutFormat = 2;
static BOOL DVDInitialized = FALSE;
const char* __DVDVersion = "<< RVL_SDK - DVD \trelease build: Feb 22 2008 06:17:35 (0x4199_60831) >>";

static void stateDownRotation(DVDCommandBlock *);
static void stateGettingError(void);
static void stateReadingFST(void);
static void stateReady(void);
static void stateGoToRetry(void);
static void stateBusy(DVDCommandBlock *);
static void stateReadingTOC(DVDCommandBlock *);
static void stateCheckID2a(DVDCommandBlock *);
static void stateCheckID3(DVDCommandBlock *);
static void stateReadingPartitionInfo(DVDCommandBlock *);
static void stateOpenPartition(DVDCommandBlock* );
static void stateOpenPartition2(DVDCommandBlock *);
static void stateTimeout(void);
static void stateSecurityError(void);
static void stateMotorStopped(void);
static void stateCoverClosed(void);
static void stateCoverClosed_CMD(DVDCommandBlock *);

static void cbForStateReadingFST(u32);
static void cbForStateGoToRetry(u32);
static void cbForStateCheckID1(u32);
static void cbForStateCheckID2(u32);
static void cbForStateCheckID2a(u32);
static void cbForStateCheckID3(u32);
static void cbForStateGettingError(u32);
static void cbForUnrecoveredErrorRetry(u32);
static void cbForUnrecoveredError(u32);
static void cbForStateMotorStopped(u32);
static void cbForStateBusy(u32);
static void cbForStateError(u32);
static void cbForStateDownRotation(u32);
static void cbForStateReset(u32);
static void cbForStoreErrorCode1(s32, DVDCommandBlock *);
static void cbForStoreErrorCode2(s32, DVDCommandBlock *);
static void stateCheckID2(DVDCommandBlock *);
static void cbForStateCoverClosed(u32);
static void cbForStateOpenPartition(u32);

static void CoverAlarmHandler(OSAlarm *, OSContext *);
static void ResetAlarmHandler(OSAlarm *, OSContext *);

void DVDInit(void) {
    DVDDiskID* id;
    s32 rv;

    if (DVDInitialized) {
        return;
    }

    OSRegisterVersion(__DVDVersion);
    DVDInitialized = TRUE;
    DVDLowInit();
    
    if (!__OSInIPL && __OSLockedFlag == 0x80) {
        rv = ESP_InitLib();

        if (rv == 0) {
            rv = ESP_DiGetTicketView(NULL, (ESTicketView*)__DVDTicketViewBuffer);
        }

        if (rv == 0) {
            rv = ESP_DiGetTmd(NULL, &__DVDNumTmdBytes);
        }

        if (rv == 0) {
            rv = ESP_DiGetTmd((ESTitleMeta*)__DVDTmdBuffer, &__DVDNumTmdBytes);
        }

        ESP_CloseLib();
    }

    __DVDFSInit();
    __DVDClearWaitingQueue();

    MotorState = 0;
    bootInfo = (OSBootInfo*)OSPhysicalToCached(0);
    IDShouldBe = &bootInfo->DVDDiskID;
    OSInitThreadQueue(&__DVDThreadQueue);

    DVDLowUnmaskStatusInterrupts();
    DVDLowMaskCoverInterrupt();

    if (bootInfo->magic == 0xE5207C22) {

    }
    else if (bootInfo->magic == 0x0D15EA5E) {

    }
    else {
        FirstTimeInBootrom = TRUE;
    }

    memset(&__ErrorInfo, 0, sizeof(DVDErrorInfo));
    id = (DVDDiskID*)OSPhysicalToCached(0);
    memcpy(__ErrorInfo.gameName, id->gameName, 4);
    __ErrorInfo.diskNumber = id->diskNumber;
    __ErrorInfo.gameVersion = id->gameVersion;
    __DVDLayoutFormat = 0;
    DVDSetAutoFatalMessaging(TRUE);
}

static void stateReadingFST(void) {
    LastState = (stateFunc)stateReadingFST;

    if (!(bootInfo->FSTMaxLength >= BB2.FSTLength)) {
        OSPanic(__FILE__, 0x43B, "DVDChangeDisk(): FST in the new disc is too big.   ");
    }

    DVDLowClearCoverInterrupt(NULL);
    StampCommand(1, BB2.FSTPosition >> __DVDLayoutFormat, OSRoundUp32B(BB2.FSTLength << (~__DVDLayoutFormat & 2)));
    DVDLowRead(bootInfo->FSTLocation, OSRoundUp32B(BB2.FSTLength << (~__DVDLayoutFormat & 2)), BB2.FSTPosition >> __DVDLayoutFormat, cbForStateReadingFST);
}

static void cbForStateReadingFST(u32 intType) {
    DVDCommandBlock* finished;
    StampIntType(intType);

    if (intType == 16) {
        stateTimeout();
        return;
    }

    if (intType == 32) {
        stateSecurityError();
        return;
    }

    if (intType & 1) {
        NumInternalRetry = 0;
        __DVDFSInit();
        finished = executing;
        executing = &DummyCommandBlock;
        finished->state = 0;

        if (finished->callback) {
            (finished->callback)(0, finished);
        }

        stateReady();
    }
    else {
        stateGettingError();
    }
}

static OSAlarm FatalAlarm;
static void FatalAlarmHandler(OSAlarm* alarm, OSContext* context) {
    __DVDPrintFatalMessage();
}

static void cbForStateError(u32 intType) {
    DVDCommandBlock* finished;

    if (__DVDGetAutoFatalMessaging()) {
        OSCreateAlarm(&FatalAlarm);
        OSSetAlarm(&FatalAlarm, 1, FatalAlarmHandler);
    }
    else {
        executing->state = -1;

        if (intType == 16) {
            stateTimeout();
            return;
        }

        if (intType == 32) {
            stateSecurityError();
            return;
        }

        FatalErrorFlag = FALSE;
        finished = executing;
        executing = &DummyCommandBlock;

        if (finished->callback) {
            (finished->callback)(-1, finished);
        }

        if (Canceling) {
            Canceling = FALSE;

            if (CancelCallback) {
                (CancelCallback)(0, finished);
            }
        }

        stateReady();
    }

    return;
}

static void cbForStoreErrorCode1(s32 result, DVDCommandBlock* block) {
    DVDLowStopMotor(FALSE, FALSE, cbForStateError);
}

static void stateError(u32 error) {
    __DVDStoreErrorCode(error, cbForStoreErrorCode1);
}

static void cbForStoreErrorCode2(s32 result, DVDCommandBlock* block) {
    DVDLowSetSpinupFlag(0);
    DVDLowReset(cbForStateError);
    ResetRequired = FALSE;
    ResumeFromHere = 0;
}

static void stateTimeout(void) {
    __DVDStoreErrorCode(0x1234568, cbForStoreErrorCode2);
}

static void stateSecurityError(void) {
    __DVDStoreErrorCode(0x1234569, cbForStoreErrorCode2);
}

static void stateGettingError(void) {
    StampCommand(39, 0, 0);
    DVDLowRequestError(cbForStateGettingError);
}

u32 CategorizeError(u32 error) {
    if (error == 0x20400) {
        LastError = error;
        return 1;
    }

    error &= 0xFFFFFF;

    if (error == 0x62800 || error == 0x23A00 || error == 0x53000 || error == 0xB5A01) {
        return 0;
    }

    if ((error == 0x52000) && ((executing->command == 37) || (LastState == stateDownRotation)) ) {
        return 0;
    }

    NumInternalRetry++;
    if (NumInternalRetry == 2) {
        if (error == LastError) {
            LastError = error;
            return 1;
        }
        else {
            LastError = error;
            return 2;
        }
    }
    else {
        LastError = error;

        if (error == 0x31100 || executing->command == 5) {
            return 2;
        }
        else {
            return 3;
        }
    }
}

static BOOL CheckCancel(u32 resume) {
    DVDCommandBlock* finished;

    if (Canceling) {
        ResumeFromHere = resume;
        Canceling = FALSE;
        finished = executing;
        executing = &DummyCommandBlock;

        finished->state = 10;
        if (finished->callback) {
            (*finished->callback)(-3, finished);
        }

        if (CancelCallback) {
            (CancelCallback)(0, finished);
        }

        stateReady();
        return TRUE;
    }
    else {
        return FALSE;
    }
}

static void cbForStoreErrorCode3(s32 result, DVDCommandBlock* block) {
    stateGoToRetry();
}

static void cbForStateGettingError(u32 intType) {
    u32 error, status, errorCategory, resume;

    StampIntType(intType);

    if (intType == 16) {
        stateTimeout();
        return;
    }

    if (intType == 32) {
        stateSecurityError();
        return;
    }

    if (intType & 2) {
        stateError(0x1234567);
        return;
    }

    error = DVDLowGetImmBufferReg();
    status = error & 0xFF000000;
    errorCategory = CategorizeError(error);

    if (errorCategory == 1) {
        stateError(error);
        return;
    }

    if ((errorCategory == 2) || (errorCategory == 3)) {
        resume = 0;
    }
    else {
        if (status == 0x1000000) {
            resume = 4;
        }
        else if (status == 0x2000000) {
            resume = 6;
        }
        else if (status == 0x3000000) {
            resume = 3;
        }
        else if (status == 0) {
            if (error == 0x53000) {
                resume = 1;
            }
            else {
                resume = 5;
            }
        }
        else {
            resume = 5;
        }
    }

    if (CheckCancel(resume)) {
        return;
    }

    if (errorCategory == 2) {
        __DVDStoreErrorCode(error, cbForStoreErrorCode3);
        return;
    }
    
    if (errorCategory == 3) {
        if ((error & 0xFFFFFF) == 0x31100) {
            StampCommand(2, executing->offset, 0);
            DVDLowSeek(executing->offset, cbForUnrecoveredError);
        }
        else {
            LastState(executing);
        }

        return;
    }

    if (status == 0x1000000) {
        executing->state = 5;
        stateMotorStopped();
        return;
    }
    else if (status == 0x2000000) {
        executing->state = 3;
        stateCoverClosed();
        return;
    }
    else if (status == 0x3000000) {
        executing->state = 4;
        stateMotorStopped();
        return;
    }
    else if (status == 0) {
        if (error == 0x53000) {
            StampCommand(16, 0, 0);
            DVDLowStopMotor(FALSE, FALSE, cbForStateCheckID1);
            return;
        }
        else {
            stateError(0x1234567);
            return;
        }
    }
    else {
        stateError(0x1234567);
        return;
    }
}

static void cbForUnrecoveredError(u32 intType) {
    StampIntType(intType);

    if (intType == 16) {
        stateTimeout();
        return;
    }

    if (intType == 32) {
        stateSecurityError();
        return;
    }

    if (intType & 1) {
        stateGoToRetry();
        return;
    }

    StampCommand(39, 0, 0);
    DVDLowRequestError(cbForUnrecoveredErrorRetry);
}

static void cbForUnrecoveredErrorRetry(u32 intType) {
    StampIntType(intType);

    if (intType == 16) {
        stateTimeout();
        return;
    }

    if (intType == 32) {
        stateSecurityError();
        return;
    }

    if (intType & 2) {
        stateError(0x1234567);
    }
    else {
        stateError(DVDLowGetImmBufferReg());
    }

    return;
}

static void stateGoToRetry(void) {
    StampCommand(16, 0, 0);
    DVDLowStopMotor(FALSE, FALSE, cbForStateGoToRetry);
}

static void cbForStateGoToRetry(u32 intType) {
    StampIntType(intType);

    if (intType == 16) {
        stateTimeout();
        return;
    }

    if (intType == 32) {
        stateSecurityError();
        return;
    }

    if (intType & 2) {
        stateError(0x1234567);
        return;
    }

    NumInternalRetry = 0;

    if ((CurrCommand == 4 || CurrCommand == 5 || CurrCommand == 13 || CurrCommand == 33 || CurrCommand == 34 || CurrCommand == 41 || CurrCommand == 42 || CurrCommand == 15 || CurrCommand == 37)) {
        ResetRequired = TRUE;
    }

    if (!CheckCancel(2)) {
        executing->state = 11;
        stateMotorStopped();
    }
}

void stateCheckID(void) {
    switch (CurrCommand) {
        case 3:
            ChangedDisc = FALSE;

            if (DVDCompareDiskID(&CurrDiskID, executing->id)) {
                memcpy(IDShouldBe, &CurrDiskID, sizeof(DVDDiskID));
                executing->state = 1;
                DCInvalidateRange(&BB2, sizeof(DVDBB2));
                NumInternalRetry = 0;
                stateReadingTOC(executing);
                return;
            }
            else {
                StampCommand(16, 0, 0);
                DVDLowStopMotor(FALSE, FALSE, cbForStateCheckID1);
            }
            break;

        default:
            if (memcmp(&CurrDiskID, IDShouldBe, sizeof(DVDDiskID))) {
                StampCommand(16, 0, 0);
                DVDLowStopMotor(FALSE, FALSE, cbForStateCheckID1);
            }
            else {
                NumInternalRetry = 0;
                stateReadingTOC(executing);
            }

            break;
    }
}

static void stateCheckID3(DVDCommandBlock* block) {
    StampCommand(13, IDShouldBe->streaming, 10);
    DVDLowAudioBufferConfig((BOOL)IDShouldBe->streaming, 10, cbForStateCheckID3);
}

static void stateCheckID2a(DVDCommandBlock* block) {
    StampCommand(13, IDShouldBe->streaming, 10);
    DVDLowAudioBufferConfig((BOOL)IDShouldBe->streaming, 10, cbForStateCheckID2a);
}

static void cbForStateCheckID2a(u32 intType) {
    StampIntType(intType);

    if (intType == 16) {
        stateTimeout();
        return;
    }

    if (intType == 32) {
        stateSecurityError();
        return;
    }

    if (intType & 1) {
        NumInternalRetry = 0;
        stateCheckID2(executing);
    }
    else {
        stateGettingError();
    }
}

static DVDGameTOC* GameToc;
static DVDPartitionInfo* PartInfo;
static DVDPartitionInfo* BootGameInfo;
static ESTitleMeta* Tmd;

static void cbForStateReadingTOC(u32 intType) {
    StampIntType(intType);

    if (intType == 16) {
        stateTimeout();
        return;
    }

    if (intType == 32) {
        stateSecurityError();
        return;
    }

    if (intType & 1) {
        NumInternalRetry = 0;
        GameToc = (DVDGameTOC*)__DVDGameTocBuffer;
        stateReadingPartitionInfo(executing);
    }
    else {
        stateGettingError();
    }
}

static void stateReadingTOC(DVDCommandBlock* block) {
    DVDLowClearCoverInterrupt(NULL);
    StampCommand(33, 0x40000 >> 2, OSRoundUp32B(sizeof(DVDGameTOC)));
    DVDLowUnencryptedRead(__DVDGameTocBuffer, OSRoundUp32B(sizeof(DVDGameTOC)), 0x10000, cbForStateReadingTOC);
}

static void cbForStateReadingPartitionInfo(u32 intType) {
    StampIntType(intType);

    if (intType == 16) {
        stateTimeout();
        return;
    }

    if (intType == 32) {
        stateSecurityError();
        return;
    }

    if (intType & 1) {
        s16 i;
        NumInternalRetry = 0;
        PartInfo = (DVDPartitionInfo*)__DVDPartInfoBuffer;
        BootGameInfo = NULL;

        if (*((u32*)OSPhysicalToCached(0x3198))) {
            BootGameInfo = PartInfo;
            BootGameInfo->type = *((u32*)OSPhysicalToCached(0x3194));
            BootGameInfo->gamePartition = (DVDGamePartition*)*((u32*)OSPhysicalToCached(0x3198));
        }
        else {
            for ( i = 0; i < GameToc->numGamePartitions; i++) {
                if (PartInfo->type == __OSLaunchPartitionType) {
                    BootGameInfo = PartInfo;
                }

                PartInfo++;
            }
        }

        if (BootGameInfo) {
            switch (CurrCommand) {
                case 3:
                    NumInternalRetry = 0;
                    stateOpenPartition(executing);
                    break;

                default:
                    NumInternalRetry = 0;
                    stateOpenPartition2(executing);
                    break;
            }
        }
        else {
            if (!CheckCancel(1)) {
                executing->state = 6;
                stateMotorStopped();
            }
        }
    }
    else {
        stateGettingError();
    }
}

static void stateReadingPartitionInfo(DVDCommandBlock* block) {
    DVDLowClearCoverInterrupt(0);
    StampCommand(33, (0x40000 + OSRoundUp32B(sizeof(DVDGameTOC))) >> 2, OSRoundUp32B(sizeof(DVDPartitionInfo)));
    DVDLowUnencryptedRead(__DVDPartInfoBuffer, OSRoundUp32B(sizeof(DVDPartitionInfo)), (0x40000 + OSRoundUp32B(sizeof(DVDGameTOC))) >> 2, cbForStateReadingPartitionInfo);
}

static void stateOpenPartition(DVDCommandBlock* block) {
    DVDLowClearCoverInterrupt(0);
    StampCommand(34, (u32)BootGameInfo->gamePartition, 0);

    if (__OSLockedFlag == 0x80) {
        DVDLowOpenPartitionWithTmdAndTicketView((u32)BootGameInfo->gamePartition, (ESTicketView*)__DVDTicketViewBuffer, __DVDNumTmdBytes, (ESTitleMeta*)__DVDTmdBuffer, 0, NULL, cbForStateOpenPartition);
    }
    else {
        DVDLowOpenPartition((u32)BootGameInfo->gamePartition, NULL, 0, 0, (ESTitleMeta*)__DVDTmdBuffer, cbForStateOpenPartition);
    }
}

static void cbForStateOpenPartition(u32 intType) {
    StampIntType(intType);

    if (intType == 16) {
        stateTimeout();
        return;
    }

    if (intType == 32) {
        stateSecurityError();
        return;
    }

    if (intType & 1) {
        NumInternalRetry = 0;
        stateCheckID2(executing);
    }
    else {
        stateGettingError();
    }
}

static void cbForStateOpenPartition2(u32 intType) {
    StampIntType(intType);

    if (intType == 16) {
        stateTimeout();
        return;
    }

    if (intType == 32) {
        stateSecurityError();
        return;
    }

    if (intType & 1) {
        NumInternalRetry = 0;

        if (!CheckCancel(0)) {
            executing->state = 1;
            stateBusy(executing);
        }
    }
    else {
        stateGettingError();
    }
}

static void stateOpenPartition2(DVDCommandBlock* block) {
    DVDLowClearCoverInterrupt(0);
    StampCommand(34, (u32)BootGameInfo->gamePartition, 0);

    if (__OSLockedFlag == 0x80) {
        DVDLowOpenPartitionWithTmdAndTicketView((u32)BootGameInfo->gamePartition, (ESTicketView*)__DVDTicketViewBuffer, __DVDNumTmdBytes, (ESTitleMeta*)__DVDTmdBuffer, 0, NULL, cbForStateOpenPartition2);
    }
    else {
        DVDLowOpenPartition((u32)BootGameInfo->gamePartition, NULL, 0, 0, (ESTitleMeta*)__DVDTmdBuffer, cbForStateOpenPartition2);
    }
}

static void stateCheckID2(DVDCommandBlock* block) {
    DVDLowClearCoverInterrupt(0);
    StampCommand(1, (u32)(0x420 >> 2), OSRoundUp32B(sizeof(DVDBB2)));
    DVDLowRead(&BB2, OSRoundDown32B(sizeof(DVDBB2)), (u32)(0x420 >> 2), cbForStateCheckID2);
}

static void cbForStateCheckID1(u32 intType) {
    StampIntType(intType);

    if (intType == 16) {
        stateTimeout();
        return;
    }

    if (intType == 32) {
        stateSecurityError();
        return;
    }

    if (intType & 2) {
        stateError(0x1234567);
        return;
    }

    NumInternalRetry = 0;

    if (!CheckCancel(1)) {
        executing->state = 6;
        stateMotorStopped();
    }
}

static void cbForStateCheckID2(u32 intType) {
    StampIntType(intType);

    if (intType == 16) {
        stateTimeout();
        return;
    }

    if (intType == 32) {
        stateSecurityError();
        return;
    }

    if (intType & 1) {
        NumInternalRetry = 0;
        stateReadingFST();
    }
    else {
        stateGettingError();
    }
}

static void cbForStateCheckID3(u32 intType) {
    StampIntType(intType);

    if (intType == 16) {
        stateTimeout();
        return;
    }

    if (intType == 32) {
        stateSecurityError();
        return;
    }

    if (intType & 1) {
        NumInternalRetry = 0;

        if (!CheckCancel(0)) {
            executing->state = 1;
            stateBusy(executing);
        }
    }
    else {
        stateGettingError();
    }
}


static void stateCoverClosed(void) {
    DVDCommandBlock* finished;
    MotorState = 1;

    switch (CurrCommand) {
        case 5:
        case 4:
        case 33:
        case 34:
        case 41:
        case 42:
        case 13:
        case 15:
        case 37:
            __DVDClearWaitingQueue();
            finished = executing;
            executing = &DummyCommandBlock;

            if (finished->callback) {
                (finished->callback)(-4, finished);
            }

            stateReady();
            break;

        case 32:
            MotorState = 0;

        case 35:
        case 38:
        case 36:
            executing->state = 1;
            stateBusy(executing);
            break;

        case 1:
        case 2:
            if (__OSInIPL) {
                break;
            }

        default:
            MotorState = 0;
            DVDLowSetSpinupFlag(1);
            DVDLowReset(cbForStateReset);
            break;
    }
}

static void ResetAlarmHandler(OSAlarm* alarm, OSContext* context) {
    if (__OSDeviceCode == (u16)(0x8000 | 0x003)) {
        LastState = stateDownRotation;
        stateDownRotation(executing);
    }
    else {
        DCInvalidateRange(&CurrDiskID, sizeof(DVDDiskID));
        LastState = stateCoverClosed_CMD;
        stateCoverClosed_CMD(executing);
    }
}

static void cbForStateReset(u32 intType) {
    if (intType == 16) {
        stateTimeout();
        return;
    }

    if (intType == 32) {
        stateSecurityError();
        return;
    }

    if (intType & 1) {
        LastResetEnd = __OSGetSystemTime();
        ResetRequired = FALSE;
        ResumeFromHere = 0;
        OSCreateAlarm(&ResetAlarm);
        OSSetAlarm(&ResetAlarm, DVD_RESETCOVER_TIMELAG_TICKS2, ResetAlarmHandler);
    }
    else {
        stateGettingError();
    }
}

extern BOOL __OSIsDiag;
void __DVDShowFatalMessageForSystem(u32 command, u32 intType);
static void stateMotorStopped(void) {
    MotorState = 1;

    if (WaitingForCoverOpen || WaitingForCoverClose) {
        return;
    }

    WaitingForCoverOpen = TRUE;
    OSCreateAlarm(&CoverAlarm);
    OSSetPeriodicAlarm(&CoverAlarm, OSGetTick(), OSMillisecondsToTicks(100), CoverAlarmHandler);
}

static void cbForStateMotorStopped(u32 intType) {
    WaitingForCoverClose = FALSE;

    if (CurrCommand == 3) {
        ChangedDisc = TRUE;
    }

    if (MotorState == 2) {
        if (executing) {
            executing->state = 12;
        }

        return;
    }

    DVDLowMaskCoverInterrupt();

    if (executing) {
        executing->state = 3;
        stateCoverClosed();
    }
    else {
        ResumeFromHere = 7;
    }
}



static u32 ImmCommand[] = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };
static u32 DmaCommand[] = { 0xFFFFFFFF };
#define USE_DMA(command) ((command == 1) ||   \
                        (command == 4) ||     \
                        (command == 5) ||     \
                        (command == 33) ||    \
                        (command == 14) )

#define REQUEST_AUDIO_STATUS(command)       ( (command == 9)    ||  \
                                    (command == 10)  ||  \
                                    (command == 11) ||  \
                                    (command == 12) )

static BOOL IsImmCommandWithResult(u32 command) {
    u32 i;

    if (REQUEST_AUDIO_STATUS(command)) {
        return TRUE;
    }

    for (i = 0; i < sizeof(ImmCommand) / sizeof(ImmCommand[0]); i++) {
        if (command == ImmCommand[i]) {
            return TRUE;
        }
    }

    return FALSE;
}

static BOOL IsDmaCommand(u32 command) {
    u32 i;

    if (USE_DMA(command)) {
        return TRUE;
    }

    for (i = 0 ; i < sizeof(DmaCommand) / (sizeof(DmaCommand[0])); i++) {
        if (command == DmaCommand[i]) {
            return TRUE;
        }
    }

    return FALSE;
}

static void cbForStateBusy(u32 intType) {
    DVDCommandBlock* finished;
    StampIntType(intType);

    if (intType == 16) {
        stateTimeout();
        return;
    }

    if (intType == 32) {
        stateSecurityError();
        return;
    }

    if ((CurrCommand == 3) || (CurrCommand == 15)) {
        if (intType & 2) {
            stateError(0x1234567);
            return;
        }

        NumInternalRetry = 0;

        if (CurrCommand == 15) {
            ResetRequired = TRUE;
        }

        if (CheckCancel(7)) {
            return;
        }

        if (MotorState != 2) {
            executing->state = 7;
            stateMotorStopped();
        }

        return;
    }
    else {
        if (IsDmaCommand(CurrCommand)) {
            executing->transferredSize += (intType & (8 | 1)) ? executing->currTransferSize : 0;
        }

        if (Breaking) {
            Breaking = FALSE;
            Canceling = FALSE;

            finished = executing;
            executing = &DummyCommandBlock;
            finished->state = 10;

            if (finished->callback) {
                (*finished->callback)(-3, finished);
            }

            if (CancelCallback) {
                (CancelCallback)(0, finished);
            }

            stateReady();
            return;
        }

        if (intType & 1) {
            NumInternalRetry = 0;

            if (CurrCommand == 16) {
                if (executing->offset) {
                    MotorState = 2;
                }
                else {
                    MotorState = 1;
                }

                finished = executing;
                executing = &DummyCommandBlock;
                finished->state = 0;

                if (finished->callback) {
                    (finished->callback)(0, finished);
                }

                stateReady();
                return;
            }

            if (CurrCommand == 32) {
                LastResetEnd = __OSGetSystemTime();

                ResetRequired = FALSE;
                ResumeFromHere = 0;

                finished = executing;
                executing = &DummyCommandBlock;

                finished->state = 0;
                if (finished->callback)
                {
                    (finished->callback)(0, finished);
                }

                stateReady();
                return;
            }

            if (CheckCancel(0)) {
                return;
            }

            if (CurrCommand == 38) {
                s32 retVal;
                u32 coverReg;

                coverReg = DVDLowGetCoverRegister();

                if (__OSGetSystemTime() - LastResetEnd < DVD_RESETCOVER_TIMELAG_TICKS2) {
                    retVal = 0;
                }
                else if (coverReg & 1) {
                    retVal = 1;
                }
                else {
                    retVal = 2;
                }

                finished = executing;
                executing = &DummyCommandBlock;

                finished->state  = 0;
                finished->offset = (u32)retVal;
                if (finished->callback) {
                    (finished->callback)(retVal, finished);
                }

                stateReady();
                return;
            }

            if (CurrCommand == 36) {
                s32 retVal;
                u32 coverReg;

                coverReg = DVDLowGetCoverRegister();
                if (((((unsigned long)(coverReg)) & 0x00000004) >> 2) || (coverReg & 1)) {
                    retVal = FALSE;
                }
                else {
                    if (ResumeFromHere != 0) {
                        retVal = FALSE;
                    }
                    else {
                        retVal = TRUE;
                    }
                }

                finished = executing;
                executing = &DummyCommandBlock;

                finished->state  = 0;
                finished->offset = (u32)retVal;

                if (finished->callback) {
                    (finished->callback)(retVal, finished);
                }

                stateReady();
                return;
            }

            if (CurrCommand == 40) {
                if (DVDCompareDiskID(&CurrDiskID, executing->id)) {
                    memcpy(IDShouldBe, &CurrDiskID, sizeof(DVDDiskID));
                    finished = executing;
                    executing = &DummyCommandBlock;

                    finished->state  = 0;
                    finished->offset = (u32)TRUE;
                    if (finished->callback) {
                        (finished->callback)(TRUE, finished);
                    }

                    NumInternalRetry = 0;

                    stateReady();
                    return;
                }
                else {
                    StampCommand(16, 0, 0);
                    DVDLowStopMotor(FALSE, FALSE, cbForStateCheckID1);
                    return;
                }
            }

            if (CurrCommand == 41) {
                DVDPartitionParams* params;

                params = (DVDPartitionParams*)(executing->addr);
                if (!params->dataWordOffset)
                {
                    stateBusy(executing);
                    return;
                }

                finished = executing;
                executing = &DummyCommandBlock;

                finished->state  = 0;
                if (finished->callback)
                {
                    (finished->callback)(0, finished);
                }

                stateReady();
                return;
            }

            if (IsDmaCommand(CurrCommand)) {
                if (executing->transferredSize != executing->length) {
                    stateBusy(executing);
                    return;
                }

                finished = executing;
                executing = &DummyCommandBlock;

                finished->state = 0;
                if (finished->callback) {
                    (finished->callback)((s32)finished->transferredSize, finished);
                }
                stateReady();
            }
            else if (IsImmCommandWithResult(CurrCommand)) {
                s32 result;

                if ((CurrCommand == 11) || (CurrCommand == 10)) {
                    result = (s32)(DVDLowGetImmBufferReg() << 2);
                }
                else {
                    result = (s32)DVDLowGetImmBufferReg();
                }

                finished = executing;
                executing = &DummyCommandBlock;

                finished->state = 0;
                if (finished->callback) {
                    (finished->callback)(result, finished);
                }

                stateReady();
            }
            else {
                finished = executing;
                executing = &DummyCommandBlock;

                finished->state = 0;
                if (finished->callback) {
                    (finished->callback)(0, finished);
                }
                stateReady();
            }
        }
        else {
            if (CurrCommand == 14) {
                stateError(0x1234567);
                return;
            }

            if ((USE_DMA(CurrCommand)) && (executing->transferredSize == executing->length)) {
                if (CheckCancel(0)) {
                    return;
                }

                finished = executing;
                executing = &DummyCommandBlock;
                finished->state = 0;

                if (finished->callback) {
                    (finished->callback)((s32)finished->transferredSize, finished);
                }

                stateReady();
                return;
            }

            stateGettingError();
        }
    }
}

static void stateDownRotation(DVDCommandBlock* block) {
    DVDLowClearCoverInterrupt(0);
    StampCommand(37, 0, 0);
    DVDLowSetMaximumRotation(0x20000, cbForStateDownRotation);
}

static void stateCoverClosed_CMD(DVDCommandBlock *);

static void cbForStateDownRotation(u32 intType) {
    StampIntType(intType);

    if (intType == 16) {
        stateTimeout();
        return;
    }

    if (intType == 32) {
        stateSecurityError();
        return;
    }

    if (intType & 1) {
        DCInvalidateRange(&CurrDiskID, sizeof(DVDDiskID));
        LastState = stateCoverClosed_CMD;
        stateCoverClosed_CMD(executing);
    }
    else {
        stateGettingError();
    }
}

static void stateCoverClosed_CMD(DVDCommandBlock* block) {
    if (CurrCommand == 40) {
        NumInternalRetry = 0;

        if (!CheckCancel(0)) {
            executing->state = 1;
            stateBusy(executing);
        }
    }
    else {
        DVDLowClearCoverInterrupt(0);
        StampCommand(5, 0, sizeof(DVDDiskID));
        DVDLowReadDiskID(&CurrDiskID, cbForStateCoverClosed);
    }
}

static void cbForStateCoverClosed(u32 intType) {
    StampIntType(intType);

    if (intType == 16) {
        stateTimeout();
        return;
    }

    if (intType == 32) {
        stateSecurityError();
        return;
    }

    if (intType & 1) {
        NumInternalRetry = 0;
        stateCheckID();
    }
    else {
        stateGettingError();
    }
}

static void cbForPrepareCoverRegister(u32 intType) {
    PreparingCover = FALSE;

    if (WaitingForCoverClose) {
        if (!(DVDLowGetCoverRegister() & 1)) {
            OSCancelAlarm(&CoverAlarm);
            WaitingForCoverClose = FALSE;
            cbForStateMotorStopped(4);
        }

        return;
    }

    if (DVDLowGetCoverRegister() & 1) {
        WaitingForCoverOpen = FALSE;
        WaitingForCoverClose = TRUE;

        if (MotorState == 2) {
            executing->state = 12;
        }
        else {
            executing->state = 5;
        }
    }
    else if (DVDLowGetCoverRegister() & 4) {
        OSCancelAlarm(&CoverAlarm);
        WaitingForCoverOpen = FALSE;
        DVDLowClearCoverInterrupt(0);
        cbForStateMotorStopped(4);
    }
}

static void CoverAlarmHandler(OSAlarm* alarm, OSContext* context) {
    if (!PreparingCover) {
        PreparingCover = TRUE;
        DVDLowPrepareCoverRegister(cbForPrepareCoverRegister);
    }
}

static void stateReady(void) {
    DVDCommandBlock* finished;

    if (!__DVDCheckWaitingQueue()) {
        executing = NULL;
        return;
    }

    if (PauseFlag) {
        PausingFlag = TRUE;
        executing = NULL;
        return;
    }

    executing = __DVDPopWaitingQueue();

    if (FatalErrorFlag) {
        executing->state = -1;
        finished = executing;
        executing = &DummyCommandBlock;

        if (finished->callback) {
            (finished->callback)(-1, finished);
        }

        stateReady();
        return;
    }

    CurrCommand = executing->command;

    if ((CurrCommand == 32) || (CurrCommand == 14) || (CurrCommand == 35)) {
        ResumeFromHere = 0;
    }

    if (ResumeFromHere) {
        switch (ResumeFromHere) {
            case 2:
                executing->state = 11;
                stateMotorStopped();
                break;
            case 3:
                executing->state = 4;
                stateMotorStopped();
                break;
            case 4:
                executing->state = 5;
                stateMotorStopped();
                break;
            case 1:
            case 7:
            case 6:
                executing->state = 3;
                stateCoverClosed();
                break;

            case 5:
                stateError(CancelLastError);
                break;
        }

        ResumeFromHere = 0;
    }
    else {
        switch (MotorState) {
            case 2:
                if (MotorStopped) {
                    executing->state = 12;
                }
                else {
                    executing->state = 3;
                    stateCoverClosed();
                }
                break;
            case 0:
                executing->state = 1;
                stateBusy(executing);
                break;
            case 1:
            default:
                stateCoverClosed();
                break;
        }
    }
}

static void stateBusy(DVDCommandBlock* block) {
    DVDCommandBlock* finished;
    LastState = stateBusy;

    switch (block->command) {
        case 5:
        case 2:
        case 3:
        case 15:
        case 14:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 16:
        case 34:
        case 37:
        case 41:
        case 42:
            StampCommand(block->command, block->offset, block->length);
            break;
        default:
            break;
    }

    switch (block->command) {
        case 5:
            DVDLowClearCoverInterrupt(0);
            block->currTransferSize = sizeof(DVDDiskID);
            DVDLowReadDiskID(block->addr, cbForStateBusy);
            break;

        case 1:
        case 4:
            if (block->length) {
                finished = executing;
                executing = &DummyCommandBlock;
                finished->state = 0;

                if (finished->callback) {
                    (finished->callback)(0, finished);
                }

                stateReady();
            }
            else {
                DVDLowClearCoverInterrupt(0);
                block->currTransferSize = MIN(block->length - block->transferredSize, 0x80000);
                StampCommand(block->command, ((block->offset) + (block->transferredSize >> 2)), block->currTransferSize);
                DVDLowRead((void*)((u8*)block->addr + block->transferredSize), block->currTransferSize, ((block->offset) + (block->transferredSize >> 2)), cbForStateBusy);
            }

            break;
        case 2:
            DVDLowClearCoverInterrupt(0);
            DVDLowSeek(block->offset, cbForStateBusy);
            break;
        case 3:
            DVDLowStopMotor(FALSE, FALSE, cbForStateBusy);
            break;
        case 15:
            DVDLowStopMotor(FALSE, FALSE, cbForStateBusy);
            break;
        case 13:
            DVDLowClearCoverInterrupt(0);
            DVDLowAudioBufferConfig(block->offset, block->length, cbForStateBusy);
            break;
        case 14:
            DVDLowClearCoverInterrupt(0);
            block->currTransferSize = sizeof(DVDDriveInfo);
            DVDLowInquiry(block->addr, cbForStateBusy);
            break;
        case 16:
            DVDLowClearCoverInterrupt(0);
            DVDLowStopMotor(FALSE, FALSE, cbForStateBusy);
            break;
        case 32:
            DVDLowSetSpinupFlag(1);
            DVDLowReset(cbForStateBusy);
            break;
        case 33:
            if (block->length == 0) {
                finished = executing;
                executing = &DummyCommandBlock;
                finished->state = 0;
                
                if (finished->callback) {
                    (finished->callback)(0, finished);
                }

                stateReady();
            }
            else {
                DVDLowClearCoverInterrupt(0);
                block->currTransferSize = MIN(block->length - block->transferredSize, 0x80000);
                StampCommand(block->command, ((block->offset) + (block->transferredSize >> 2)), block->currTransferSize);
                DVDLowUnencryptedRead((void*)((u8*)block->addr + block->transferredSize), block->currTransferSize, ((block->offset) + (block->transferredSize >> 2)), cbForStateBusy);
            }
            break;

            case 34:
                DVDLowClearCoverInterrupt(0);
                DVDLowOpenPartition(block->offset, NULL, 0, 0, (ESTitleMeta*)block->addr, cbForStateBusy);
                break;
            case 35:
                DVDLowClearCoverInterrupt(0);
                DVDLowClosePartition(cbForStateBusy);
                break;
            case 38:
                DVDLowPrepareCoverRegister(cbForStateBusy);
                break;
            case 36:
                DVDLowPrepareCoverRegister(cbForStateBusy);
                break;
            case 37:
                DVDLowClearCoverInterrupt(0);
                DVDLowSetMaximumRotation(0x20000, cbForStateBusy);
                break;
            case 40:
                DVDLowClearCoverInterrupt(0);
                block->addr = &CurrDiskID;
                block->currTransferSize = sizeof(DVDDiskID);
                DVDLowReadDiskID(block->addr, cbForStateBusy);
                break;
            case 41:
            {
                DVDPartitionParams* params;
                DVDLowClearCoverInterrupt(0);
                params = block->addr;

                if (!params->numTmdBytes && !params->numCertBytes) {
                    DVDLowGetNoDiscBufferSizes(block->offset, &params->numTmdBytes, &params->numCertBytes, cbForStateBusy);
                }
                else {
                    DVDLowGetNoDiscOpenPartitionParams(block->offset, &params->ticket, &params->numTmdBytes, 
                                                        &params->tmd, &params->numCertBytes, params->certificates, 
                                                        &params->dataWordOffset, params->h3Hash, cbForStateBusy);
                }
                break;
            }
            case 42:
            {
                DVDPartitionParams* params;
                DVDLowClearCoverInterrupt(0);
                params = block->addr;
                DVDLowOpenPartitionWithTmdAndTicketView(block->offset, &params->ticketView, params->numTmdBytes, &params->tmd,
                                                        params->numCertBytes, params->certificates, cbForStateBusy);
                break;
            }
            default:
                checkOptionalCommand(block, cbForStateBusy);
                break;
        
    }
}

static BOOL issueCommand(s32 prio, DVDCommandBlock* block) {
    BOOL level, result;

    if (autoInvalidation && USE_DMA(block->command)) {
        DCInvalidateRange(block->addr, block->length);
    }

    level = OSDisableInterrupts();
    block->state = 2;
    result = __DVDPushWaitingQueue(prio, block);

    if ((executing == NULL) && (PauseFlag == FALSE)) {
        stateReady();
    }

    OSRestoreInterrupts(level);
    return result;
}

BOOL DVDReadAbsAsyncPrio(DVDCommandBlock* block, void* addr, s32 length, u32 offset, DVDCBCallback callback, s32 prio) {
    BOOL idle;
    block->command = 1;
    block->addr = addr;
    block->length = (u32)length;
    block->offset = (u32)offset;
    block->transferredSize = 0;
    block->callback = callback;
    idle = issueCommand(prio, block);
    return idle;
}

BOOL DVDInquiryAsync(DVDCommandBlock* block, DVDDriveInfo* info, DVDCBCallback callback) {
    BOOL idle;
    block->command = 14;
    block->addr = (void*)info;
    block->length = sizeof(DVDDriveInfo);
    block->transferredSize = 0;
    block->callback = callback;
    idle = issueCommand(2, block);
    return idle;
}

s32 DVDGetCommandBlockStatus(const DVDCommandBlock* block) {
    BOOL enabled;
    s32 retVal;
    DVDCommandBlock* next;

    enabled = OSDisableInterrupts();

    if (((volatile DVDCommandBlock*)block)->state == 3) {
        retVal = 1;
    }
    else if (((volatile DVDCommandBlock*)block)->state == 5) {
        retVal = 4;
    }
    else if (executing == &__DVDStopMotorCommandBlock) {
        next = __DVDGetNextWaitingQueue();
        if (next) {
            if(block == next) {
                retVal = 1;
            }
            else {
                retVal = ((volatile DVDCommandBlock*)block)->state;
            }
        }
        else {
            if (block == &__DVDStopMotorCommandBlock) {
                retVal = 0;
            }
            else {
                retVal = ((volatile DVDCommandBlock*)block)->state;
            }
        }
    }
    else {
        retVal = ((volatile DVDCommandBlock*)block)->state;
    }

    OSRestoreInterrupts(enabled);

    return retVal;
}

s32 DVDGetDriveStatus(void) {
    BOOL enabled;
    s32 retVal;

    enabled = OSDisableInterrupts();

    if (FatalErrorFlag) {
        retVal = -1;
    }
    else if (PausingFlag) {
        retVal = 8;
    }
    else {
        if (executing == NULL) {
            retVal = 0;
        }
        else if (executing == &DummyCommandBlock) {
            retVal = 0;
        }
        else {
            retVal = DVDGetCommandBlockStatus(executing);
        }
    }

    OSRestoreInterrupts(enabled);

    return retVal;
}

BOOL DVDSetAutoInvalidation(BOOL autoInval) {
    BOOL prev = autoInvalidation;
    autoInvalidation = autoInval;
    return prev;
}

BOOL __DVDLowTestAlarm(const OSAlarm *);
static void stateReady(void);

BOOL __DVDTestAlarm(const OSAlarm* alarm) {
    if (alarm == &ResetAlarm) {
        return TRUE;
    }

    return __DVDLowTestAlarm(alarm);
}

BOOL __DVDStopMotorAsync(DVDCommandBlock* block, DVDCBCallback callback) {
    return TRUE;
}

void __DVDRestartMotor(void) {
    return;
}

static BOOL __DVDLowBreak(void) {
    Breaking = TRUE;
    return TRUE;
}

void DVDPause(void) {
    BOOL level = OSDisableInterrupts();
    PauseFlag = TRUE;

    if (executing == NULL) {
        PausingFlag = TRUE;
    }

    OSRestoreInterrupts(level);
}

void DVDResume(void) {
    BOOL enabled = OSDisableInterrupts();
    PauseFlag = FALSE;

    if (PausingFlag) {
        PausingFlag = FALSE;
        stateReady();
    }

    OSRestoreInterrupts(enabled);
}

BOOL DVDCancelAsync(DVDCommandBlock* block, DVDCBCallback callback) {
    BOOL enabled;
    DVDCommandBlock* finished;

    enabled = OSDisableInterrupts();

    switch (block->state) {
        case -1:
        case 0:
        case 10:
            if (callback) {
                (*callback)(0, block);
            }

            break;

        case 1:
            if (Canceling) {
                OSRestoreInterrupts(enabled);
                return FALSE;
            }

            Canceling = TRUE;
            CancelCallback = callback;

            if (block->command == 4 || block->command == 33 || block->command == 34 || block->command == 41 || block->command == 42 || block->command == 1) {
                __DVDLowBreak();
            }

            break;

        case 2:
            __DVDDequeueWaitingQueue(block);
            block->state = 10;

            if (block->callback) {
                (block->callback)(-3, block);
            }

            if (callback) {
                (*callback)(0, block);
            }

            break;

        case 3:
            switch (block->command) {
                case 5:
                case 4:
                case 13:
                case 15:
                case 33:
                case 34:
                case 37:
                case 41:
                case 42:
                    if (callback) {
                        (*callback)(0, block);
                    }

                    break;

                case 1:
                case 2:
                    if (__OSInIPL) {
                        finished = executing;
                        executing = &DummyCommandBlock;
                        block->state = 10;

                        if (block->callback) {
                            (block->callback)(-3, block);
                        }

                        if (callback) {
                            (*callback)(0, block);
                        }

                        stateReady();
                        break;
                    }

                default:
                    if (Canceling) {
                        OSRestoreInterrupts(enabled);
                        return FALSE;
                    }

                    Canceling = TRUE;
                    CancelCallback = callback;
                    break;
            }

            break;

        case 4:
        case 5:
        case 6:
        case 7:
        case 11:
            if (!(WaitingForCoverClose || WaitingForCoverOpen)) {
                OSRestoreInterrupts(enabled);
                return FALSE;
            }

            if (WaitingForCoverOpen) {
                OSCancelAlarm(&CoverAlarm);
                WaitingForCoverOpen = FALSE;
            }

            if (block->state == 4) {
                ResumeFromHere = 3;
            }

            if (block->state == 5) {
                ResumeFromHere = 4;
            }

            if (block->state == 6) {
                ResumeFromHere = 1;
            }

            if (block->state == 11) {
                ResumeFromHere = 2;
            }

            if (block->state == 7) {
                ResumeFromHere = 7;
            }

            finished = executing;
            executing = &DummyCommandBlock;
            block->state = 10;

            if (block->callback) {
                (block->callback)(-3, block);
            }

            if (callback) {
                (callback)(0, block);
            }

            stateReady();
            break;

        case 12:
            finished = executing;
            executing = &DummyCommandBlock;
            block->state = 10;

            if (block->callback) {
                (block->callback)(-3, block);
            }

            if (callback) {
                (callback)(0, block);
            }

            stateReady();
            break;
    }

    OSRestoreInterrupts(enabled);
    return TRUE;
}

static void cbForCancelSync(s32, DVDCommandBlock *);

s32 DVDCancel(DVDCommandBlock* block) {
    BOOL result;
    s32 state;
    u32 command;
    BOOL enabled;
    
    result = DVDCancelAsync(block, cbForCancelSync);

    if (result == FALSE) {
        return -1;
    }

    enabled = OSDisableInterrupts();

    for (;;) {
        state = ((volatile DVDCommandBlock*)block)->state;

        if ((state == 0) || (state == -1) || (state == 10)) {
            break;
        }

        if (state == 3) {
            command = ((volatile DVDCommandBlock*)block)->command;

            if ((command == 4) || (command == 5) || (command == 13) || (command == 33) || (command == 34) || (command == 41) ||
                (command == 42) || (command == 15) || (command == 37)) {
                break;
            }
        }

        OSSleepThread(&__DVDThreadQueue);
    }

    OSRestoreInterrupts(enabled);
    return 0;
}

static void cbForCancelSync(s32 result, DVDCommandBlock* block) {
    OSWakeupThread(&__DVDThreadQueue);
}

BOOL DVDCancelAllAsync(DVDCBCallback callback) {
    BOOL enabled, retVal;
    DVDCommandBlock* p;

    enabled = OSDisableInterrupts();
    DVDPause();

    while ((p = __DVDPopWaitingQueue()) != 0) {
        DVDCancelAsync(p, NULL);
    }

    if (executing) {
        retVal = DVDCancelAsync(executing, callback);
    }
    else {
        retVal = TRUE;
        if (callback) {
            (*callback)(0, NULL);
        }
    }

    DVDResume();
    OSRestoreInterrupts(enabled);
    return retVal;
}

DVDDiskID* DVDGetCurrentDiskID(void) {
    return (DVDDiskID*)OSPhysicalToCached(0);
}

static volatile u32 __BS2DVDLowIntType = 0;

static void __BS2DVDLowCallback(u32 type) {
    __BS2DVDLowIntType = type;
}

u32 __DVDGetCoverStatus(void) {
    u32 reg;
    __BS2DVDLowIntType = 0;
    DVDLowPrepareCoverRegister(__BS2DVDLowCallback);

    while (!__BS2DVDLowIntType) { 

    }

    if (!(__BS2DVDLowIntType & 1)) {
        return 0;
    }

    reg = DVDLowGetCoverRegister();

    if (__OSGetSystemTime() - LastResetEnd < DVD_RESETCOVER_TIMELAG_TICKS2) {
        return 0;
    }
    else if (reg & 1) {
        return 1;
    }
    else {
        return 2;
    }
}

void __DVDResetWithNoSpinup(void) {
    __BS2DVDLowIntType = 0;
    DVDLowSetSpinupFlag(0);
    DVDLowReset(__BS2DVDLowCallback);

    while (!__BS2DVDLowIntType) {

    }
}

BOOL DVDCheckDiskAsync(DVDCommandBlock* block, DVDCBCallback callback) {
    BOOL enabled;
    s32 retVal, state;

    enabled = OSDisableInterrupts();

    if (FatalErrorFlag) {
        state = -1;
    }
    else if (PausingFlag) {
        state = 8;
    }
    else {
        if (WaitingForCoverOpen) {
            state = 7;
        }
        if (WaitingForCoverClose) {
            state = 5;
        }
        else if (executing == NULL) {
            switch (ResumeFromHere) {
                case 3:
                {
                    state = 4;
                    break;
                }
                case 4:
                {
                    state = 5;
                    break;
                }
                case 1:
                {
                    state = 6;
                    break;
                }
                case 2:
                {
                    state = 11;
                    break;
                }
                case 7:
                {
                    state = 7;
                    break;
                }
                default:
                {
                    state = 0;
                    break;
                }
            }
        }
        else if (executing == &DummyCommandBlock) {
            state = 0;
        }
        else {
            state = executing->state;
        }
    }

    retVal = TRUE;
    switch (state) {
      case 1:
      case 9:
      case 10:
      case 2:
        block->state = 0;
        if (callback) {
          (*callback)(TRUE, block);
        }
        OSRestoreInterrupts(enabled);
        break;

      case -1:
      case 11:
      case 7:
      case 3:
      case 4:
      case 5:
      case 6:
      case 12:
        block->state = 0;
        if (callback) {
          (*callback)(FALSE, block);
        }
        OSRestoreInterrupts(enabled);
        break;

      case 0:
      case 8:
        OSRestoreInterrupts(enabled);

        block->command = 36;
        block->callback = callback;

        retVal = issueCommand(2, block);
        break;
    }

    return retVal;
}

void __DVDPrepareResetAsync(DVDCBCallback callback) {
    BOOL enabled = OSDisableInterrupts();

    __DVDClearWaitingQueue();

    if (Canceling) {
        CancelCallback = callback;
    }
    else {
        if (executing != NULL) {
            executing->callback = NULL;
        }

        DVDCancelAllAsync(callback);
    }

    OSRestoreInterrupts(enabled);
}

static volatile BOOL Prepared;

static void Callback(s32 result, DVDCommandBlock* block ){
    Prepared = TRUE;
}

void __DVDPrepareReset(void) {
    OSDisableInterrupts();

    Prepared = FALSE;

    __DVDPrepareResetAsync(Callback);
    OSEnableInterrupts();

    while (!(Prepared == TRUE)) {

    }
}
