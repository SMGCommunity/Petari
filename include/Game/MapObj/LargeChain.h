#include "Game/LiveActor/LiveActor.h"
#include "Game/MapObj/LargeChainParts.h"

class LargeChain : public LiveActor {
public:
    LargeChain(const char *);

    virtual ~LargeChain();
    virtual void init(const JMapInfoIter &);
    virtual void makeActorAppeared();
    virtual void makeActorDead();

    void exeBreak();
    void createChainParts();
    void invalidateClippingAllChainParts();

    LargeChainParts* _8C;
    LargeChainParts* _90;
    /* 0x94 */  LargeChainParts* mChainArray;   ///< The array of LargeChainParts
    /* 0x98 */  s32 mChainCount;                ///< The number of chain parts.
};

namespace NrvLargeChain {
    NERVE_DECL_NULL(LargeChainNrvWait);
    NERVE_DECL(LargeChainNrvBreak, LargeChain, LargeChain::exeBreak);
};