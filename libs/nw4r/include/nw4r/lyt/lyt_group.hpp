#pragma once

#include "nw4r/lyt/lyt_pane.hpp"
#include "nw4r/ut/LinkList.hpp"

namespace nw4r {
    namespace lyt {
        namespace detail {
            struct PaneLink {
                ut::LinkListNode    mLink;
                Pane*               mTarget;
            };
        };

        typedef ut::LinkList<detail::PaneLink, offsetof(detail::PaneLink, mLink)> PaneLinkList;
    
        class Group {
        public:
            Group(const res::Group *, Pane *);

            virtual ~Group();

            void Init();
            void AppendPane(Pane *);

            ut::LinkListNode mLink;
            PaneLinkList mPaneLinkList;
            char mName[17];
            u8 mbUserAllocated;
            u8 mPad[2];
        };
    };
};