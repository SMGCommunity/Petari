#pragma once

#include "Game/Screen/LayoutActor.hpp"

namespace nw4r {
    namespace lyt {
        class TexMap;
    };
}; // namespace nw4r

class ButtonPaneController;
class FileSelectIconID;
class MiiFaceIcon;

class MiiSelectIcon : public LayoutActor {
public:
    MiiSelectIcon(int, int, int, const char*);

    virtual void appear();
    virtual void control();

    void               appear(const FileSelectIconID&);
    void               appearMiiDummy();
    void               validate();
    void               invalidate();
    void               prohibit();
    bool               isSelected() const;
    bool               isMiiDummy() const;
    void               copyIconID(FileSelectIconID*);
    nw4r::lyt::TexMap* getTexMap();
    bool               isPointing() const;
    void               exeCreate();
    void               exeWait();
    void               exeSelected();
    void               exeDisappear();
    void               exeInvalid();
    void               createButton();
    void               createFaceImageObj();

private:
    /* 0x20 */ ButtonPaneController* _20;
    /* 0x24 */ ButtonPaneController* _24;
    /* 0x28 */ ButtonPaneController* _28;
    /* 0x2C */ MiiFaceIcon*          mIcon;
    /* 0x30 */ nw4r::lyt::TexMap*    mMiiTexMap;
    /* 0x34 */ nw4r::lyt::TexMap*    mFellowTexMap;
    /* 0x38 */ FileSelectIconID*     mIconID;
    /* 0x3C */ bool                  mIsMiiDummy;
    /* 0x3D */ bool                  _3D;
};
