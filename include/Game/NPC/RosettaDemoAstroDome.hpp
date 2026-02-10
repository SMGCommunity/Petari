#pragma once

#include "Game/Demo/DemoFunction.hpp"
#include "Game/NPC/Rosetta.hpp"
#include "Game/NPC/TalkMessageFunc.hpp"
#include "Game/NPC/TalkTextFormer.hpp"
#include "Game/Screen/SimpleLayout.hpp"
#include "Game/System/NerveExecutor.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/TalkUtil.hpp"

class IconAButton;
class NameObjArchiveListCollector;
class RosettaMonologue;

class RosettaDemoAstroDomeExplain : public NerveExecutor {
public:
    /// @brief Creates a new `RosettaDemoAstroDomeExplain`.
    /// @param pRosetta The pointer to the actor instance for Rosalina.
    /// @param rIter TODO.
    RosettaDemoAstroDomeExplain(Rosetta* pRosetta, const JMapInfoIter& rIter);

    static void makeArchiveList(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter);

    void startDemo();
    void exeDemo();

    /// @brief The pointer to the actor instance for Rosalina.
    /* 0x08 */ Rosetta* mRosetta;
    /* 0x0C */ RosettaMonologue* mMonologue;
};

class RosettaDemoAstroDomeFinalBattle : public NerveExecutor {
public:
    /// @brief Creates a new `RosettaDemoAstroDomeFinalBattle`.
    /// @param pRosetta The pointer to the actor instance for Rosalina.
    /// @param rIter TODO.
    RosettaDemoAstroDomeFinalBattle(Rosetta* pRosetta, const JMapInfoIter& rIter);

    void startDemo();
    void exeDemo();

    /// @brief The pointer to the actor instance for Rosalina.
    /* 0x08 */ Rosetta* mRosetta;
};

class RosettaDemoAstroDomeTalk : public NerveExecutor {
public:
    /// @brief Creates a new `RosettaDemoAstroDomeTalk`.
    /// @param pRosetta The pointer to the actor instance for Rosalina.
    /// @param rIter TODO.
    RosettaDemoAstroDomeTalk(Rosetta* pRosetta, const JMapInfoIter& rIter);

    void startDemo();
    void exeDemo();

    /// @brief The pointer to the actor instance for Rosalina.
    /* 0x08 */ Rosetta* mRosetta;
};

class RosettaMonologue : public SimpleLayout {
public:
    RosettaMonologue();

    virtual void movement();
    virtual void calcAnim();
    virtual void appear();
    virtual void control();

    /* 0x20 */ s32 mMsgID;
    /* 0x24 */ TalkTextFormer mTextFormer;
    /* 0x34 */ IconAButton* mIcon;
};
