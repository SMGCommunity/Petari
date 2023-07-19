#include <revolution/os.h>

static DVDCommandBlock* executing;
static vu32 CurrCommand;
static vu32 Canceling = FALSE;
static DVDCBCallback CancelCallback;
static volatile BOOL PauseFlag;
static volatile BOOL PausingFlag;
static vu32 WaitingForCoverOpen  = FALSE;
static vu32 WaitingForCoverClose = FALSE;
static vu32 MotorStopped = FALSE;
static vu32 MotorState;
static vu32 CoverState;

static vu32 ResumeFromHere;

static DVDCommandBlock DummyCommandBlock;

static OSAlarm ResetAlarm;
static OSAlarm CoverAlarm;

extern BOOL __OSInIPL;

static vu32 Breaking = FALSE;

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

void DVDPause(void) {
    BOOL enabled = OSDisableInterrupts();
    PauseFlag = TRUE;

    if (executing == NULL) {
        PauseFlag = TRUE;
    }

    OSRestoreInterrupts(enabled);
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

BOOL DVDCancelAllAsync(DVDCBCallback callback) {
    BOOL enabled, retVal;
    DVDCommandBlock* p;

    enabled = OSDisableInterrupts();
    DVDPause();

    while ((p = __DVDPopWaitingQueue()) != NULL) {
        DVDCancelAsync(p, NULL);
    }

    if (executing != NULL) {
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
