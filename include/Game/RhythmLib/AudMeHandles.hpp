#pragma once

class AudMeHandle;

class AudMeHandles {
public:
    AudMeHandles(AudMeHandle*, int);

    AudMeHandle* getHandleMeID(u32);
    AudMeHandle* getFreeHandle();

private:
    /* 0x00 */ AudMeHandle* mHandles;
    /* 0x04 */ int mHandleNum;
};
