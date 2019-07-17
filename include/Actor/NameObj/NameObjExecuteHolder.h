#ifndef NAMEOBJEXECUTEHOLDER_H
#define NAMEOBJEXECUTEHOLDER_H

class NameObj;

namespace MR
{
    void connectToSceneTemporarily(NameObj *);
    void disconnectToSceneTemporarily(NameObj *);
    void connectToDrawTemporarily(NameObj *);
    void disconnectToDrawTemporarily(NameObj *);
};

#endif // NAMEOBJEXECUTEHOLDER_H