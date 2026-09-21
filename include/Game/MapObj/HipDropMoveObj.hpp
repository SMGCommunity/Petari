#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class HipDropMoveObj : public LiveActor {
public:
    /// @brief Creates a new `HipDropMoveObj`.
    /// @param pName A pointer to the null-terminated name of the object.
    HipDropMoveObj(const char* pName);

    virtual ~HipDropMoveObj();
    virtual void init(const JMapInfoIter&);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual void moveStart() {};
    virtual void moving() {};
    virtual void moveEnd() {};
    virtual bool isEndMove() const;

    void exeWait();
    void exeMove();
    void exeDone();

    /* 0x8C */ const char* mModelName;
};

class HipDropDemoMoveObj : public HipDropMoveObj {
public:
    /// @brief Creates a new `HipDropDemoMoveObj`.
    /// @param pName A pointer to the null-terminated name of the object.
    HipDropDemoMoveObj(const char* pName);

    virtual ~HipDropDemoMoveObj();
    virtual void moveStart();
    virtual void moving();

    /* 0x90 */ TMtx34f mMtx;
};
