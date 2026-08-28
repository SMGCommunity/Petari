#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class CameraTargetMtx;
class ModelObj;
class PrologueLetter;
class ProloguePictureBook;

class PrologueDirector : public LiveActor {
public:
    /// @brief Creates a new `PrologueDirector`.
    /// @param pName A pointer to the null-terminated name of the object.
    PrologueDirector(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x24 */ virtual void appear();
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();

    void exeWait();
    void exePictureBook();
    void exePeachLetterStart();
    void exePeachLetter();
    void exePeachLetterWait();
    void exePeachLetterEnd();
    void exeBindWait();
    void exeArrive();
    void exeGameStart();
    void createPictureBook();
    void createLetter();
    void createScenery();
    void createMarioPosDummyModel();
    void createCameraTarget();
    void pauseOff();

private:
    /* 0x8C */ ProloguePictureBook* mPictureBook;
    /* 0x90 */ PrologueLetter* mLetter;
    /* 0x94 */ ModelObj* mScenery;
    /* 0x98 */ ModelObj* mMarioPosDummyModel;
    /* 0x9C */ CameraTargetMtx* mCameraTarget;
    /* 0xA0 */ TMtx34f _A0;
    /* 0xD0 */ bool _D0;
};

class PrologueHolder : public NameObj {
public:
    /// @brief Creates a new `PrologueHolder`.
    /// @param pName The pointer to the null-terminated name of the object.
    PrologueHolder(const char* pName);

    void registerPrologueObj(PrologueDirector* pDirector);
    void start();

private:
    /* 0xC */ PrologueDirector* mDirector;
};

namespace MR {
    void startPrologue();
};  // namespace MR
