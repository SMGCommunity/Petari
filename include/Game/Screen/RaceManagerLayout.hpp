#pragma once

#include "Game/Screen/LayoutActor.hpp"

class RaceManagerLayout : public LayoutActor {
public:
    RaceManagerLayout(const char* pName);

    virtual ~RaceManagerLayout();
    virtual void init(const JMapInfoIter&);
    void playCountAndGo();
    void playGo();
    void playGoal();
    void playLose();
    void playTimeUp();
    void playRank(u32);
    void isAllAnimStopped();
    void hideRecordPane();
    void hideBestRecordPane();
    void hideAllPane();
    void isPlayCountAnim() const;
    void isPlayCountAndGoAnim() const;
    void setTime(u32);
};
