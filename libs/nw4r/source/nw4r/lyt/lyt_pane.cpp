#include "nw4r/lyt/lyt_pane.h"
#include <cstring>

namespace nw4r {
    namespace lyt {
        void Pane::SetName(const char *name) {
            strncpy(mName, name, sizeof(mName) - 1);
            mName[sizeof(mName) - 1] = '\0';
        }

        void Pane::AppendChild(Pane* pChild) {
            InsertChild(mChildList.GetEndIter(), pChild);
        }

        void Pane::InsertChild(PaneList::Iterator next, Pane* pChild) {
            mChildList.Insert(next, pChild);
            pChild->mpParent = this;
        }

        void Pane::RemoveChild(Pane* pChild) {
            mChildList.Erase(pChild);
            pChild->mpParent = nullptr;
        }

        const ut::Color Pane::GetVtxColor(u32) const {
            return ut::Color();
        }

        void Pane::SetVtxColor(u32 , ut::Color) {

        }

        u8 Pane::GetColorElement(u32 idx) const {
            switch (idx) {
                case 16:
                    return mAlpha;
                default:
                    return GetVtxColorElement(idx);
            }
        }

        void Pane::SetColorElement(u32 idx, u8 val) {
            switch (idx) {
                case 16:
                    mAlpha = val;
                    break;
                default:
                    SetVtxColorElement(idx, val);
            }
        }
    };
};