#pragma once

class GameSequenceDirector {
public:
    GameSequenceDirector();

    void initAfterResourceLoaded();
    void update();
    void draw() const;

    u32 _0;
    u32 _4;
    u32 _8;
    u32 _C;
};
