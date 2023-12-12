#include "nw4r/lyt/lyt_group.hpp"
#include "nw4r/lyt/lyt_layout.hpp"
#include <cstring>

/*
namespace nw4r {
    namespace lyt {
        Group::Group(const res::Group* pGroup, Pane* pRootPane) {
            Init();
            strncpy(mName, pGroup->name, sizeof(mName) - 1);
            mName[sizeof(mName) - 1] = '\0';

            const char* const paneName = detail::ConvertOffsToPtr<char>(pGroup, sizeof(res::Group));

            for (int i = 0; i < pGroup->paneNum; ++i) {
                if (Pane* p = pRootPane->FindPaneByName(paneName + i * 17, true)) {
                    AppendPane(p);
                }
            }
        }

        void Group::Init() {
            mbUserAllocated = false;
        }
        
        void Group::AppendPane(Pane* p) {
            if (detail::PaneLink* pLink = Layout::NewObj<detail::PaneLink>()) {
                pLink->mTarget = p;
                mPaneLinkList.PushBack(pLink);
            }
        }
    };
};
*/