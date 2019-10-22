#include "System/Galaxy/GalaxyCometScheduler.h"
#include "System/Game/GameDataFunction.h"

const GalaxyCometTimePaper sCometTimeTableGrandGalaxy1[16] =
{
    { "EggStarGalaxy", 0 },
    { "HoneyBeeKingdomGalaxy", 0 },
    { "HoneyBeeKingdomGalaxy", 1 },
    { "EggStarGalaxy", 1 },
    { "EggStarGalaxy", 0 },
    { "HoneyBeeKingdomGalaxy", 1 },
    { "EggStarGalaxy", 1 },
    { "HoneyBeeKingdomGalaxy", 1 },
    { "EggStarGalaxy", 0 },
    { "HoneyBeeKingdomGalaxy", 1 },
    { "EggStarGalaxy", 1 },
    { "HoneyBeeKingdomGalaxy", 0 },
    { "HoneyBeeKingdomGalaxy", 0 },
    { "EggStarGalaxy", 0 },
    { "HoneyBeeKingdomGalaxy", 1 },
    { "EggStarGalaxy", 1 }
};

const GalaxyCometTimePaper sCometTimeTableGrandGalaxy2[16] =
{
    { "StarDustGalaxy", 0 },
    { "StarDustGalaxy", 1 },
    { "BattleShipGalaxy", 0 },
    { "BattleShipGalaxy", 1 },
    { "StarDustGalaxy", 1 },
    { "BattleShipGalaxy", 0 },
    { "StarDustGalaxy", 0 },
    { "BattleShipGalaxy", 1 },
    { "StarDustGalaxy", 0 },
    { "StarDustGalaxy", 1 },
    { "BattleShipGalaxy", 0 },
    { "BattleShipGalaxy", 1 },
    { "StarDustGalaxy", 0 },
    { "BattleShipGalaxy", 0 },
    { "StarDustGalaxy", 1 },
    { "BattleShipGalaxy", 1 },
};

const GalaxyCometSerializeInfo _806C14F0[2] =
{
    { 7, 0 },
    { 0, 0 }
};

const GalaxyCometTimePaper* sGalaxyCometTimerPaper[7] =
{
    sCometTimeTableGrandGalaxy1,
    sCometTimeTableGrandGalaxy2,
    /*sCometTimeTableGrandGalaxy3,
    sCometTimeTableGrandGalaxy4,
    sCometTimeTableGrandGalaxy5,
    sCometTimeTableGrandGalaxy6*/
};

const GalaxyCometSerializeInfo _8053C8F0[10] =
{
    { 1, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0x4142, 0x4344 },
    { 0x4546, 0x4748 },
    { 0x4900, 0 },
    { 0, 0 }
};

GalaxyCometScheduler::GalaxyCometScheduler()
{
    mCometTables = 0;
    mNumTables = 0;
    mIsActive = true;

    //mCometTables = new GalaxyCometTimeTable[6];
    mNumTables = 6;

    s32 curIdx = 0;

    while (curIdx < mNumTables)
    {
        mCometTables[curIdx] = new GalaxyCometTimeTable(sGalaxyCometTimerPaper[curIdx]);
        curIdx++;
    }
}

void GalaxyCometScheduler::update()
{
    if (!mIsActive)
    {
        return;
    }

    s32 curIdx = 0;
    while(curIdx < mNumTables)
    {
        mCometTables[curIdx]->update();
        curIdx++;
    }
}

void GalaxyCometScheduler::activate()
{
    mIsActive = true;
}

void GalaxyCometScheduler::deactivate()
{
    mIsActive = false;
}

void GalaxyCometScheduler::forceToNextState()
{
    s32 curIdx = 0;
    while (curIdx < mNumTables)
    {
        GalaxyCometTimeTable* table = mCometTables[curIdx];
        table->mCometState->forceToNext();
        table->updateTimePaperPos();
        curIdx++;
    }

    if (GameDataFunction::isPowerStarLeftInCometOnly())
    {
        setScheduleMostForwardCometAppear();
    }
}

void GalaxyCometScheduler::syncWithFlags()
{
    s32 curIdx = 0;
    while (curIdx < mNumTables)
    {
        mCometTables[curIdx]->checkIsReady();
    }

    if (GameDataFunction::isOnJustGameEventFlag("EventCometStarter"))
    {
        setScheduleFirstContact();
    }
}

void GalaxyCometScheduler::updateStateToGameData()
{
    s32 curIdx = 0;
    while (curIdx < mNumTables)
    {
        GalaxyCometSerializeInfo info;
        mCometTables[curIdx]->serializeStateToGameData(&info);
        GameDataFunction::updateGalaxyCometStatus(curIdx + 1, info._0, info._2);
        curIdx++;
    }
}

void GalaxyCometScheduler::restoreStateFromGameData()
{
    s32 curIdx = 0;
    while (curIdx < mNumTables)
    {
        GalaxyCometSerializeInfo info;
        GameDataFunction::restoreGalaxyCometStatus(curIdx + 1, &info._0, &info._2);
        mCometTables[curIdx]->deserializeStateFromGameData(&info);
        curIdx++;
    }

    if (GameDataFunction::isOnJustGameEventFlag("Coin100CometEggStarGalaxy"))
    {
        if (!GameDataFunction::isOnGameEventFlag("SpecialStarCoin100CometRelease"))
        {
            mCometTables[0]->deserializeStateFromGameData(_806C14F0);
        }
    }
}

void GalaxyCometScheduler::setScheduleFirstContact()
{
    s32 curIdx = 0;
    while (curIdx < mNumTables)
    {
        mCometTables[curIdx]->deserializeStateFromGameData(&_806C14F0[curIdx]);
        curIdx++;
    }
}

void GalaxyCometScheduler::setScheduleMostForwardCometAppear()
{
    if (isHideAll())
    {
        /* todo */
    }
}