#ifndef LAYOUT_H
#define LAYOUT_H

#include "nw4r/lyt/animation.h"
#include "nw4r/ut/LinkList.h"
#include "nw4r/lyt/lyt_types.h"
#include "nw4r/lyt/resourceAccessor.h"

namespace nw4r
{
    namespace lyt
    {
        class Pane;
        class GroupContainer;
        typedef ut::LinkList<AnimTransform, offsetof(AnimTransform, mLink)> AnimTransformList;

        class Layout
        {
        public:
            Layout();

            virtual ~Layout();
            virtual bool Build(const void *, ResourceAccessor *);

            AnimTransformList mTransformList; // _0
            Pane* mRootPane;
            GroupContainer* mContainer;
            Size mSize;
        };
    };
};

#endif // LAYOUT_H