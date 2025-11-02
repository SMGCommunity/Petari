#pragma once

class GameDataHolder;
class GameDataTemporaryInGalaxy;
class GameSequenceProgress;
class NWC24Messenger;
class SaveDataHandleSequence;

class GameSequenceDirector {
public:
    /// @brief Creates a new `GameSequenceDirector`.
    GameSequenceDirector();

    void initAfterResourceLoaded();
    void update();
    void draw() const;
    bool isInitializedGameDataHolder() const;
    GameDataHolder* getGameDataHolder();
    void executeOnSaveSuccess();
    void executeJustBeforeSave();

    /* 0x00 */ GameDataTemporaryInGalaxy* mGameDataTemporaryInGalaxy;
    /* 0x04 */ GameSequenceProgress* mGameSequenceProgress;
    /* 0x08 */ SaveDataHandleSequence* mSaveDataHandleSequence;
    /* 0x0C */ NWC24Messenger* mNWC24Messenger;
};
