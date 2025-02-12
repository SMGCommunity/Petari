#pragma once

#include "nw4r/ut/LinkList.h"
#include "nw4r/lyt/pane.h"
#include "nw4r/lyt/resources.h"
#include <cstddef>

namespace nw4r {
    namespace lyt {
        namespace detail {
            struct PaneLink {
                ut::LinkListNode mLink;
                Pane* mTarget;
            };
        };

        #ifdef __MWERKS__
        typedef ut::LinkList<detail::PaneLink, offsetof(detail::PaneLink, mLink)> PaneLinkList;
        #else
        typedef ut::LinkList<detail::PaneLink, 0>   PaneLinkList;
        #endif

        class Group {
        public:
            Group(const res::Group *, Pane *);

            virtual ~Group();

            void Init();

            const char* GetName() const {
                 return mName;
            }

            PaneLinkList& GetPaneList() { 
                return mPaneLinkList;
            }

            inline void AppendPane(Pane *);

            bool IsUserAllocated() const {
                return mbUserAllocated;
            }

            ut::LinkListNode mLink;
            PaneLinkList mPaneLinkList;
            char mName[17];
            u8 mbUserAllocated;
            u8 mPadding[2];
        };

        #ifdef __MWERKS__
        typedef ut::LinkList<Group, offsetof(Group, mLink)> GroupList;
        #else
        typedef ut::LinkList<Group, 0> GroupList;
        #endif

        class GroupContainer {
        public:
            GroupContainer() {

            }

            ~GroupContainer();

            void AppendGroup(Group *);

            Group* FindGroupByName(const char *);

            GroupList mGroupList;
        };
    };
};
