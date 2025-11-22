#pragma once

#include "Game/NameObj/NameObj.hpp"

class SkeletalFishBabyRail;

class SkeletalFishBabyRailSetLinkNode {
public:
    SkeletalFishBabyRailSetLinkNode();

    SkeletalFishBabyRail* _0;
    SkeletalFishBabyRail* _4;
    u8 _8;
    SkeletalFishBabyRailSetLinkNode* _C;
    SkeletalFishBabyRailSetLinkNode* _10;
};

class SkeletalFishBabyRailGroupNode {
public:
    SkeletalFishBabyRailGroupNode(s32);

    void addChild(SkeletalFishBabyRail*);

    void createChild();
    void tidy();

    s32 _0;
    u32 mNumNodes;  // 0x4
    u32 _8;
    u32 _C;
    SkeletalFishBabyRailGroupNode* _10;
    SkeletalFishBabyRailGroupNode* _14;
    SkeletalFishBabyRailSetLinkNode* _18;
    SkeletalFishBabyRailSetLinkNode* _1C;
    SkeletalFishBabyRailSetLinkNode* _20;
    SkeletalFishBabyRailSetLinkNode* _24;
};

class SkeletalFishBabyRailHolder : public NameObj {
public:
    SkeletalFishBabyRailHolder(const char*);

    virtual ~SkeletalFishBabyRailHolder();
    virtual void initAfterPlacement();

    void add(SkeletalFishBabyRail*);
    SkeletalFishBabyRailGroupNode* createGroup(s32);

    SkeletalFishBabyRailGroupNode* mNodes;  // 0xC
};

namespace MR {
    void createSkeletalFishBabyRailHolder();
    SkeletalFishBabyRailHolder* getSkeletalFishBabyRailHolder();
};  // namespace MR