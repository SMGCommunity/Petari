#include "nw4r/lyt/group.h"
#include "nw4r/lyt/layout.h"
#include <cstring>

namespace nw4r {
    namespace lyt {
        void Group::AppendPane(Pane *pPane) {
            if (detail::PaneLink* pPaneLink = Layout::NewObj<detail::PaneLink>()) {
                pPaneLink->mTarget = pPane;
                mPaneLinkList.PushBack(pPaneLink);
            }
        }

        void Group::Init() {
            mbUserAllocated = false;
        }

        Group::Group(const res::Group *pResGroup, Pane *pRootPane) {
            Init();
            strncpy(mName, pResGroup->name, sizeof(mName) - 1);
            mName[sizeof(mName) - 1] = '\0';

            const char *const paneNameBase = detail::ConvertOffsToPtr<char>(pResGroup, sizeof(res::Group));

            for (int i = 0; i < pResGroup->paneNum; ++i) {
                if (Pane* pFindPane = pRootPane->FindPaneByName(paneNameBase + i * 16, true)) {
                    AppendPane(pFindPane);
                }
            }
        }

        Group::~Group() {
            for (PaneLinkList::Iterator it = mPaneLinkList.GetBeginIter(); it != mPaneLinkList.GetEndIter();) {
                PaneLinkList::Iterator currIt = it++;
                mPaneLinkList.Erase(currIt);
                Layout::DeleteObj(&(*currIt));
            }
        }

        GroupContainer::~GroupContainer() {
            for (GroupList::Iterator it = mGroupList.GetBeginIter(); it != mGroupList.GetEndIter();) {
                GroupList::Iterator currIt = it++;
                mGroupList.Erase(currIt);
                if (!currIt->IsUserAllocated()) {
                    Layout::DeleteObj(&(*currIt));
                }
            }
        }

        void GroupContainer::AppendGroup(Group *pGroup) {
            mGroupList.PushBack(pGroup);
        }

        Group* GroupContainer::FindGroupByName(const char *findName) {
            for (GroupList::Iterator it = mGroupList.GetBeginIter(); it != mGroupList.GetEndIter(); ++it) {
                if (detail::EqualsResName(it->GetName(), findName)) {
                    return &(*it);
                }
            }

            return nullptr;
        }
    };
};
