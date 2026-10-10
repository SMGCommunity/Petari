#pragma once

#include "Game/NameObj/NameObj.hpp"

class SkeletalFishBabyRail;

class SkeletalFishBabyRailSetLinkNode {
public:
    SkeletalFishBabyRailSetLinkNode();

    /* 0x00 */ SkeletalFishBabyRail* _0;
    /* 0x04 */ SkeletalFishBabyRail* _4;
    /* 0x08 */ u8 _8;
    /* 0x0C */ SkeletalFishBabyRailSetLinkNode* _C;
    /* 0x10 */ SkeletalFishBabyRailSetLinkNode* _10;
};

class SkeletalFishBabyRailGroupNode {
public:
    SkeletalFishBabyRailGroupNode(s32);

    void addChild(SkeletalFishBabyRail*);

    void createChild();
    void tidy();

    /* 0x00 */ s32 _0;
    /* 0x04 */ u32 mNumNodes;
    /* 0x08 */ u32 _8;
    /* 0x0C */ u32 _C;
    /* 0x10 */ SkeletalFishBabyRailGroupNode* _10;
    /* 0x14 */ SkeletalFishBabyRailGroupNode* _14;
    /* 0x18 */ SkeletalFishBabyRailSetLinkNode* _18;
    /* 0x1C */ SkeletalFishBabyRailSetLinkNode* _1C;
    /* 0x20 */ SkeletalFishBabyRailSetLinkNode* _20;
    /* 0x24 */ SkeletalFishBabyRailSetLinkNode* _24;
};

class SkeletalFishBabyRailHolder : public NameObj {
public:
    SkeletalFishBabyRailHolder(const char*);

    virtual ~SkeletalFishBabyRailHolder();
    virtual void initAfterPlacement();

    void add(SkeletalFishBabyRail*);
    SkeletalFishBabyRailGroupNode* createGroup(s32);

    /* 0xC */ SkeletalFishBabyRailGroupNode* mNodes;
};

namespace MR {
    void createSkeletalFishBabyRailHolder();
    SkeletalFishBabyRailHolder* getSkeletalFishBabyRailHolder();
};  // namespace MR
