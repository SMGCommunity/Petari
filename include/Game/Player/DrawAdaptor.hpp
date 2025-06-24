#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/Functor.hpp"

class DrawAdaptor : public NameObj {
public:
    DrawAdaptor(const MR::FunctorBase &, s32);

    virtual ~DrawAdaptor();
    virtual void draw() const;

    MR::FunctorBase* mFunctor;
};
